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
    int msize=m.hline.size();
    alphaD.set_size(msize);
    alphaL.set_size(msize);
    v0.set_size(msize);
    q=m.conc/Press;
    cerr << "vmr: " << q << " lines: " << msize << endl;
    mu=linspace<fvec>(m.llim,m.hlim,MNPTS);
    y.zeros( mu.size() );
    for(int i=0;i<msize;i++ ) {
      v0[i]=m.hline[i].trans_mu+m.hline[i].pressure_S*Press;
      alphaD[i]=v0[i]*sqrt( ( 2*kb*Temp )/( c_sq*atmmass*hitran.GetMolecM(m.hline[i].molec_num*10+m.hline[i].isotp_num) ));
      alphaL[i]=( (1.-q)*m.hline[i].air_B+q*m.hline[i].self_B )*Press*pow( (296/Temp),m.hline[i].temp_air_B );
    }
    /* Test of computation will change structure */
    //bool end=false;
    for(int mm=0;mm<msize;mm++) {
      float yy=alphaL[mm]/alphaD[mm];
      for(int i=0;i<mu.size();i++) {
		y[i]+=m.hline[mm].line_I*gfunct(mu[i],v0[mm])*voigt((mu[i]-v0[mm])/alphaD[mm],yy);
      }
    }
    std::string fname=rootdir+"/"+m.name+"_"+std::to_string(cc)+".spc";
    spectras.addSPC(fname, m.hlim, m.llim, MNPTS, y.memptr() );
    cc++;
  }
  for( auto s : spectras.spectras) {
      s->write();
  }
}
void HalIR::runDawsonVoigt()
{

  //cout << "Running Dawson Voigt...\n";
  setupRun();
}
