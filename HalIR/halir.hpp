#pragma once
/********************************************************
 * HalIR a program to simulate and fit IR spectra 	*
 * Jimmy Heimdal 2011-07-27				*
 ********************************************************/
#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <boost/python.hpp>
#include <boost/python/object.hpp>


#include "tips.hpp"
#include "cmath"
#include "halirSpec.hpp"


namespace bpy = boost::python;

class HalIR : public HalIRSpec
{
 private:
    //double *conc;
    //double Temp;
    //double Press;
    //double PathL;
    //float Resol;
    double q;

    const double ln2=log(2);
    const double sqrt_ln2=sqrt(ln2);
    const double sqrt_2=sqrt(2);
    const double pi=3.14159265358979;
    const double sqrt_pi=sqrt(pi);
    const double sqrt_2pi=sqrt(2*pi);
    const double hc=1.98644521E-16; // erg*cm
    const double c_sq=8.987551787E20; // cm^2*s^-2
    const double c=sqrt(8.987551787E20); // cm/s
    const double kb=1.3806503E-16; // erg/K
    const double atmmass=1.6605E-24; // g
    const double NL=2.686780524E+19; // Loschmid's number cm^-3
    const double c_sqs=8.987551787E16; // m^2*s^-2
    const double kbs=1.3806503E-23; // J/K
    const double Na=6.02214086E23;

    bool simulate;
    bool verbose;
    // Armadillo init
    arma::fvec alphaL;
    arma::fvec alphaD;
    arma::fvec v0;
    arma::fvec y;
    arma::fvec x;
    arma::fvec mu;
    TIPS_2011 tips;
    struct SINC {
        float g;
        float pi = 3.14159265358979;
        SINC(double vg) : g(vg) {}
        float operator() (float &v) {
            if ( v==0 )
                return 2./g;
            else
                return 2./g*sin(2*pi*v/g)/(2*pi*v/g);
        }
    };
    /* This is function for COMMENT */
    inline double gfunct(double vi,double v) {
        return (v*tanh(hc*v)/(2*kb*temp))/(vi*tanh((hc*vi)/(2*kb*temp)));
    }
    inline float gfunct(float vi,float v) {
        return (v*tanh(hc*v)/(2*kb*temp))/(vi*tanh((hc*vi)/(2*kb*temp)));
    }
    inline float qvr(float &line_I,int &molec_num,int &isotp_num,float &low_state_en,float &v0) {
        float qvr = tips(molec_num,isotp_num,temp);
        float qvr_ref=tips(molec_num,isotp_num,296);
        float Top = ( qvr_ref*exp(hc*low_state_en/(kb*temp))*(1-exp(-hc*v0/(kb*temp))));
        float Bot = ( qvr*exp(hc*low_state_en/(kb*296))*(1-exp(-hc*v0/(kb*296))));
        return line_I*(Top/Bot);
    }
    /* next_p2 returns the next number that is a power of two */
    inline int next_p2 ( int a )
    {
		    int rval=1;
		    while(rval<a) rval<<=1;
		    return rval;
    }
    void runDawsonVoigt();
 public:
    HalIR(const HalIRSpec &spec);
    HalIR(std::string parm_in);
    void createCalibrationSpectra();
    void calcSpectra();
    void calcSpectra_cu();
    bpy::list getSpectra();
};
