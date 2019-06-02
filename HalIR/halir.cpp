#include "halir.hpp"
#include "voigt.hpp"
#include "spc.hpp"

#include <omp.h>
#include "Faddeeva.hpp"

using namespace std;

HalIR::HalIR(const HalIRSpec &spec) : HalIRSpec(spec)
{
  // Read some values from spec

  // if ( simulate ) { cout << "This is a simulation" << endl;}
}
HalIR::HalIR(string parm_in) : HalIRSpec(parm_in)
{
  // Set some things
}
void HalIR::runDawsonVoigt()
{
  using namespace arma;
  //int MNPTS=0;
  //cout << "Run!: " << mparm.size() << " lines\n";
  int cc=0;
  for( auto m : mparm ) {
    unsigned idx;
    size_t msize=m->nlines;
    fvec t_mu = fvec(m->trans_mu,msize,false,true);
    fvec p_S = fvec(m->pressure_S,msize,false,true);
    fvec a_B = fvec(m->air_B,msize,false,true);
    fvec s_B = fvec(m->self_B,msize,false,true);
    fvec MM(msize);
    fvec taB(msize);
    for(unsigned i=0;i<msize;i++) {
        MM[i]=(float)hitran.GetMolecM(m->molec_num[i]*10+m->isotp_num[i]);
        taB[i]=pow( (296/temp),m->temp_air_B[i] );
    }
    MM=arma::sqrt(MM);
    float tfac=sqrt( ( 2*Na*kbs*temp*ln2*1e3 ) );
    q=m->conc/press;
    //cerr << "vmr: " << q << " lines: " << msize << endl;
    v0=t_mu+p_S*press;
    alphaD=v0*tfac/(3e8*MM);
    alphaL=press*taB%( (press-m->conc)*a_B+s_B*m->conc );
    float sig_v = 0.1;
    float mu_step=(sig_v*(alphaL.min()+alphaD.min()));
    float mu_off1=ceil(50*max(alphaD.max(),alphaL.max()));
    fvec yy=sqrt_ln2*alphaL/alphaD;
    mu=regspace<fvec>(m->llim-mu_off1,mu_step,m->hlim+mu_off1);
    //y.zeros( mu.size() );
    int off1 = (int)ceil(mu_off1/mu_step);
/*
    for(unsigned mm=0;mm<msize;mm++) {
        idx = arma::as_scalar(arma::find( mu>v0[mm], 1, "first" ));
        float qvr = tips(m->molec_num[mm],m->isotp_num[mm],temp);
        float qvr_ref=tips(m->molec_num[mm],m->isotp_num[mm],296);
        float top = ( qvr_ref*exp(hc*m->low_state_en[mm]/(kb*temp))*(1-exp(-hc*v0[mm]/(kb*temp))));
        float bot = ( qvr*exp(hc*m->low_state_en[mm]/(kb*296))*(1-exp(-hc*v0[mm]/(kb*296))));
        #pragma omp parallel for
        for(unsigned i=(idx-off1);i<(idx+off1);i++)
            //y[i]+=(top/bot)*m->line_I[mm]*gfunct(mu[i],v0[mm])*voigt((mu[i]-v0[mm])/alphaD[mm],yy[mm]);
            y[i]+=(sqrt_ln2/(sqrt_pi*alphaD[mm]))*(top/bot)*m->conc*NL*pathL*298/temp*m->line_I[mm]*(float)Faddeeva::w(std::complex<double>((double)sqrt_ln2*(mu[i]-v0[mm])/alphaD[mm],(double)yy[mm])).real();
    }
    */
/*
    fvec split = regspace<fvec>(-10.,mu_step,10.);
    //cerr << "mu_step: " << mu_step << " res: " << res << endl;
    SINC sinc(res);
    split.transform(sinc);
    split=split/sum(split);
    //fvec tran = conv(y,split,"same");
    for(unsigned i=0;i<tran.size();i++)
        cout << fixed << setprecision(6) << mu[i] << " " << scientific << exp(-tran[i]) << endl;

    cc++;
  }
  */
  /*
  for( auto s : spectras.spectras) {
      s->write();*/
  }
}
// Place holder
void HalIR::createCalibrationSpectra()
{
  cout << "Lets create some spectra\n"
       << "Press: " << press << " Temp: " << temp << endl
       << "Ncomp: " << comp.size() << endl;
  for( auto c : comp) {
    cout << c->molec << " " << c->isotop << " " << c->hpar << " " << c->vmr << endl;
  }
}
// calculate spectra from the parameters
void HalIR::calcSpectra()
{
  using namespace arma;
  //int MNPTS=0;
  //cout << "Run!: " << cmp.mparm->nlines << " lines\n";
  int cc=0;
  for( auto cmp : comp ) {
    unsigned idx,npts;
    double xdata[2];
    size_t msize=cmp->parm->nlines;
    fvec t_mu = fvec(cmp->parm->trans_mu,msize,false,true);
    fvec p_S = fvec(cmp->parm->pressure_S,msize,false,true);
    fvec a_B = fvec(cmp->parm->air_B,msize,false,true);
    fvec s_B = fvec(cmp->parm->self_B,msize,false,true);
    fvec MM  = fvec(cmp->parm->molecMass,msize,true);
    fvec taB(msize);
    for(unsigned i=0;i<msize;i++) {
        taB[i]=pow( (296/temp),cmp->parm->temp_air_B[i] );
    }
    MM=arma::sqrt(MM);
    float tfac=sqrt( ( 2*Na*kbs*temp*ln2*1e3 ) );
    q=cmp->vmr;
    //cerr << "vmr: " << q << " lines: " << msize << endl;
    v0=t_mu+p_S*press;
    alphaD=v0*tfac/(3e8*MM);
    alphaL=press*taB%( (press-q)*a_B+s_B*q );
    float sig_v = 0.1;
    float mu_step=(sig_v*(alphaL.min()+alphaD.min()));
    float mu_off1=ceil(50*max(alphaD.max(),alphaL.max()));
    fvec yy=sqrt_ln2*alphaL/alphaD;
    mu=regspace<fvec>(cmp->parm->llim-mu_off1,mu_step,cmp->parm->hlim+mu_off1);
    cmp->ROI[0] = (double)cmp->parm->llim-mu_off1;
    cmp->ROI[1] = (double)cmp->parm->hlim+mu_off1;
    npts = (unsigned)mu.size();
    cmp->npts = npts;
    y.zeros( mu.size() );
    int off1 = (int)ceil(mu_off1/mu_step);

    for(unsigned mm=0;mm<msize;mm++) {
        idx = arma::as_scalar(arma::find( mu>v0[mm], 1, "first" ));
        float qvr = tips(cmp->parm->molec_num[mm],cmp->parm->isotp_num[mm],temp);
        float qvr_ref=tips(cmp->parm->molec_num[mm],cmp->parm->isotp_num[mm],296);
        float top = ( qvr_ref*exp(hc*cmp->parm->low_state_en[mm]/(kb*temp))*(1-exp(-hc*v0[mm]/(kb*temp))));
        float bot = ( qvr*exp(hc*cmp->parm->low_state_en[mm]/(kb*296))*(1-exp(-hc*v0[mm]/(kb*296))));

        #pragma omp parallel for
        for(unsigned i=(idx-off1);i<(idx+off1);i++)
            //y[i]+=(top/bot)*m->line_I[mm]*gfunct(mu[i],v0[mm])*voigt((mu[i]-v0[mm])/alphaD[mm],yy[mm]);
            y[i]+=(sqrt_ln2/(sqrt_pi*alphaD[mm]))*(top/bot)*q*NL*pathL*298/temp*cmp->parm->line_I[mm]*(float)Faddeeva::w(std::complex<double>((double)sqrt_ln2*(mu[i]-v0[mm])/alphaD[mm],(double)yy[mm])).real();
    }
    fvec split = regspace<fvec>(-10.,mu_step,10.);
    //cerr << "mu_step: " << mu_step << " res: " << res << endl;
    SINC sinc(res);
    split.transform(sinc);
    split=split/sum(split);
    fvec tran = conv(y,split,"same");
    fvec ret = arma::exp(-tran);
    cmp->absCoeff = new float[npts];
    cmp->mu = new float[npts];

    memcpy(cmp->absCoeff, ret.memptr(),sizeof(float)*npts);
    memcpy(cmp->mu, mu.memptr(),sizeof(float)*npts);

  }
}

bpy::list HalIR::getSpectra()
{
  bpy::list ret;
  for( auto cmp : comp ) {
    bpy::dict spec;
    bpy::list r1, r2;
    for(int i=0;i<cmp->npts;i++) {
      r1.append(cmp->absCoeff[i]);
      r2.append(cmp->mu[i]);
    }
    spec["molec"] = cmp->molec;
    spec["absCoeff"] = r1;
    spec["mu"] = r2;
    ret.append(spec);
  }
  return ret;
}
