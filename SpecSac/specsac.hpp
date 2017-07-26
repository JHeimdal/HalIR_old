#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "spectra.hpp"

class SpecSac : public Spectra
{
private:
    unsigned nfiles;
    //std::vector<Spectra*> spectras;
public:
    std::vector<Spectra*> spectras;
    SpecSac();
    ~SpecSac() {};
    void add(std::string &filepath);
    void addSPC(std::string &filepath, double &hlim, double &llim, int &nps, float *data);
    //add(std::string &filepath, specForm format);
    Spectra* operator[] (const int idx) { return spectras[idx]; }
};
