#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>
#include <boost/filesystem.hpp>


#include "hitran.hpp"
// #include "../SpecSac/specsac.hpp"
namespace bf = boost::filesystem;

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



    void check_file(std::string &filename);

protected:
    Hitran hitran;
    struct Comp {
      std::string molec;
      std::string isotop;
      std::string hpar;
      float vmr;
      molparm *parm;
      unsigned npts;
      double ROI[2];
      float *absCoeff;
      Comp(std::string &m, std::string &isotp, const std::string &path, float &v)
        : molec(m), isotop(isotp), hpar(path), vmr(v) {
          bf::path p(hpar);
          if (!bf::exists(p) && !bf::is_regular_file(p))
            std::cerr << "Something is wrong no file: " << p.string() << std::endl;
      }
    };
    std::vector<molparm*> mparm;
    //Hitran hitran;
    std::vector<Comp*> comp;
    std::vector<std::string> pfiles;
    std::string p_db;
    std::string pdir;
    std::string pcomments;
    std::string bg_file;
    std::string pname;
    std::string ftype;
    std::string apod;

    float   ROI[2];
    float   press;
    float   temp;
    float   res;
    float   fov;
    float   ext;
    float   pathL;
public:
    HalIRSpec();
    HalIRSpec(std::string &infile);
    //HalIRSpec(const HalIRSpec &spec);
    virtual ~HalIRSpec();

    double getPress() {
      return press;
    }
    double getTemp() {
      return temp;
    }
    double getResol() {
      return res;
    }
    double getFov() {
      return fov;
    }
    double getPathl() {
      return pathL;
    }
    void printParm() {
      std::cout << pcomments << std::endl;
    }
};
