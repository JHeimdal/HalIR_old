#pragma once
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <cstdlib>

class Spectra
{
private:
    bool m_init_ok;
    typedef float xydata[2];
    boost::filesystem::path fpath;
    std::stringstream m_sstrm;
protected:
	xydata *data;
    float *xdata;
    float *ydata;
	unsigned npts;
	void SetSize(int dim) {
		if (dim > 0) {
			data = new xydata[dim];
			xdata = new float[dim];
			ydata = new float[dim];
            npts=dim;
		}
		else
			dim=0;
            npts=dim;
	}
    enum specForm { NS, SPCF, JCAMP };
    struct cfile_msg {
         bool isOk;
         std::stringstream msg;
         specForm format;
     } m_msg;
    void check_file(std::string &filepath,cfile_msg &ans,bool guess=true, specForm frm=NS);
	float ymax,ymin,xmax,xmin;
    std::string filename;
    std::string filepath;
    std::string name;
    Spectra(std::string &infile);
public:
    Spectra() { data=NULL;xdata=NULL;ydata=NULL; }
    Spectra(int dim,const xydata* indata);
	Spectra(const Spectra& spec) { };
	Spectra& operator=(const Spectra& spec) {
		SetSize(spec.npts);
		for (int i=0;i<spec.npts;i++) {
			xdata[i]=spec.xdata[i];
			ydata[i]=spec.ydata[i];
		}
		return *this;
	}
	inline Spectra& operator/=(const Spectra& spec) {
        // TODO: m�ste se range
		for (int i=0;i<spec.npts;i++) {
			data[i][1]=data[i][1]/spec.data[i][1];
		}
		return *this;
	}
	virtual ~Spectra() {
        if (data==NULL) {
	        delete [] data;
	        delete [] xdata;
	        delete [] ydata;
        }
	}
    bool isOK() { return m_init_ok; }
    virtual void write(std::string &fname) {}
};
