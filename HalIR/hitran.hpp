#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <string>

struct molparm {
    char name[16];
    int nmolec;
    int nlines;
    double conc;
    float  llim;
    float  hlim;
    int    *molecules;
    int    *molec_num;	 // Molecule Number
    int    *isotp_num;	 // Isotopologue Number
    float  *trans_mu;	 // Transition wavenumber (cm^-1)
    float  *line_I;	 // Line Intensity
    float  *einstein_A;	 // Einstein A-coefficient
    float  *air_B;		 // Air broadened with
    float  *self_B;	 // Self broadened width
    float  *low_state_en;	 // Lower state energy
    float  *temp_air_B;	 // Temperature dependence of air width
    float  *pressure_S;	 // Pressure Shift
    //char   **u_vib_quant;// Upper vibrational quanta
    //char   **l_vib_quant;// Lower vibrational quanta
    //char   **u_loc_quant;// Upper local quanta
    //char   **l_loc_quant;// Lower local quanta
    int    *err_code;	 // Error codes
    int    *ref_code;	 // Reference codes
    //char   **line_mix;	 // Flag for line mixing
    float  *u_stat_w;	 // Upper statistical weight
    float  *l_stat_w;	 // Lower statistical weight
    float  *abundance; // Abundance of line
    float  *molecMass; // Mass of line isotope
};

class Hitran
{
private:
void initMolparm(unsigned size, molparm *mp) {
      mp->molec_num = new int[size];
      mp->isotp_num = new int[size];
      mp->trans_mu = new float[size];
      mp->line_I = new float[size];
      mp->einstein_A = new float[size];
      mp->air_B = new float[size];
      mp->self_B = new float[size];
      mp->low_state_en = new float[size];
      mp->temp_air_B = new float[size];
      mp->pressure_S = new float[size];
      //mp->u_vib_quant = new char[size][16];
      //mp->l_vib_quant = new char[size][16];
      //mp->u_loc_quant = new char[size][16];
      //mp->l_loc_quant = new char[size][16];
      //mp->err_code = new int[size];
      //mp->ref_code = new int [size];
      //mp->line_mix = new char[size][2];
      mp->u_stat_w = new float[size];
      mp->l_stat_w = new float[size];
      mp->abundance = new float[size];
      mp->molecMass = new float[size];
}
inline std::string trims(const std::string &ss);
struct HitranDat {
    std::map<int,std::string> molec_map; // Molecular map int->name
    std::multimap<std::string,int> molec_rmap; // Molec reverse map name-> int
    std::map<int,std::string> abund_map; // Molecular map int->name
    std::multimap<std::string,int> abund_rmap; // Molec reverse map name-> int
    std::vector<int>    molec_num;	// Molecule Number
    std::vector<int>    isotp_num;	// Isotopologue Number
    std::vector<std::string> molec_name;	// Name of molecule
    std::vector<std::string> moliso_name;	// Long name of molecule
    std::map<int,int>  isotp_d_map;	// Molecular dist
    std::map<int,int>  isotp_d_rmap;	// Molecular dist
    std::vector<double> isotp_Abun;	// Abundance
    std::vector<int>    molec_M;	// Molecular mass
    std::map<int,int>  molec_mass; // isotp->mass map
    //std::vector<double> part_func;	// Value of patrition function at 298 K
    //std::vector<int>    num_vib;	// Number of fundamental vibrations (v_n,degen)
    //std::vector<int> vib;		// fundamental vibration
    //std::vector<int> deg;		// degenerecy of vibration
    int mole_num(const std::string& name) {
        //binary_find();
        int ret=1;
        return ret;
    }
} hitdat;

struct HitranHead {
  char molec[6];
  char isoname[12];
  int nisotp;
  float roi_low;
  float roi_high;
  int *molecs;
};

struct HitranLine {
  int    molec_num;	 // Molecule Number
  int    isotp_num;	 // Isotopologue Number
  float  trans_mu;	 // Transition wavenumber (cm^-1)
  float  line_I;	 // Line Intensity
  float  einstein_A;	 // Einstein A-coefficient
  float  air_B;		 // Air broadened with
  float  self_B;	 // Self broadened width
  float  low_state_en;	 // Lower state energy
  float  temp_air_B;	 // Temperature dependence of air width
  float  pressure_S;	 // Pressure Shift
  char   u_vib_quant[16];// Upper vibrational quanta
  char   l_vib_quant[16];// Lower vibrational quanta
  char   u_loc_quant[16];// Upper local quanta
  char   l_loc_quant[16];// Lower local quanta
  char   err_code[6];	 // Error codes
  char   ref_code[12];	 // Reference codes
  char   line_mix[2];	 // Flag for line mixing
  float  u_stat_w;	 // Upper statistical weight
  float  l_stat_w;	 // Lower statistical weight
  float  abundance; // abundace for Isotopologue
  float  molecMass; // Mass for this Isotopologue

  inline bool operator<(const HitranLine &b) {
      return this->trans_mu < b.trans_mu;
  }
  inline bool operator<(const double &b) {
      return this->trans_mu < b;
  }
  friend inline bool operator<(const double &a, const HitranLine &b) {
      return a < b.trans_mu;
  }
  inline bool operator>(const HitranLine &b) {
      return this->trans_mu > b.trans_mu;
  }
  inline bool operator>(const double &b) {
      return this->trans_mu > b;
  }
  friend inline bool operator>(const double &a, const HitranLine &b) {
      return a > b.trans_mu;
  }
  inline bool operator==(const HitranLine &b) {
      return std::tie(this->molec_num,this->isotp_num,this->trans_mu,this->line_I)
          == std::tie(b.molec_num, b.isotp_num, b.trans_mu, b.line_I);
  }
} hl;

std::vector<HitranLine> hitpar;
void writeHitPar(const char *filename);
void readHitPar(const char *filename, std::vector<HitranLine> &data, HitranHead &head);
public:
    Hitran();
    molparm* create_molparm(const int *molecules, const int &nmolec,const double &conc,const double &low,const double &high);
    molparm* create_molparm(const std::string &filename);
    void print(std::vector<HitranLine> &data) {
        for( auto hl : data)
            //printf("%2d%1d%12.6f%10.3E%10.3E%5f%5f%10f%4f%8f%15s%15s%15s%15s%6s%12s%1s%7f%7f\n",hl.molec_num,hl.isotp_num,hl.trans_mu,hl.line_I,hl.einstein_A,hl.air_B,hl.self_B,hl.low_state_en,hl.temp_air_B,hl.pressure_S,&hl.u_vib_quant,&hl.l_vib_quant,hl.u_loc_quant,hl.l_loc_quant,hl.err_code,hl.ref_code,hl.line_mix,hl.u_stat_w,hl.l_stat_w);
            std::cout  << hl.molec_num << hl.isotp_num << " " << std::left << std::fixed << std::setw(12) << std::setprecision(6) << hl.trans_mu << std::scientific << " " << hl.line_I << "\n";
            //std::cout << hl.molec_num << " " << hl.isotp_num << " " << hl.trans_mu << "\n";
    }
    bool save_molparm(const molparm *data);
    /*
    void makeHitPar() {
        writeHitPar();
    }*/
    std::vector<int> GetNisotp(std::string &molec);
    std::vector<int> GetMolecN(std::string &molec);
    int GetMolecN(int isotp);
    int GetMolecM(int molec);
    std::string GetMolecName(const int molec);
};
