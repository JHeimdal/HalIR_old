#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "spectra.hpp"
#include "spcformat.hpp"
class SPC : public Spectra
{
private:
    std::string filename;
    // SPC pointers
    SPCHDR hdr;
    SUBHDR *shd;
    std::fstream iofile;
    float *temp;
public:
    SPC(std::string &pathname);
    SPC(std::string &filepath, double &hlim, double &llim, int &nps, float *data);
    ~SPC() {
        if (shd!=NULL) {
            if  ( sizeof(shd)/sizeof(SUBHDR) > 1 )
	            delete [] shd;
            else if ( sizeof(shd)/sizeof(SUBHDR) == 1 )
                delete shd;
        }
    }
    virtual void write();
    virtual std::string GetName() {
        return filename;
    }
};
