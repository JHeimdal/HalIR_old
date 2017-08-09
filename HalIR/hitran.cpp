#include "hitran.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;

string Hitran::trims(const std::string &ss ) {
    string ans(ss);
    boost::trim(ans);
    return ans;
}
Hitran::Hitran()
{
    bool HaveBin=true;
    char *par_path;
    char *dat_path;
    par_path = getenv("HITPAR");
    if ( par_path==NULL ) {
        std::cerr << "Environment variable HITPAR is not set\n";
        exit(99);
    }
    dat_path = getenv("HITDAT");
    if ( dat_path==NULL ) {
        std::cerr << "Environment variable HITDAT is not set\n";
        exit(99);
    }
    // Read HITDAT data
    ifstream in;
    string line,tmp;
    int molec_int;
    in.open(dat_path);
    if (!in) cerr << "Could not open file: " << dat_path << endl;
    while( !getline(in,line).eof() ) {
        if( line.empty() || line.size() < 10) continue;
        tmp = line.substr(0,2) + line.substr(3,1);
        molec_int = atoi(tmp.c_str());
        hitdat.molec_map.emplace(molec_int,trims(line.substr(5,7)));
        hitdat.molec_rmap.emplace(trims(line.substr(5,7)),molec_int);
        hitdat.molec_num.push_back(molec_int);
        hitdat.molec_name.push_back(line.substr(5,7));
        hitdat.moliso_name.push_back(line.substr(13,18));
        hitdat.abund_map.emplace(molec_int,trims(line.substr(13,18)));
        hitdat.abund_rmap.emplace(trims(line.substr(13,18)),molec_int);
        hitdat.isotp_d_map.emplace(atoi(line.substr(32,5).c_str()),molec_int);
        hitdat.isotp_d_rmap.emplace(molec_int,atoi(line.substr(32,5).c_str()));
        if (line.size() <= 41) {
            hitdat.isotp_Abun.push_back(atof(line.substr(38,2).c_str()));
            continue;
        }
        hitdat.isotp_Abun.push_back(atof(line.substr(38,10).c_str()));
        hitdat.molec_M.push_back(atoi(line.substr(48,3).c_str()));
	    hitdat.molec_mass.emplace(molec_int,atoi(line.substr(48,3).c_str()));
        hitdat.part_func.push_back(atof(line.substr(52,3).c_str()));
        hitdat.num_vib.push_back(atoi(line.substr(57,2).c_str()));
	    continue;
    }
    in.close();
    // Read in the HITRAN database
    if (HaveBin) {
        readHitPar(hitpar);
    } else {
    in.open(par_path);
    char ctmp[16];
    do {
        in.get(ctmp,3); hl.molec_num=atoi(ctmp);
        in.get(ctmp,2); hl.isotp_num=atoi(ctmp);
        in.get(ctmp,13); hl.trans_mu=atof(ctmp);
        in.get(ctmp,11); hl.line_I=(float)atof(ctmp);
        in.get(ctmp,11); hl.einstein_A=(float)atof(ctmp);
        in.get(ctmp,6); hl.air_B=(float)atof(ctmp);
        in.get(ctmp,6); hl.self_B=(float)atof(ctmp);
        in.get(ctmp,11); hl.low_state_en=(float)atof(ctmp);
        in.get(ctmp,5); hl.temp_air_B=(float)atof(ctmp);
        in.get(ctmp,9); hl.pressure_S=(float)atof(ctmp);
        in.get(ctmp,16); strcpy(hl.u_vib_quant, ctmp);
        in.get(ctmp,16); strcpy(hl.l_vib_quant, ctmp);
        in.get(ctmp,16); strcpy(hl.u_loc_quant, ctmp);
        in.get(ctmp,16); strcpy(hl.l_loc_quant, ctmp);
        in.get(ctmp,7); hl.err_code=atoi(ctmp);
        in.get(ctmp,13); hl.ref_code=atoi(ctmp);
        in.get(ctmp,2); strcpy(hl.line_mix, ctmp);
        in.get(ctmp,8); hl.u_stat_w=(float)atof(ctmp);
        in.get(ctmp,8); hl.l_stat_w=(float)atof(ctmp);
        in.ignore(8,'\n');
        hitpar.push_back(hl);
    } while( !in.eof() );
    hitpar.pop_back();
    in.close();
}
}
molparm* Hitran::create_molparm(const int *molecules, const int &nmolec,const double &conc,const double &low,const double &high) {
    vector<HitranLine>::iterator down,up;
    vector<HitranLine> dtmp;
    up=upper_bound(hitpar.begin(),hitpar.end(),low);
    down=lower_bound(up,hitpar.end(),high);
    for( auto it=up;it!=down;it++) {
        if( binary_search(molecules,molecules+nmolec,(it->molec_num*10+it->isotp_num) ) )
            dtmp.push_back(*it);
    }
    //size_t hls = sizeof(HitranLine);
    molparm *result = new molparm;
    initMolparm((unsigned)dtmp.size(),result);
    unsigned cc=0;
    for( auto hl : dtmp) {
        //cout << result->molec_num << endl;
        result->molec_num[cc] = hl.molec_num   ;
        result->isotp_num[cc] = hl.isotp_num   ;
        result->trans_mu[cc] = hl.trans_mu    ;
        result->line_I[cc] = hl.line_I      ;
        result->einstein_A[cc] = hl.einstein_A  ;
        result->air_B[cc] = hl.air_B       ;
        result->self_B[cc] = hl.self_B      ;
        result->low_state_en[cc] = hl.low_state_en;
        result->temp_air_B[cc] = hl.temp_air_B  ;
        result->pressure_S[cc] = hl.pressure_S  ;
        result->err_code[cc] = hl.err_code    ;
        result->ref_code[cc] = hl.ref_code    ;
        result->u_stat_w[cc] = hl.u_stat_w    ;
        result->l_stat_w[cc] = hl.l_stat_w    ;
        cc++;
    }
    result->nlines=cc;
    result->nmolec=nmolec;
    result->molecules = new int[nmolec];
    memcpy(result->molecules,molecules,nmolec*sizeof(int));
    memcpy(result->name,GetMolecName(result->molecules[0]).data(),16);
    result->llim=low;
    result->hlim=high;
    result->conc=conc;
    dtmp.clear();
    return result;
}
void Hitran::writeHitPar() {
    ofstream fout("HITRAN08.bpar", ios::binary);
    for( auto hl : hitpar)
        fout.write( (char*)&hl,sizeof(HitranLine));
    fout.close();
}
void Hitran::readHitPar(vector<HitranLine> &data) {
    ifstream fin("HITRAN08.bpar", ios::binary);
    HitranLine thl;
    while( !fin.eof() ) {
        fin.read((char*)&thl,sizeof(HitranLine));
        data.push_back(thl);
    }
    fin.close();
}
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
string Hitran::GetMolecName(const int molec) {
  return hitdat.molec_map[molec];
}
