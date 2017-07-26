#ifdef WINDOWS
#include <direct.h>
#define GetCurrDir _getcwd
#else
#include <unistd.h>
#define GetCurrDir getcwd
#endif

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "../HalIR/halirSpec.hpp"
#include "../HalIR/halir.hpp"
#include "../SpecSac/specsac.hpp"

using namespace std;

string GetCurrWDir( void ) {
    char buff[FILENAME_MAX];
    GetCurrDir( buff, FILENAME_MAX);
    string retval(buff);
    return retval;
}

int main(int argc,char **argv)
{
    bool prm_in=false;
    string rootdir = GetCurrWDir();
    vector<string> args( argv+1, argv+argc );
    vector<string> files;
    string parm_in;
    SpecSac spectras;
    if ( argc==1 )
        return 42;
    for( auto arg=args.begin();arg!=args.end();++arg ) {
        /*
        if ( arg->compare(0,3,"-di")==0 ) {
            size_t pos = (++arg)->find("-");
            lval=stod( arg->substr(0,pos) );
            hval=stod( arg->substr(pos+1,arg->npos) );
        } else if (arg->compare(0,2,"-p")==0 ) {
            percent=stod( *++arg )/100;
        */

        if (arg->compare(0,2,"-i")==0 ) {
            if ( ++arg != args.end() ) {
                parm_in=*arg;
                prm_in=true;
                continue;
            } else
                cerr << "Error: command -i needs an argument ( input file )" << endl;
                return 99;
        } else if (arg->compare(0,2,"-h")==0 ) {
            cout << "Usage: " << argv[0] <<  " [commands] [files] \n";
            cout << "Possible Commands\n"
                 << "-h: prints this help.\n"
                 << "-i ifile: input to HalIR.\n"
                 << "-tmpi: print template input to stdout.\n";
            exit(1);
        } else if (arg->compare(0,5,"-tmpi")==0 ) {
            cout << "# This is a examples txt input for HalIR\n\n"
                 << "# The environment section section\n"
                 << "# Temp Pressure pathl\n"
                 << "envi: 298 1000 25\n"
                 << "uenvi: C mb cm\n"
                 << "# ILS Instrument line shape\n"
                 << "# resolution fov apodization\n"
                 << "ILS: 0.01 6.5\n\n"
                 << "# Spectral Window\n"
                 << "# high limit, low limit, ext units in cm^-1\n"
                 << "swin: 2102 2320 0\n"
                 << "# What molecules to fit\n"
                 << "# molec is the name of the molecule\n"
                 << "# isotp is the isotopic composistion 0 for natural abundance otherwise use the 3 digit code from hitran.par (column 5)\n"
                 << "# conc concentration\n"
                 << "# uconc unit on the conectration\n"
                 << "molec:  H2O CO\n"
                 << "isotp:  181  0\n"
                 << "conc:   1.2 2\n"
                 << "uconc:  mb  ppm\n"
                 << "files:\n";
        } else if (arg->compare(0,1,"-")==0 ) {
            cerr << "Error: command: " << *arg << " does not exist" << endl;
            return 99;
        } else {
            files.push_back(*arg);
        }
    }
    if ( prm_in ) {
        //HalIRSpec halir_parm(parm_in);
	    //cout << halir_parm;
        parm_in = rootdir + "/" + parm_in;
        HalIR halir(parm_in);
        cout << halir << endl;
        //spectras.add(files[0]);
        //string fname="co_2.spc";
        //spectras[0]->write(fname);
	    //cout << "halir_init: Fine\n";
	    halir.runDawsonVoigt();
    }
    return 0;
}
