#include <boost/algorithm/string.hpp>
#include "spectra.h"

void Spectra::check_file(std::string &filepath,cfile_msg &ans,bool guess, specForm frm)
{
    // Make some checks on the file befor load as well as try to guess 
    // the file format if required.
    using namespace boost::filesystem;
    path p(filepath);
    try {
    if ( exists(p) ) {
        if ( is_regular_file(p) ) {
            if ( guess ){
                // Try to guess file format
                // First check the file extension
                if ( p.has_extension() ) {
                    if ( p.extension()==".spc" or p.extension()==".SPC" )
                        ans.format=SPCF;
                    else if  ( p.extension()==".dx" or p.extension()==".DX"  or p.extension()==".JDX"  or p.extension()==".jdx" )
                        ans.format=JCAMP;
                    else {
                        ans.isOk=false;
                        ans.msg << "Guess failed. File extension " <<  p.extension() << " is not known!";
                        ans.format=frm;
                        return;
                    }
                }
                ans.msg << " ";
                ans.isOk=true;
                return;
            } else {
                if ( frm=NS ) {
                    ans.isOk=false;
                    ans.format=frm;
                    ans.msg << "File type " << p.filename() << " is not supported";
                    return;
                } else { 
                    ans.isOk=true;
                    ans.format=frm;
                    ans.msg << " ";
                    return;
                }
            }
        } else if ( is_directory(p) ) {
            ans.isOk=false;
            ans.msg << p.relative_path() << " is a directory";
            ans.format=frm;
            return;
        } else {
            ans.isOk=false;
            ans.msg <<  p.relative_path() << " exists but is neither a file nor a directory";
            ans.format=frm;
            return;
        }
    } else {
        ans.isOk=false;
        ans.msg << p.relative_path() <<  " does not exist";
        ans.format=frm;
        return;
    }
    } catch( const filesystem_error& ex) {
        ans.isOk=false;
        ans.msg << ex.what();
        ans.format=frm;
        return;
    }
}
Spectra::Spectra(std::string &infile) {
    using namespace boost::filesystem;
    using namespace boost::algorithm;
    // Check that file is ok.
    check_file(infile,m_msg,false);
    if ( !m_msg.isOk ) {
        std::cerr << m_msg.msg.str() << std::endl;
        data=NULL;
        m_init_ok=false;
        return;
    }
    fpath=path(infile);
    m_sstrm << fpath.filename(); 
    m_sstrm >> filename;
    m_sstrm.clear();
    m_sstrm << fpath.relative_path();
    m_sstrm >> filepath;
    m_sstrm.clear();
    m_sstrm << fpath.stem();
    m_sstrm >> name;
    m_sstrm.clear();
    trim_if( filename, is_any_of("\"") );
    trim_if( filepath, is_any_of("\"") );
    trim_if(     name, is_any_of("\"") );
    data=NULL;
    xdata=NULL;
    ydata=NULL;
    m_init_ok=true;
}

Spectra::Spectra(int dim,const xydata* indata) 
{
	ymax=1e-30;
	ymin=1e30;
    npts=dim;
    SetSize(npts);
	for(int i=0;i<npts;++i) {
	    data[i][1]=indata[i][1];
	    data[i][0]=indata[i][0];
	    ymax = ( ymax < data[i][1] ) ? data[i][1]: ymax;
	    ymin = ( ymin > data[i][1] ) ? data[i][1]: ymin;
	}
	xmax = data[npts-1][0];
	xmin = data[0][0];
}
/*
void Spectra::Cut(CutInfo& cut)
{
    int start=0;
    int cc=0;
    bool first=true;
    for( unsigned i=0;i<npts;i++ ) {
        if ( (data[i][0] < cut.xmin  && data[i][0] > cut.xmax ) ) {
            if ( first ) {
               start=i;
               first=false;
            }
            cc++;
        }
    }
    xydata *tmp = new xydata[cc];
    memcpy(tmp,data[start],sizeof(xydata)*cc);
    delete [] data;
    npts=cc;
    SetSize(npts);
    memcpy(data,tmp,sizeof(xydata)*cc);
    delete [] tmp;
    if ( cut.newfile ) {
	hdr.ffirst=data[0][0];
	hdr.flast=data[npts-1][0];
	hdr.fnpts=npts;
	out.open("mysave.spc", ios_base::out | ios_base::binary);
	out.write((char *)&hdr, sizeof(SPCHDR));
	out.write((char *)&shd[0],sizeof(SUBHDR));
	temp = new float[hdr.fnpts];
	for(int i=0;i<npts;++i) 
	    temp[i]=data[i][1];
	out.write((char *)temp, sizeof(float)*hdr.fnpts);
	out.close();
        delete [] temp;
    }
}
*/
