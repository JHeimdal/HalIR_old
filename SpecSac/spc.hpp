#pragma once 
#include <iostream>
#include <fstream>
#include <string>

#include "spectra.h"
#include "spcformat.h"
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
    ~SPC() {
        if (shd!=NULL) {
            if  ( sizeof(shd)/sizeof(SUBHDR) > 1 )
	            delete [] shd;
            else if ( sizeof(shd)/sizeof(SUBHDR) == 1 )
                delete shd;
        }
    }
    virtual void write(std::string &fname);
};
