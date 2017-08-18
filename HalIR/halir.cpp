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
  simulate=isSimulation();
  if ( simulate ) { cout << "This is a simulation" << endl;}
}
HalIR::HalIR(string &parm_in) : HalIRSpec(parm_in)
{
    // Init HalIRSpec
    Temp=getTemp();
    Press=getPress();
    PathL=getPathl();
    simulate=isSimulation();
    //if ( simulate ) { cout << "This is a simulation" << endl;}
}
void HalIR::runDawsonVoigt()
{
  using namespace arma;
  int MNPTS=0;
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
    for(int i=0;i<msize;i++) {
        MM[i]=(float)c_sq*atmmass*hitran.GetMolecM(m->molec_num[i]*10+m->isotp_num[i]);
        taB[i]=pow( (296/Temp),m->temp_air_B[i] );
    }
    MM=arma::sqrt(MM);
    float tfac=sqrt( ( 2*kb*Temp ));
    q=m->conc/Press;
    cerr << "vmr: " << q << " lines: " << msize << endl;


    v0=t_mu+p_S*Press;
    alphaD=v0*tfac/MM;
    alphaL=Press*taB%( (1.-q)*a_B+(q*s_B) );
    float sig_v = 0.2;
    float mu_step=(sig_v*(alphaL.min()+alphaD.min()));
    float mu_off1=ceil(50*max(alphaD.max(),alphaL.max()));
    //cerr << "mu_off1: " << mu_off1 << endl;
    //float mu_off2=ceil(30*(alphaL.min()+alphaD.max()));
    //float mu_off3=ceil(5000*alphaL.max());
    fvec yy=alphaL/alphaD;
    mu=regspace<fvec>(m->llim-mu_off1,mu_step,m->hlim+mu_off1);
    //cerr << "length mu: " << mu.size() << endl;
    //for( auto v : mu)
    //    cout << fixed << setprecision(6) << v << endl;
    /* Test of computation will change structure */
    //float qvr = tips(m->molec_num[0],m->isotp_num[0],Temp);
    //float qvr_ref=tips(m->molec_num[0],m->isotp_num[0],296);
    //float Top = ( qvr_ref*exp(hc*m->low_state_en[0]/(kb*Temp))*(1-exp(-hc*v0[0]/(kb*Temp))));
    //float Bot = ( qvr*exp(hc*m->low_state_en[0]/(kb*296))*(1-exp(-hc*v0[0]/(kb*296))));
    y.zeros( mu.size() );
    int off1 = (int)ceil(mu_off1/mu_step);
    //int off2 = (int)ceil(mu_off2/mu_step);
    //int off3 = (int)ceil(mu_off3/mu_step);
    //cerr << "off1: " << off1 << " off2: " << off2 << " off3: " << off3 << endl;
    float fact = 1/9.869233e-7;
    #pragma omp parallel for
    for(int mm=0;mm<msize;mm++) {
        idx = arma::as_scalar(arma::find( mu>v0[mm], 1, "first" ));
        for(int i=(idx-off1);i<(idx+off1);i++)
        //for(int i=0;i<mu.size();i++)
            y[i]+=m->line_I[mm]*gfunct(mu[i],v0[mm])*voigt((mu[i]-v0[mm])/alphaD[mm],yy[mm]);
            //y[i]+=m->line_I[mm]*(float)Faddeeva::w(std::complex<double>((double)(mu[i]-v0[mm])/alphaD[mm],(double)yy[mm])).real();
    }
    float u = fact*q*Press*PathL/(kb*Temp);
    fvec tran = exp(-u*y);
    for(int i=0;i<mu.size();i++)
        cout << fixed << setprecision(6) << mu[i] << " " << scientific << tran[i] << endl;
/*

    //bool end=false;
    #pragma omp parallel for
    for(int mm=0;mm<msize;mm++) {
      for(int i=0;i<mu.size();i++) {
		//y[i]+=m->line_I[mm]*gfunct(mu[i],v0[mm])*voigt((mu[i]-v0[mm])/alphaD[mm],yy[mm]);
        y[i]+=m->line_I[mm]*voigt((mu[i]-v0[mm])/alphaD[mm],yy[mm]);
      }
    }
    //for(int mm=0;mm<msize;mm++) {
    //  for(int i=0;i<mu.size();i++) {
    //    y[i]+=m->line_I[mm]*gfunct(mu[i],v0[mm])*Faddeeva::w(complex<double> {(mu[i]-v0[mm])/alphaD[mm],yy[mm]},1e-6).real();
    //  }
    //}
    double u = q*Press*PathL/(kb*Temp);
    for(int i=0;i<MNPTS;i++)
        cout << fixed << setprecision(6) << mu[i] << " " << scientific << y[i] << endl;
    //std::string fname=rootdir+"/"+m->name+"_"+std::to_string(cc)+".spc";
    //spectras.addSPC(fname, m->hlim, m->llim, MNPTS, y.memptr() );
*/
    cc++;
  }
  /*
  for( auto s : spectras.spectras) {
      s->write();
  }
  */
}
