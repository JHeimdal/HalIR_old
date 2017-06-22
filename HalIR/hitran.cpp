#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <memory>
#include <string>
#include <boost/algorithm/string.hpp>

#include "hitran.h"

using namespace std;
bool mycomp(string& s, string& v) {
	return (s==v);
}
string Hitran::trims(const std::string &ss ) {
    string ans(ss);
    boost::trim(ans);
    return ans;
}
Hitran::Hitran()
{
    // Initiate the hitran object.
    // This means to find the hitran database
    // For now assume that env varible HITRAN
    // exists and points to *.par and *.dat.
    string line,tmp;
    int molec_int;
    stringstream is (stringstream::in | stringstream::out);
    par_path = getenv("HITPAR");
    if ( par_path==NULL ) {
	    cerr << "Environment varible HITPAR is not set\n";
	    exit(99);
    }
    dat_path = getenv("HITDAT");
    if ( dat_path==NULL ) {
	    cerr << "Environment varible HITDAT is not set\n";
	    exit(99);
    }
    ifstream in;
    in.open(dat_path);
    if (!in) cerr << "Could not open file " << dat_path << endl;
    hitdat.it_map = hitdat.molec_map.begin();
    hitdat.it_rmap = hitdat.molec_rmap.begin();
    hitdat.it_a_map = hitdat.abund_map.begin();
    hitdat.it_a_rmap = hitdat.abund_rmap.begin();
    hitdat.it_d_map = hitdat.isotp_d_map.begin();
    hitdat.it_d_rmap = hitdat.isotp_d_rmap.begin();
    hitdat.molec_mass_it = hitdat.molec_mass.begin();
    while( getline(in,line) ) {
        if( line.empty() || line.size() < 10) continue;
	    tmp = line.substr(0,2) + line.substr(3,1);
	    molec_int = atoi(tmp.c_str());
        hitdat.it_map=hitdat.molec_map.insert( hitdat.it_map, pair<int,string>(molec_int,trims(line.substr(5,7))) );
        hitdat.it_rmap=hitdat.molec_rmap.insert( hitdat.it_rmap, pair<string,int>(trims(line.substr(5,7)),molec_int) );
	    hitdat.molec_num.push_back(molec_int);
        hitdat.molec_name.push_back(line.substr(5,7));
        hitdat.moliso_name.push_back(line.substr(13,18));
        hitdat.it_a_map=hitdat.abund_map.insert( hitdat.it_a_map, pair<int,string>(molec_int,trims(line.substr(13,18))) );
        hitdat.it_a_rmap=hitdat.abund_rmap.insert( hitdat.it_a_rmap, pair<string,int>(trims(line.substr(13,18)),molec_int) );
        hitdat.isotp_d_map.insert( hitdat.it_d_map, pair<int,int>(atoi(line.substr(32,5).c_str()),molec_int));
        hitdat.isotp_d_rmap.insert( hitdat.it_d_rmap, pair<int,int>(molec_int,atoi(line.substr(32,5).c_str())));
	    if (line.size() <= 41) {
            hitdat.isotp_Abun.push_back(atof(line.substr(38,2).c_str()));
	        continue;
	    }
        hitdat.isotp_Abun.push_back(atof(line.substr(38,10).c_str()));
        hitdat.molec_M.push_back(atoi(line.substr(48,3).c_str()));
	hitdat.molec_mass_it=hitdat.molec_mass.insert( hitdat.molec_mass_it, pair<int,int>(molec_int,atoi(line.substr(48,3).c_str())));
        hitdat.part_func.push_back(atof(line.substr(52,3).c_str()));
        hitdat.num_vib.push_back(atoi(line.substr(57,2).c_str()));
	    continue;
    }
}
/*
wxString Hitran::GetHitNum(wxString ss)
{
    wxString ans;
    ans << hitdat.molec_rmap.find( string(ss.mb_str()) )->second;
    return ans;
}
wxArrayString Hitran::GetAbundence(wxString ss)
{
    wxArrayString ans;
    wxArrayString tmp;
    multimap<string,int>::iterator it;
    pair<multimap<string,int>::iterator,multimap<string,int>::iterator> ret;
    ret = hitdat.molec_rmap.equal_range( string(ss.mb_str()) );
    for( it=ret.first; it!=ret.second; ++it ) {
    	tmp.Add( wxString( hitdat.abund_map.find( (*it).second )->second.c_str(), wxConvUTF8 ));
    }
    size_t c=tmp.GetCount();
    ans.Add( tmp[c-2] );
    ans.Add( tmp[c-1] );
    for( size_t i=c-2;i>0;i--) {
	    ans.Add( tmp[i-1] );
    }
    return ans;
}
*/
vector<int> Hitran::GetNisotp(std::string &molec) {
    vector<int> ans;
    auto ret=hitdat.molec_rmap.equal_range(molec);
    for( auto it=ret.first; it!=ret.second; ++it )
        ans.push_back(it->second);
    return ans;
}
vector<int> Hitran::GetMolecN(std::string &molec) {
    vector<int> ans;
    auto ret=hitdat.molec_rmap.equal_range(molec);
    for( auto it=ret.first; it!=ret.second; ++it ) {
        ans.push_back(it->second);
    }
    return ans;
}
int Hitran::GetMolecN(const int isotp) {
    return hitdat.isotp_d_map[isotp];
}
int Hitran::GetMolecM(const int molec) {
  return hitdat.molec_mass[molec];
}
Molparm Hitran::create_molparm(const int *molecules,const int *nmolec,const double *conc,const double *low,const double *high)
{
    ifstream in;
    string line;
    bool fline=false;
    double wave_num;
    const int *atom;
    // Init Molparm structure.
    Molparm data;
    data.nmolec=*nmolec;
    data.conc=*conc;
    data.llim=*low;
    data.hlim=*high;
    in.open(par_path);
    if (!in) cerr << "Could not open file " << par_path << endl;
    while( getline(in,line) ) {
        if( line.empty() ) continue;
				wave_num = atof( line.substr(3,12).c_str() );
				atom = find(molecules,molecules+data.nmolec,atoi(line.substr(0,3).c_str()));
				if( ( wave_num >= *high ) && ( wave_num <= *low ) && ( *atom==atoi(line.substr(0,3).c_str()) )) {
					fline=true;
					// 		data->trans_mu.push_back(atof(line.substr(3,12).c_str()));
					// 		data->line_I.push_back(atof(line.substr(15,10).c_str()));
					// 		data->airB.push_back(atof(line.substr(35,5).c_str()));
					// 		data->selfB.push_back(atof(line.substr(40,5).c_str()));
					// 		data->temp_air.push_back(atof(line.substr(55,4).c_str()));
					// 		data->Dpress.push_back(atof(line.substr(59,8).c_str()));
					// 		data->mass.push_back(mol_mass_fnum(atoi(line.substr(0,3).c_str())));
					data.hline.push_back(getHitranLine(line));
				}
				if( wave_num >= *low ) break;
    }
    in.close();
    if ( fline )
      return data;
    else
      data.hline.push_back(getEmptyHitranLine());
      return data;
}
