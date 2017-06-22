#pragma once
/**********************************************************************
 * routine for calculating the voigt profile, accordin to Drayson S.R *
 * J, Quant, Spectrosc, Radiat, Transfer, Vol 16, pp 611-614. 1976    *
 * x=ln(2)^1/2*(v-v0)/alphaD, y=ln(2)^1/2*alphaL/alphaD               *
 * alphaD: Gaussian half-width                                        *
 * alphaL: Lorentzian half-width                                      *
 **********************************************************************/
#include "nr3.h"
#include "dawson.h"

inline double voigt(const double v,const double y) {
	int region,i,j;
	double x,H=0.201,y2,V,VV,U,UU,MAX,LOW;
	double RI[]={-0.5,-1.0,-1.5,-2.0,-2.5,-3.0,-3.5,-4.0,-4.5,-5.0,-5.5,-6.0,-6.5,-7.0,-7.5};
	double XN[]={10.,9.,8.,8.,7.,6.,5.,4.,3.,3.,3.,3.,3.,3.,3.};
	double YN[]={.6,.6,.6,.5,.4,.4,.3,.3,.3,.3,1.,.9,.8,.7,.7};
	double XX[]={.5246476,1.65068,.7071068};
	double HH[]={.2562121,.2588268E-1,.2820948};
	double NBY2[]={9.5,9.,8.5,8.,7.5,7.,6.5,6.,5.5,5.,4.5,4.,3.5,3.,2.5,2.,1.5,1.,.5};
    double DC[]={8.521437889662E-01, 2.369277586821E-01, 1.831563888873E-02, 3.936690406551E-04, 2.352575200010E-06, 3.908938434265E-09, 1.805831437513E-12, 2.319522830244E-16};
	// Determine region
    x=abs(v);
	region=(( (x-5.0) < 0 ) ? ( ((y-1.0) <= 0) ? ( ((x+y) > 5.) ? 3 : 1 ) : ( (x>(1.85*(3.6-y)) ? 3 : 2 ) ) ): 3);
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
	    return UU/(UU*UU+VV*VV)/1,772454;
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
