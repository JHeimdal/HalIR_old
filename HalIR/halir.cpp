#include "halir.h"
#include "voigt.h"

using namespace std;

HalIR::HalIR(const HalIRSpec &spec) : HalIRSpec(spec)
{
  // Read some values from spec
  
  Temp=getTemp();
  Press=getPress();
  PathL=getPathl();
  //     << "Mparm Size: " << mparm.size() << endl
  //     << "hline Size: ";
  //for( auto m : mparm ) 
  //  cout << m.hline.size() << " ";
  //cout << endl;
}

void HalIR::setupRun()
{
  using namespace arma;
  for( auto m : mparm ) {
    int msize=m.hline.size();
    //cout << "Run!: " << msize << " lines\n";
    alphaD.set_size(msize);
    alphaL.set_size(msize);
    v0.set_size(msize);
    q=m.conc/Press;
    //cerr << "vmr: " << q << " lines: " << msize << endl;
    mu=linspace(m.llim,m.hlim,25536);
    y.zeros( mu.size() );
    for(int i=0;i<msize;i++ ) {
      v0[i]=m.hline[i].trans_mu+m.hline[i].pressure_S*Press;
      alphaD[i]=v0[i]*sqrt( ( 2*kb*Temp )/( c_sq*atmmass*hitran.GetMolecM(m.hline[i].molec_num*10+m.hline[i].isotp_num) ));
      alphaL[i]=( (1.-q)*m.hline[i].air_B+q*m.hline[i].self_B )*Press*pow( (296/Temp),m.hline[i].temp_air_B );
      //cout << "aD: " << alphaD[i] << " aL: " << alphaL[i] << endl;
    }
    //cout << mu;
    bool end=false;
    for(int mm=0;mm<msize;mm++) {
      double yy=alphaL[mm]/alphaD[mm];
      //cout << "v0: " << v0[mm] << endl;
      for(int i=0;i<mu.size();i++) {
		y[i]+=m.hline[mm].line_I*gfunct(mu[i],v0[mm])*voigt((mu[i]-v0[mm])/alphaD[mm],yy);
      }
    } 
  }
  //for(int i=0;i<mu.size();i++)
  //  cout << setprecision(12) << mu[i] << " " << (q*Press*PathL)/(kb*Temp)*y[i] << endl;
}
void HalIR::runDawsonVoigt()
{
  
  //cout << "Running Dawson Voigt...\n";
  setupRun();
}
/*
SIM_RESULT *HalIR::simulate()
{
    SIM_RESULT *result = new SIM_RESULT;
    double lspace;
    unsigned npoints;
    NRvector<double> v,inten,gfunc,voigtv;
    //NRmatrix<double> ans;
    cout << "Simulate starts\n";
    setparm(mdata->trans_mu[0],mdata->airB[0],mdata->selfB[0],mdata->mass[0],rdata->conc[0],mdata->temp_air[0],mdata->Dpress[0]);
    lspace=0.2*(alphaL+alphaD)*2;
    //npoints=next_p2(int( (abs(rdata->high-rdata->low))/lspace ));
    npoints=131072;
    // adjust lspace.
    lspace=abs(rdata->high-rdata->low)/npoints;
    v.resize(npoints);inten.assign(npoints,0.0);gfunc.assign(npoints,0.0);voigtv.assign(npoints,0.0);
    cout << "lspace:  " << lspace << endl
         << "npoints: " << npoints << endl;
    // fill the v vector 
    for( unsigned i=0;i<npoints;i++ ) 
        v[i]=rdata->high+i*lspace;

    switch(rdata->method) {
    	case 0:
            for( unsigned j=0;j<mdata->trans_mu.size();j++) {
                setparm(mdata->trans_mu[j],mdata->airB[j],mdata->selfB[j],mdata->mass[j],rdata->conc[0],mdata->temp_air[j],mdata->Dpress[j]);
                DawsonVoigt voigt(alphaD,alphaL,v0);
                for( unsigned i=0;i<npoints;i++ ) {
                    inten[i]+=(0.04697/(alphaD))*1.01*rdata->ug[0]*mdata->line_I[j]*voigt(v[i]);
                }
            }
            break;
        case 1:
            MankinFFT(mdata,rdata,v,inten);
            break;
        #ifdef __CUDA__
        case 2: // Cuda DawsonVoight 
        	for( unsigned j=0;j<mdata->trans_mu.size()j++) {
        		setparm(mdata->trans_mu[j],mdata->airB[j],mdata->selfB[j],mdata->mass[j],rdata->conc[0],mdata->temp_air[j],mdata->Dpress[j]);
        	}
        	break;
        #endif // __CUDA__ 
        };
    result->name="TEST";
    result->npts=npoints;
    result->data=new xydata[npoints];
    for( unsigned i=0;i<npoints;i++ ) {
        result->data[i][0]=v[i];
        result->data[i][1]=exp(-inten[i]);
    }
    return result;
}
*/
