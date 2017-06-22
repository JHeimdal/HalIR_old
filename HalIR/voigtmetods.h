#ifndef _VOIGHTM_H_
#define _VOIGHTM_H_
#include "nr3.h"
#include "dawson.h"
#include "fourier.h"

static const Doub pi=3.141592653589793;
struct Molparm;
struct RUNPARM;
/**********************************************************************
* routine for calculating the voigt profile, accordin to Drayson S.R *
* J, Quant, Spectrosc, Radiat, Transfer, Vol 16, pp 611-614. 1976    *
* x=ln(2)^1/2*(v-v0)/alphaD, y=ln(2)^1/2*alphaL/alphaD               *
* alphaD: Gaussian half-width                                        *
* alphaL: Lorentzian half-width                                      *
* JH 2013-04-02
**********************************************************************/
struct DawsonVoigt {
        double alphaD;
        double alphaL;
        double v0;
        DawsonVoigt(const double aD,const double aL,const double cl) : alphaD(aD),alphaL(aL),v0(cl) {}
        inline double operator()(const double v) {
        double x,y;
        x=abs(0.832554611*(v-v0)/alphaD);
        y=0.832554611*alphaL/alphaD;
        //x=abs((v-v0)/alphaD);
        //y=alphaL/alphaD;
        //if ( abs(x)>1000 ) return 0.0;
	int region,i,j;
	double y2,V,VV,U,UU,MAX,LOW;
	double RI[]={-0.5,-1.0,-1.5,-2.0,-2.5,-3.0,-3.5,-4.0,-4.5,-5.0,-5.5,-6.0,-6.5,-7.0,-7.5};
	double XN[]={10.,9.,8.,8.,7.,6.,5.,4.,3.,3.,3.,3.,3.,3.,3.};
	double YN[]={.6,.6,.6,.5,.4,.4,.3,.3,.3,.3,1.,.9,.8,.7,.7};
	double XX[]={.5246476,1.65068,.7071068};
	double HH[]={.2562121,.2588268E-1,.2820948};
	double NBY2[]={9.5,9.,8.5,8.,7.5,7.,6.5,6.,5.5,5.,4.5,4.,3.5,3.,2.5,2.,1.5,1.,.5};
	// Determine region
	region=(( (x-5.0) < 0 ) ? ( ((y-1.0) <= 0) ? ( ((x+y) > 5.) ? 3 : 1 ) : ( (x>(1.85*(3.6-y)) ? 3 : 2 ) ) ): 3);
        //cout << "R" << region << " x: " << x << " y: " << y << endl;
	switch(region) {
	  case 1:
            y2=y*y;
	    U=dawson(x);
	    V=1.-2.*x*U;
	    // Taylor expansion around y=0.0
	    VV=exp(y2-x*x)*cos(2.*x*y)/1.128379-y*V;
	    UU=-y;
	    MAX=(5.+(12.5-x)*.8*y);
	    for( int k=1;k<MAX;k=k+2 ) {
		U=(x*V+U)/RI[k];
		V=(x*U+V)/RI[k+1];
		UU=-UU*y2;
		VV=VV+V*UU;
	    }
            //cerr << "x: " << x << " y: " << y << " R1: " << VV << endl;
	    return 1.128379*VV;
	    break;
	  case 2:
	    i=(y>1.45) ? 11.*y : y+y;
	    j=x*x+1.85;
	    MAX=XN[j]*YN[i]+.46;
	    LOW=( ((21-2*MAX)-16) < 0  ? 15 : (20-2*MAX) );
	    UU=y;
	    VV=x;
	    for(int j=LOW;j<19;++j) {
		U=NBY2[j]/(UU*UU+VV*VV);
		UU=y+U*UU;
		VV=x-U*VV;
	    } 
	    return UU/(UU*UU+VV*VV)/1.772454;
	    break;
	  case 3:
            y2=y*y;
            U=x-XX[0];
            V=x+XX[0];
            UU=x-XX[1];
            VV=x+XX[1];
            return y*(HH[0]/(y2+U*U)+HH[0]/(y2+V*V)+HH[1]/(y2+UU*UU)+HH[1]/(y2+VV*VV));
	    break;
	  default:
	    return 0.0;
	}
    }
};

inline Doub sinc(Doub x) {
    if ( x==0 )
        return 1.00;
    else
        return sin(2*pi*x)/(2*pi*x);
}

struct MankinFFT  {
    MankinFFT(Molparm *mparm,RUNDATA *rdata,VecDoub_I &x, VecDoub_IO &ans) {
        const Doub kb=1.3803E-16;
        const Doub atmm=1.6605E-24;
        const Doub c_sq=8.988E20;
        const Doub pi_sq=pi*pi;
        const Doub L0=2.68819E19;
        const Doub  Q=2.15E25;
        Doub lineI,lineP,fval,alphaD,t1,t2,alphal,alphaL,G0,Dv;
        Uint nlip = mparm->line_I.size();
        Int npts = x.size();
        Int cpos;
        Doub highf=rdata->high;
        Doub lowf=rdata->low;
        Doub Temp=rdata->temp;
        Doub Press=rdata->press;
        Doub PathL=rdata->pathl;
        Doub Conc=rdata->conc[0];
        Doub lds=(lowf-highf)/(npts-1);
        Dv=abs(lowf-highf);
        G0=PathL*Q*Press*(296/Temp)*Conc*0.18;
        alphaD=mparm->trans_mu[nlip/2]*sqrt( (2*kb*Temp)/(atmm*mparm->mass[nlip/2]*c_sq) );
        t1=t2=0.0;
        for(unsigned i=0;i<nlip;++i) {
            lineP=mparm->trans_mu[i]+mparm->Dpress[i]*Press;
            lineI=mparm->line_I[i];
            cpos=int((lineP-highf)/lds);
            fval=(lineP-x[cpos])/lds;
            ans[cpos]=(1-fval)*lineI/Dv;ans[cpos+1]=fval*lineI/Dv;
            /* add sinc
            ans[cpos]=sinc(x[cpos])
            for( unsigned j=0;j<=6;++j ) {
                ans[cpos+j]=sinc(x[cpos+j])
                ans[cpos-j]=sinc(x[cpos-j])
            }*/
            alphal=( (1-Conc/Press)*mparm->airB[i]+(Conc/Press)*mparm->selfB[i])*(Press/1)*pow(296/Temp,mparm->temp_air[i]);
            t1+=sqrt(lineI*alphal);
            t2+=sqrt(lineI);
        }
        alphaL=(t1/t2*t1/t2);
        realft(ans,1);
        t1=-1*pi*pi*alphaD*alphaD;
        t2=2*pi*alphaL*Press;
        Dv=1/Dv;
        for(unsigned i=0;i<ans.size();++i) {
            ans[i]=ans[i]*G0*exp(t1*i*i*Dv*Dv-t2*i*Dv);
            //++i;
        }
        realft(ans,-1);
        Doub df=4./ans.size();
        for(unsigned i=0;i<ans.size();++i) 
            ans[i]=df*ans[i];
            
    }
};
#endif
