#pragma once
#include <fstream>
#include <string>

#include "spcformat.hpp"

void readSPC(std::string &fname, double *xdata, unsigned &npts, float *&ydata) {

  double xmin,xmax;
  // float *data;
  //unsigned npts;
  SPCHDR hdr;
  SUBHDR *shd;
  std::fstream iofile(fname.c_str(), std::ios_base::in | std::ios_base::binary);
  if (iofile.is_open()) {

    iofile.read((char *)&hdr, sizeof(SPCHDR));
    if (hdr.ftflgs==TMULTI) {
      std::cerr << "Multifile Format\n";
      exit(2);
    } else if (hdr.ftflgs==(TMULTI|TORDRD)) {
      std::cerr << "Multifile Format Tordrd set\n";
      exit(2);
    }
    xdata[0]=hdr.ffirst;
    xdata[1]=hdr.flast;
    npts=hdr.fnpts;
    // Read hdr data:
    shd = new SUBHDR[hdr.fnsub];
    iofile.read((char *)shd,sizeof(SUBHDR));

    ydata = new float[npts];
    iofile.read((char*)ydata, sizeof(float)*npts);
    iofile.close();
    //return data;
  }
}

void writeSPC(std::string &fname, double *xdata, unsigned &npts, float *&ydata) {
  std::ofstream iofile(fname.c_str(), std::ios_base::out | std::ios_base::binary);
  if (iofile.is_open()) {
    SPCHDR hdr;
    SUBHDR shd;
    DWORD one = 1;
    hdr.ffirst= xdata[0];
    hdr.flast = xdata[1];
    hdr.fnpts = npts;
    hdr.fnsub = one;
    iofile.write( (char*)&hdr, sizeof(SPCHDR) );
    iofile.write( (char*)&shd, sizeof(SUBHDR) );
    iofile.write( reinterpret_cast<const char*>(ydata), sizeof(float)*hdr.fnpts);
    iofile.close();
  }
  return;
}
