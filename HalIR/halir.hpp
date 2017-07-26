#pragma once
/********************************************************
 * HalIR a program to simulate and fit IR spectra 	*
 * Jimmy Heimdal 2011-07-27				*
 ********************************************************/
#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include "cmath"
#include "halirSpec.hpp"

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
    bool simulate;
    bool verbose;
    // Armadillo init
    arma::fvec alphaL;
    arma::fvec alphaD;
    arma::fvec v0;
    arma::fvec y;
    arma::fvec x;
    arma::fvec mu;
    /* This is function for COMMENT */
    inline double gfunct(double v,double vi) {
        return (v*tanh(hc*v)/(2*kb*Temp))/(vi*tanh((hc*vi)/(2*kb*Temp)));
    }
    /* next_p2 returns the next number that is a power of two */
    inline int next_p2 ( int a )
    {
		    int rval=1;
		    while(rval<a) rval<<=1;
		    return rval;
    }
 public:
    HalIR(const HalIRSpec &spec);
    HalIR(std::string &parm_in);
    void setupRun();
    void runDawsonVoigt();
};
