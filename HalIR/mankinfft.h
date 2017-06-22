#ifndef _MFFT_H_
#define _MFFT_H_

#include "hitran.h"
#include "nr3.h"
#include "fourier.h"
static const Doub pi=3.141592653589793;
static const Doub kb=1.3803E-16;
static const Doub atmm=1.6605E-24;
static const Doub c_sq=8.988E20;
static const Doub pi_sq=pi*pi;
static const Doub L0=2.68819E19;
static const Doub  Q=2.15E25;

struct EnvParm  {
        Doub highf;
        Doub lowf;
        Doub temp;
        Doub press;
        Doub pathl;
        Doub conc;
        Doub lds;
        EnvParm(Doub nn): lds(nn) {
            highf=2600.0; // cm^-1        
            lowf=3150.0; // cm^-1        
            temp=296.0; // K        
            press=1.0; // atm   
            pathl=20; // cm
            conc=0.0002; // atm       
        }
};


inline Doub sinc(Doub x) {
    if ( x==0 )
        return 1.00;
    else
        return sin(2*pi*x)/(2*pi*x);
}

struct MankinFFT  {
    MankinFFT(Molparm *mparm,EnvParm &env,VecDoub_I &x, VecDoub_IO &ans) {
        Doub lineI,lineP,fval,alphaD,t1,t2,alphal,alphaL,G0,Dv;
        Uint nlip = mparm->line_I.size();
        Int npts = x.size();
        Int cpos;
        Doub highf=env.highf;
        Doub lowf=env.lowf;
        Doub Temp=env.temp;
        Doub Press=env.press;
        Doub PathL=env.pathl;
        Doub Conc=env.conc;
        Doub lds=env.lds;
        Dv=abs(lowf-highf);
        G0=PathL*Q*Press*(296/Temp)*Conc*0.3;
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
        Doub df=2./ans.size();
        for(unsigned i=0;i<ans.size();++i) 
            ans[i]=df*ans[i];
            
    }
};

#endif
