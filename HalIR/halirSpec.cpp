#include <boost/algorithm/string.hpp>
#include <algorithm>
#include "halirSpec.h"
//#include "specsac.h"

using namespace std;
HalIRSpec::HalIRSpec() 
{
    simulate=false;
    has_bgfile=false;
    nmol=-1;
    yunit=-1;
    high=-1;
    low=-1;
    press=-1;
    temp=-1;
    resol=-1;
    apod=-1;
    method=-1;
    fov=-1;
    ext=-1;
    pathl=-1;
    set_maps();
}

HalIRSpec::HalIRSpec(std::string &infile)
{
    /********************
     *     Header       *
     * Set some members  *
     ********************/
    simulate=false;
    nmol=-1;
    yunit=-1;
    high=-1;
    low=-1;
    press=-1;
    temp=-1;
    resol=-1;
    apod=-1;
    method=-1;
    fov=-1;
    ext=-1;
    pathl=-1;
    set_maps();
    int ntmp;
    double ctmp;
    int uctmp;
    double ugtmp;
    std::fstream infil;
    infil.open(infile.c_str(),ios_base::in);
    if (!infil.is_open()) {
        std::cerr << "Cant open file\n";
        exit(98);
    }
    infil >> *this;
    // Check the sanity of the input tmolec tconc tuconc and tisotp has to be the same size.
    if ( tmolec.size() != tuconc.size() || tmolec.size() != tisotp.size() ) {
        cerr << ">>> Input error: molec, isotp, conc, and uconc must have the same size <<<\n" << endl;
        cerr << *this;
        exit(98);
    }
    if ( files.size() == 0 )
        simulate=true;
    unsigned tN=tmolec.size();
    //unsigned old=0;
    for( unsigned i=0;i<tN;i++ ) {
        if ( tisotp[i]==0 ) {
            auto mvec=hitran.GetMolecN(tmolec[i]);
            tnmolec.insert( tnmolec.end(),mvec.begin(),mvec.end() );
	    	nmolec.push_back( mvec.size() );
            //tconc.insert( find(tconc.begin(),tconc.end(),tconc[i+old]), mvec.size()-1, tconc[i+old] );
            //tuconc.insert( find(tuconc.begin(),tuconc.end(),tuconc[i+old]), mvec.size()-1, tuconc[i+old] );
            //old+=mvec.size()-1;
        } else {
	    	nmolec.push_back( 1 );
            tnmolec.push_back( hitran.GetMolecN(tisotp[i]) );
        }
    }
    nmol=tnmolec.size();
    // Convert the input to the appropate units //
    press=press_map[upress]*press;
    pathl=path_map[upathl]*pathl;
    if ( utemp.compare("C")==0 )
        temp=C2K(temp);
    else if ( utemp.compare("F")==0 )
        temp=F2K(temp);
    // Calculate mass path
    // Prep the rundata
    yunit = 0;
    resol = 0;
    apod  = 0;
    method= 0;
    fov   = 0;
    
    
    int off=0;int c=0;
    for( int v : nmolec ) {
      int *m = new int[v];
      memcpy(m,tnmolec.data()+off,sizeof(int)*v);
      off+=v;
      double cc = press_map[tuconc[c]]*tconc[c];
      mparm.push_back(hitran.create_molparm(m,&v,&cc,&low,&high));
      delete [] m;
      c++;
    }
    //for( auto v : mparm[0].hline )
    //  std::cout << v.molec_num*10 + v.isotp_num << std::endl;
// // 	  std::cerr << "ERROR\n";
//     }    
}
HalIRSpec::~HalIRSpec()
{
    //delete conc;
    //delete uconc;
    //delete ug;
    //delete molec;
}

void HalIRSpec::set_maps()
{
    conc_map.emplace("ppm",1E-6);
    conc_map.emplace("ppb",1E-9);
    conc_map.emplace("bar",bar2atm);
    conc_map.emplace("mb",mb2atm);
    conc_map.emplace("pa",pa2atm);
    conc_map.emplace("kpa",kpa2atm);
    conc_map.emplace("hpa",hpa2atm);
    conc_map.emplace("torr",torr2atm);
    conc_map.emplace("atm",1.0);
    press_map.emplace("bar",bar2atm);
    press_map.emplace("mb",mb2atm);
    press_map.emplace("pa",pa2atm);
    press_map.emplace("kpa",kpa2atm);
    press_map.emplace("hpa",hpa2atm);
    press_map.emplace("torr",torr2atm);
    press_map.emplace("atm",1.0);
    path_map.emplace("km",km2cm);
    path_map.emplace("m",m2cm);
    path_map.emplace("dm",dm2cm);
    path_map.emplace("cm",1.0);
}
// Output
ostream &operator<< (ostream &os, HalIRSpec &parm)
{
    os << "envi:\t" << parm.temp <<"\t"<< parm.press <<"\t"<< parm.pathl << endl;
    os << "uenvi:\t" << "K\tatm\tcm" << endl;
    os << "molec: ";
    for( int i=0;i<parm.tmolec.size();i++ ) 
        os << parm.tmolec[i] << " ";
    os << "\nnmolec: ";
    for( int i=0;i<parm.tnmolec.size();i++ ) 
        os << parm.tnmolec[i] << " ";
    os << "\n";
    for( int i=0;i<parm.tnmolec.size();i++ )
        os << parm.hitran.GetMolecM(parm.tnmolec[i]) << " ";
    os << "\nisotp: ";
    for( int i=0;i<parm.tisotp.size();i++ ) 
        os << parm.tisotp[i] << " ";
    os << "\nconc: ";
    for( int i=0;i<parm.tconc.size();i++ ) 
        os << parm.tconc[i] << " ";
    os << "\nuconc: ";
    for( int i=0;i<parm.tuconc.size();i++ ) 
        os << parm.tuconc[i] << " ";
    os << endl;
    if ( !parm.simulate ) {
        os << "files:\n";
        for( int i=0;i<parm.files.size();i++ ) 
            os << " " << parm.files[i] << endl;
        os << "end of files" << endl;
    }
    return os;
}
// Input
istream &operator>> (istream &in, HalIRSpec &parm)
{
    string line,key_word,stmp;
    int itmp;
    double dtmp;
    stringstream ss;
    while( getline(in,line) ) {
        if ( line.empty() )
            continue;
        boost::trim(line);
        ss.clear();
        ss.str(line);
        key_word.clear();
        ss >> key_word;
        if ( key_word.substr(0,1).compare("#")==0 ) {
            continue;
        } else if ( key_word.compare("envi:")==0 ) {
            ss >> parm.temp >> parm.press >> parm.pathl;
            continue;
        } else if (  key_word.compare("uenvi:")==0 ) {
            ss >> parm.utemp >> parm.upress >> parm.upathl;
            continue;
        } else if (  key_word.compare("ILS:")==0 ) {
            ss >> parm.resol >> parm.fov;
            continue;
        } else if (  key_word.compare("swin:")==0 ) {
            ss >> parm.high >> parm.low >> parm.ext;
            continue;
        } else if (  key_word.compare("molec:")==0 ) {
            while( !ss.eof() ) {
                ss >> stmp;
                parm.tmolec.push_back(stmp);
            }
            continue;
        } else if (  key_word.compare("title:")==0 ) {
            while( !ss.eof() ) {
                ss >> stmp;
                parm.title+=stmp+" ";
            }
            continue;
        } else if (  key_word.compare("isotp:")==0 ) {
            while( !ss.eof() ) {
                ss >> itmp;
                parm.tisotp.push_back(itmp);
            }
            continue;
        } else if (  key_word.compare("conc:")==0 ) {
            while( !ss.eof() ) {
                ss >> dtmp;
                parm.tconc.push_back(dtmp);
            }
            continue;
        } else if (  key_word.compare("uconc:")==0 ) {
            while( !ss.eof() ) {
                ss >> stmp;
                parm.tuconc.push_back(stmp);
            }
            continue;
        } else if (  key_word.compare("files:")==0 ) {
            if ( ss.eof() ) {
                while( getline(in,line) ) {
                    boost::trim(line);
                    if ( line.compare(0,3,"end")==0) {
                        break;
                    }
                    parm.files.push_back(line);
                }
            } else {
                while( !ss.eof() ) {
                    ss >> stmp;
                    parm.files.push_back(stmp);
                }
            }
            continue;
        } else if (  key_word.compare("bg_file:")==0 ) {
            while( !ss.eof() ) {
                parm.has_bgfile=true;
                ss >> parm.bg_file;
            }
            continue;
        }
    }
    return in;
}


