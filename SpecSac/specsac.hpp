#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "spectra.hpp"

class SpecSac : public Spectra
{
private:
    unsigned nfiles;
    std::vector<Spectra*> spectras;
public:
    SpecSac();
    ~SpecSac() {};
    void add(std::string &filepath);
    //add(std::string &filepath, specForm format);
    Spectra* operator[] (const int idx) { return spectras[idx]; }
};
