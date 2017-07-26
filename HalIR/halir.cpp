#include "halir.hpp"
#include "voigt.hpp"

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
    if ( simulate ) { cout << "This is a simulation" << endl;}
}
void HalIR::setupRun()
{
  using namespace arma;
  int MNPTS=25538;
  cout << "Run!: " << mparm.size() << " lines\n";
  int cc=0;
  for( auto m : mparm ) {
    int msize=m->nlines;
    alphaD.set_size(msize);
    alphaL.set_size(msize);
    v0.set_size(msize);
    q=m->conc/Press;
    cerr << "vmr: " << q << " lines: " << msize << endl;
    mu=linspace<fvec>(m->llim,m->hlim,MNPTS);
    y.zeros( mu.size() );
    for(int i=0;i<msize;i++ ) {
      v0[i]=m->trans_mu[i]+m->pressure_S[i]*Press;
      alphaD[i]=v0[i]*sqrt( ( 2*kb*Temp )/( c_sq*atmmass*hitran.GetMolecM(m->molec_num[i]*10+m->isotp_num[i]) ));
      alphaL[i]=( (1.-q)*m->air_B[i]+q*m->self_B[i] )*Press*pow( (296/Temp),m->temp_air_B[i] );
    }
    /* Test of computation will change structure */
    //bool end=false;
    for(int mm=0;mm<msize;mm++) {
      float yy=alphaL[mm]/alphaD[mm];
      for(int i=0;i<mu.size();i++) {
		y[i]+=m->line_I[mm]*gfunct(mu[i],v0[mm])*voigt((mu[i]-v0[mm])/alphaD[mm],yy);
      }
    }
    for(int i=0;i<MNPTS;i++)
        cout << fixed << setprecision(6) << mu[i] << " " << scientific << y[i] << endl;
    //std::string fname=rootdir+"/"+m->name+"_"+std::to_string(cc)+".spc";
    //spectras.addSPC(fname, m->hlim, m->llim, MNPTS, y.memptr() );
    cc++;
  }
  /*
  for( auto s : spectras.spectras) {
      s->write();
  }
  */
}
void HalIR::runDawsonVoigt()
{

  //cout << "Running Dawson Voigt...\n";
  setupRun();
}
