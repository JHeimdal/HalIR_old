#include "halir.hpp"
#include "voigt.hpp"

#include <omp.h>
#include "Faddeeva.hpp"

using namespace std;

HalIR::HalIR(const HalIRSpec &spec) : HalIRSpec(spec)
{
  // Read some values from spec

  Temp=getTemp();
  Press=getPress();
  PathL=getPathl();
  Resol=getResol();
  simulate=isSimulation();
  // if ( simulate ) { cout << "This is a simulation" << endl;}
}
HalIR::HalIR(string &parm_in, bool file_name) : HalIRSpec(parm_in, file_name)
{
    // Init HalIRSpec
    Temp=getTemp();
    Press=getPress();
    PathL=getPathl();
    Resol=getResol();
    simulate=isSimulation();
    //if ( simulate ) { cout << "This is a simulation" << endl;}
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
        taB[i]=pow( (296/Temp),m->temp_air_B[i] );
    }
    MM=arma::sqrt(MM);
    float tfac=sqrt( ( 2*Na*kbs*Temp*ln2*1e3 ) );
    q=m->conc/Press;
    //cerr << "vmr: " << q << " lines: " << msize << endl;
    v0=t_mu+p_S*Press;
    alphaD=v0*tfac/(3e8*MM);
    alphaL=Press*taB%( (Press-m->conc)*a_B+s_B*m->conc );
    float sig_v = 0.1;
    float mu_step=(sig_v*(alphaL.min()+alphaD.min()));
    float mu_off1=ceil(50*max(alphaD.max(),alphaL.max()));
    fvec yy=sqrt_ln2*alphaL/alphaD;
    mu=regspace<fvec>(m->llim-mu_off1,mu_step,m->hlim+mu_off1);
    y.zeros( mu.size() );
    int off1 = (int)ceil(mu_off1/mu_step);

    for(unsigned mm=0;mm<msize;mm++) {
        idx = arma::as_scalar(arma::find( mu>v0[mm], 1, "first" ));
        float qvr = tips(m->molec_num[mm],m->isotp_num[mm],Temp);
        float qvr_ref=tips(m->molec_num[mm],m->isotp_num[mm],296);
        float top = ( qvr_ref*exp(hc*m->low_state_en[mm]/(kb*Temp))*(1-exp(-hc*v0[mm]/(kb*Temp))));
        float bot = ( qvr*exp(hc*m->low_state_en[mm]/(kb*296))*(1-exp(-hc*v0[mm]/(kb*296))));
        #pragma omp parallel for
        for(unsigned i=(idx-off1);i<(idx+off1);i++)
            //y[i]+=(top/bot)*m->line_I[mm]*gfunct(mu[i],v0[mm])*voigt((mu[i]-v0[mm])/alphaD[mm],yy[mm]);
            y[i]+=(sqrt_ln2/(sqrt_pi*alphaD[mm]))*(top/bot)*m->conc*NL*PathL*298/Temp*m->line_I[mm]*(float)Faddeeva::w(std::complex<double>((double)sqrt_ln2*(mu[i]-v0[mm])/alphaD[mm],(double)yy[mm])).real();
    }

    fvec split = regspace<fvec>(-10.,mu_step,10.);
    //cerr << "mu_step: " << mu_step << " Resol: " << Resol << endl;
    SINC sinc(Resol);
    split.transform(sinc);
    split=split/sum(split);
    fvec tran = conv(y,split,"same");
    for(unsigned i=0;i<tran.size();i++)
        cout << fixed << setprecision(6) << mu[i] << " " << scientific << exp(-tran[i]) << endl;

    cc++;
  }
  /*
  for( auto s : spectras.spectras) {
      s->write();
  }
  */
}
