#pragma once
/********************************************************
 * HalIR a program to simulate and fit IR spectra 	*
 * Jimmy Heimdal 2011-07-27				*
 ********************************************************/
#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include "cmath"
#include "halirSpec.h"
//#include "spectra.h"
/*	
struct RUNDATA {
	int *molec;
	int nmolec;
	int yunit;
	double *conc;
	int  *uconc;
    double *ug;
	double high;
	double low;
	double press;
	double temp;
	double resol;
	int    apod;
    int    method;
	double fov;
	double ext;
	double pathl;
	int    upress;
	int    utemp;
	int    upathl;
};
struct SIM_RESULT {
    xydata *data;
    int npts;
    char *name;
};
*/
class HalIR : public HalIRSpec
{
 private:
    //double *conc;
    double Temp;
    double Press;
    double PathL;
    double q;
    const double pi=3.14159265358979;
    const double hc=1.98644521E-16; // erg*cm
    const double c_sq=8.987551787E20; // cm^2*s^-2
    const double kb=1.3806503E-16; // erg/K
    const double atmmass=1.6605E-24; // g
    //static const double hc=1.98644521E-25; // J*m
    //static const double c_sq=8.987551787E16; // m^2*s^-2
    //static const double kb=1.3806503E-23; // J/K
    //static const double atmmass=1.6605E-27; // kg
    const double Na=6.022E23;
    //double alphaL;
    //double alphaD;
    //double v0;
    //inline double voigt(const double v); 
    bool verbose;
    // Armadillo init
    arma::vec alphaL;
    arma::vec alphaD;
    arma::vec v0;
    arma::vec y;
    arma::vec x;
    arma::vec mu;
    inline double gfunct(double v,double vi) {
        return (v*tanh(hc*v)/(2*kb*Temp))/(vi*tanh((hc*vi)/(2*kb*Temp)));
    }
    inline int next_p2 ( int a )
    {
		int rval=1;
		while(rval<a) rval<<=1;
		return rval;
    }   
 public:
    //double y;
    //vector<double> sigma;
    HalIR(const HalIRSpec &spec);
    void setupRun();
    void runDawsonVoigt();
/*  SIM_RESULT *simulate();
    inline void setparm(double vc,double airB,double selfB,double mass,double press_i,double gamma_air,double D_press) {
        v0=vc+D_press*(Press);
        q=press_i/Press;
        alphaD=v0*sqrt( ( 2*kb*Temp )/( atmmass*mass*c_sq ) );
        alphaL=( (1-q)*airB+q*selfB )*(Press)*pow( (296/Temp),gamma_air );
      
        cout << "alphaD: " << alphaD << endl
             << "alphaL: " << alphaL << endl
             << "     q: " << q << endl
             << "    v0: " << v0 << endl
             << "    vc: " << vc << endl;
    }
    */
};
