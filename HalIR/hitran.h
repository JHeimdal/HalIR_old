#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <string.h>
//#include "nr3.h"
//using namespace std;

struct HitranLine {
  int    molec_num;	 // Molecule Number
  int    isotp_num;	 // Isotopologue Number
  double trans_mu;	 // Transition wavenumber (cm^-1)
  double line_I;	 // Line Intensity
  double einstein_A;	 // Einstein A-coefficient
  double air_B;		 // Air broadened with
  double self_B;	 // Self broadened width
  double low_state_en;	 // Lower state energy
  double temp_air_B;	 // Temperature dependence of air width
  double pressure_S;	 // Pressure Shift
  char   u_vib_quant[16];// Upper vibrational quanta
  char   l_vib_quant[16];// Lower vibrational quanta
  char   u_loc_quant[16];// Upper local quanta
  char   l_loc_quant[16];// Lower local quanta
  int    err_code;	 // Error codes
  int    ref_code;	 // Reference codes
  char   line_mix[2];	 // Flag for line mixing
  double u_stat_w;	 // Upper statistical weight
  double l_stat_w;	 // Lower statistical weight
};
struct Molparm {
    int nmolec;
    double conc;
    double llim;
    double hlim;
    std::vector<HitranLine> hline;
};

class Hitran
{
 private:
    template <typename T>
    void vprint(const std::vector<T> &vec) {
        for( T v : vec)
            std::cout << v << " ";
        std::cout << std::endl;
    }
    inline std::string trims(const std::string &ss);
    char *par_path;
    char *dat_path;
    int nentries;

    struct HitranDat {
	    std::map<int,std::string> molec_map; // Molecular map int->name
	    std::map<int,std::string>::iterator it_map; // Molecular map int->name iterator
	    std::multimap<std::string,int> molec_rmap; // Molec reverse map name-> int
	    std::multimap<std::string,int>::iterator it_rmap; // Molec reverse map name-> int revere iterator
	    std::pair<std::multimap<std::string,int>::iterator,std::multimap<std::string,int>::iterator> ret;
	    std::map<int,std::string> abund_map; // Molecular map int->name
	    std::map<int,std::string>::iterator it_a_map; // Molecular map int->name iterator
	    std::multimap<std::string,int> abund_rmap; // Molec reverse map name-> int
	    std::multimap<std::string,int>::iterator it_a_rmap; // Molec reverse map name-> int revere iterator
	    std::vector<int>    molec_num;	// Molecule Number
	    std::vector<int>    isotp_num;	// Isotopologue Number
	    std::vector<std::string> molec_name;	// Name of molecule
	    std::vector<std::string> moliso_name;	// Long name of molecule
        std::map<int,int>  isotp_d_map;	// Molecular dist
	    std::map<int,int>::iterator it_d_map; // Isotp dist iterator
        std::map<int,int>  isotp_d_rmap;	// Molecular dist
	    std::map<int,int>::iterator it_d_rmap; // Isotp dist iterator
	    std::vector<double> isotp_Abun;	// Abundance
	    std::vector<int>    molec_M;	// Molecular mass
	    std::map<int,int>  molec_mass; // isotp->mass map
	    std::map<int,int>::iterator molec_mass_it;
	    std::vector<double> part_func;	// Value of patrition function at 298 K
	    std::vector<int>    num_vib;	// Number of fundamental vibrations (v_n,degen)
	    std::vector<int> vib;		// fundamental vibration
	    std::vector<int> deg;		// degenerecy of vibration
	    int mole_num(const std::string& name) {
		    //binary_find();
		    int ret=1;
		    return ret;
	    }
    };
    HitranDat hitdat;

    std::vector<HitranLine> hitpar;
    /*
    friend ostream& operator<< ( ostream& os,HitranDat& dline ) {
	os << setw(2) << right << dline.molec_num
	   << setw(2) << right << dline.isotp_num
	   << setw(8) << right << dline.molec_name
	   << setw(18)<< right << dline.moliso_name
	   << setw(5) << right << dline.isotp_dist
	   << setw(2) << " "
	   << setw(12)<< left << fixed << setprecision(8) << dline.isotp_Abun << endl;
    }
    */
    HitranLine getHitranLine( std::string& tmp) {
        HitranLine hline;
	      hline.molec_num =    atoi(tmp.substr(0,2).c_str());
        hline.isotp_num =    atoi(tmp.substr(2,1).c_str());
        hline.trans_mu =     atof(tmp.substr(3,12).c_str());
        hline.line_I =       atof(tmp.substr(15,10).c_str());
        hline.einstein_A =   atof(tmp.substr(25,10).c_str());
        hline.air_B =        atof(tmp.substr(35,5).c_str());
        hline.self_B =       atof(tmp.substr(40,5).c_str());
        hline.low_state_en = atof(tmp.substr(45,10).c_str());
        hline.temp_air_B =   atof(tmp.substr(55,4).c_str());
        hline.pressure_S =   atof(tmp.substr(59,8).c_str());
        strcpy(hline.u_vib_quant , tmp.substr(67,15).c_str());
        strcpy(hline.l_vib_quant , tmp.substr(82,15).c_str());
        strcpy(hline.u_loc_quant , tmp.substr(97,15).c_str());
        strcpy(hline.l_loc_quant , tmp.substr(112,15).c_str());
        hline.err_code =     atoi(tmp.substr(127,6).c_str());
        hline.ref_code =     atoi(tmp.substr(133,12).c_str());
        strcpy(hline.line_mix , tmp.substr(145,1).c_str());
        hline.u_stat_w =     atof(tmp.substr(146,7).c_str());
        hline.l_stat_w =     atof(tmp.substr(153,7).c_str());
	      return hline;
    }
    HitranLine getEmptyHitranLine() {
        HitranLine hline;
	      hline.molec_num =    0;
        hline.isotp_num =    0;
        hline.trans_mu =     0.;
        hline.line_I =       0.;
        hline.einstein_A =   0.;
        hline.air_B =        0.;
        hline.self_B =       0.;
        hline.low_state_en = 0.;
        hline.temp_air_B =   0.;
        hline.pressure_S =   0.;
        strcpy(hline.u_vib_quant , " ");
        strcpy(hline.l_vib_quant , " ");
        strcpy(hline.u_loc_quant , " ");
        strcpy(hline.l_loc_quant , " ");
        hline.err_code =     0;
        hline.ref_code =     0;
        strcpy(hline.line_mix , " ");
        hline.u_stat_w =     0.;
        hline.l_stat_w =     0.;
	      return hline;
    }
    int mol_mass_fnum(int num) {
	    std::vector<int>::iterator it;
	    it=find(hitdat.molec_num.begin(),hitdat.molec_num.end(),num);
	    return hitdat.molec_M[int(it-hitdat.molec_num.begin())];
    }

 public:
    Hitran();
    Hitran(bool verbose);
    Molparm create_molparm(const int *molec,const int *nmolec,const double *conc,const double *low,const double *high);
    std::vector<int> GetNisotp(std::string &molec);
    std::vector<int> GetMolecN(std::string &molec);
    int GetMolecN(int isotp);
    int GetMolecM(int molec);
};
