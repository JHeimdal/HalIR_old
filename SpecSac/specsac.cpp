#include <iostream>
#include "spc.hpp"
#include "specsac.hpp"

using namespace std;


SpecSac::SpecSac() : Spectra()
{
    nfiles=0;
}

void SpecSac::add(string &filepath)
{
    Spectra::cfile_msg retmsg;
    Spectra::check_file(filepath,retmsg,true);
    if (retmsg.isOk) {
        switch(retmsg.format) {
            case SPCF: spectras.push_back( new SPC(filepath) );
                break;
            case JCAMP: spectras.push_back( new SPC(filepath) );
                break;
            case NS:  cerr << "Format not supported!\n";
                break;
        }
    } else {
        cerr << retmsg.msg.str() << endl;
    }
}
void SpecSac::addSPC(std::string &filepath, double &hlim, double &llim, int &nps, float *data)
{
    cout << "addSPC: " << filepath << endl;
    spectras.push_back( new SPC(filepath,hlim,llim,nps,data) );
}
