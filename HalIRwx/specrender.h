#ifndef _SPECRENDER_H_
#define _SPECRENDER_H_
#include "wx/wx.h"
#include <sys/time.h>
#include <sys/unistd.h>
#include <sys/timeb.h>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "wx/glcanvas.h"
#include "spectra.h"
#include "FreeType.h"


//typedef boost::shared_ptr<Spectra> Spectra_ptr;
class SpecRender: public wxGLCanvas
{
    friend class MyFrame;
public:
    SpecRender( wxWindow *parent, wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        long style, const wxString& name, int* gl_attrib,std::vector<Spectra_ptr> *spectras );

    ~SpecRender();

    GLfloat dlimits[4];
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnEnterWindow(wxMouseEvent& event);
    void OnMove(wxMouseEvent& event);
    void OnMLeftUp( wxMouseEvent &event );
    void OnMLeftDown( wxMouseEvent &event );
    void OnContextMenu(wxContextMenuEvent& event);
    void OnToggleAA(wxCommandEvent& event);
    void OnToggleAAQuality(wxCommandEvent& event);
    void OnZoomIn( wxCommandEvent& WXUNUSED(event));
    void OnZoomOut( wxCommandEvent& WXUNUSED(event));
    void OnZoomAll( wxCommandEvent& WXUNUSED(event)) { ZoomAll(); };
    void OnScaleY( wxCommandEvent& WXUNUSED(event));
    void ZoomAll();
    void ToggleStack() { 
	    if (STACK)
	        STACK=false;
	    else
		STACK=true;
	    Render();
    }
    void ShowContextMenu(const wxPoint& pos);
    void Render();
    void InitGL();

private:
    std::vector<Spectra_ptr> *spectra;
    wxMenu *PlotSettingsMenu();
    int cW,cH; // client width and height
    bool   m_init,AA,AAHigh,STACK;
    bool UpdateYaxis;
    bool UpdateXaxis;
    bool IR_ZOOM_IN,IR_ZOOM_OUT,zoom_rect;
    void DrawXaxis();
    void DrawYaxis();
    long start_x,start_y,curr_x,curr_y,mxpos,mypos;
    GLfloat sizes[2];
    GLfloat step,offset_x,offset_y;
    freetype::font_data our_font;
    GLfloat adjpos_x,pixscale_x;
    GLfloat adjpos_y,pixscale_y;
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    int viewport[4];
    GLdouble startpos[3];
    GLdouble currpos[3];
    enum {
        ID_ZOOM_IN =300,
	ID_ZOOM_OUT,
	ID_FIT_ALL,
	ID_SCALE_Y
    };
    enum {
	ID_GSETTINGS=500,
	ID_AA,
	ID_AAQUAL
    };

DECLARE_EVENT_TABLE()
};

#endif
