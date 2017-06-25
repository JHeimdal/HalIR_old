#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>

#include "hitran.hpp"
#include "../SpecSac/specsac.hpp"

class HalIRSpec
{
private:
    template <typename T>
    void vprint(const std::vector<T> &vec) {
        for( T v : vec)
            std::cout << v << " ";
        std::cout << std::endl;
    }
    /* Some constants for unit conversion */
    const double m2cm       =1e2;
    const double dm2cm      =1e1;
    const double km2cm      =1e5;
    const double bar2atm    =0.986923266716;
    const double mb2atm     =bar2atm*1e-3;
    const double pa2atm     =bar2atm*1e-5;
    const double kpa2atm    =bar2atm*1e-2;
    const double hpa2atm    =bar2atm*1e-3;
    const double pp2Nden    =1.0; // partial pressure to number density
    const double torr2atm   =0.001315789473684;
    const double kb_si      =1.3806503E-23;
    const double kb_erg     =1.3806503E-16;
    const double c          =299792458;
    const double pi         =3.141592653589793;
    const double pi_sq      =pi*pi;
    const double hc_erg     =1.986445121E-16;
    const double c_sq_erg   =8.987551787E20;
    const double atm2g      =1.6605E-24;
    const double Na         =6.022E23;
    const double c_2        =1.4388; // cmK
    double C2K(double c_val) { return c_val+273.15; } // Converts Celsius to Kelvin
    double F2K(double f_val) { return (f_val-32)*5/9+273.15; } // Converts Fahrenheit to Kelvin ( never put this in documentation )
    /* End of constants */

    /* Data used for simulation     */
    std::string upress;
    std::string utemp;
    std::string upathl;
    std::string bg_file;
    std::string title;
    std::vector<std::string> molec;
    std::vector<std::string> tmolec;
    std::vector<std::string> uconc;
    std::vector<std::string> tuconc;
    std::vector<std::string> files;
    std::vector<int> isotp;
    std::vector<int> tisotp;
    std::vector<int> nmolec;
    std::vector<int> tnmolec;
    std::vector<double> tconc;
    std::vector<double> conc;
    std::vector<double> ug;

    double  high;
    double  low;
    double  press;
    double  temp;
    double  resol;
    int     apod;
    int     method;
    double  fov;
    double  ext;
    double  pathl;
    int     nmol;
    int     yunit;
    /* mebers for cheking and keeping check on input */
    bool simulate;
    bool has_bgfile;
    std::map<std::string,double> conc_map;
    std::map<std::string,double> press_map;
    std::map<std::string,double> path_map;
    void set_maps();
    /* End simulation data*/
protected:
    std::vector<Molparm> mparm;
    Hitran hitran;
    SpecSac spectras;
public:
    HalIRSpec();
    HalIRSpec(std::string &infile);
    //HalIRSpec(const HalIRSpec &spec);
    virtual ~HalIRSpec();
    friend std::ostream &operator<< (std::ostream &os,HalIRSpec &parm);
    friend std::istream &operator>> (std::istream &in,HalIRSpec &parm);
    double getPress() {
      return press;
    }
    double getTemp() {
      return temp;
    }
    double getResol() {
      return resol;
    }
    double getFov() {
      return fov;
    }
    double getPathl() {
      return pathl;
    }
    bool isSimulation() {
      return simulate;
    }
};
