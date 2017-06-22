#include <iostream> 
#include "specrender.h"

BEGIN_EVENT_TABLE(SpecRender, wxGLCanvas)
    EVT_SIZE(SpecRender::OnSize)
    EVT_PAINT(SpecRender::OnPaint)
    EVT_ERASE_BACKGROUND(SpecRender::OnEraseBackground)
    EVT_ENTER_WINDOW( SpecRender::OnEnterWindow )
//    EVT_LEFT_DOWN( SpecRender::OnLDown )
    EVT_MOTION( SpecRender::OnMove )
    EVT_LEFT_UP( SpecRender::OnMLeftUp )
    EVT_LEFT_DOWN( SpecRender::OnMLeftDown )
    EVT_CONTEXT_MENU( SpecRender::OnContextMenu )
    EVT_MENU( ID_AA, SpecRender::OnToggleAA )
    EVT_MENU( ID_AAQUAL, SpecRender::OnToggleAAQuality)
    EVT_MENU( ID_ZOOM_IN, SpecRender::OnZoomIn )
    EVT_MENU( ID_ZOOM_OUT, SpecRender::OnZoomOut )
    EVT_MENU( ID_FIT_ALL, SpecRender::OnZoomAll )
    EVT_MENU( ID_SCALE_Y, SpecRender::OnScaleY )
END_EVENT_TABLE()

SpecRender::SpecRender(wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name, int* gl_attrib, std::vector<Spectra_ptr> *spectras)
    : wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name, gl_attrib )
{
    spectra=spectras;
    zoom_rect=false;
    AA=false;
    AAHigh=false;
    STACK=false;
    offset_x=40;offset_y=30;
    dlimits[0]=8000.0;dlimits[1]= 400.0;dlimits[2]=0.0;dlimits[3]=1.0;
    //dlimits[0]=2000.0;dlimits[1]=1750.0;dlimits[2]=0.0;dlimits[3]=1.0;
    //dlimits[0]=2400; dlimits[1]=2300; dlimits[2]=0.71; dlimits[3]=0.78;
    m_init = false;
}

SpecRender::~SpecRender()
{
}

void SpecRender::Render()
{
    wxPaintDC dc(this);
    // Init OpenGL once, but after SetCurrent
    if (!m_init)
    {
        InitGL();
        m_init = true;
        if (spectra->size()>0)
            ZoomAll();
    }
    if (AA) {
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
    } else {
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POINT_SMOOTH);
    }
    GetClientSize(&cW, &cH);
    glRasterPos2f(0.0f, 0.0f);
    wxPoint mpos = wxGetMousePosition();
    if (STACK) {
      GLint sH=cH/spectra->size();
      glPushMatrix();
      glLoadIdentity();
      glClearColor( 1,1,1, 0 );
      glClear( GL_COLOR_BUFFER_BIT );

      if (UpdateXaxis) { DrawXaxis(); /*glPopMatrix();*/ }
      if (UpdateYaxis) { DrawYaxis(); /*glPopMatrix();*/ }

      pixscale_x=(dlimits[0]-dlimits[1])/(cW-offset_x);
      pixscale_y=(dlimits[3]-dlimits[2])/(sH-offset_y);
      adjpos_x=offset_x*pixscale_x;
      unsigned nspec=spectra->size();
      for (unsigned int i=0;i<nspec;i++)
      {
      if ( (*spectra)[i]->hidden ) continue;
      adjpos_y=(offset_y+(i*sH/nspec))*pixscale_y;
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho( dlimits[0],dlimits[1]-adjpos_x,dlimits[2],dlimits[3]+(i+1)*adjpos_y, 0, 1);
      glMatrixMode(GL_MODELVIEW);

      glViewport(offset_x, (offset_y+i*sH), (GLint) cW, (GLint) sH*(i+1) );
      glPushMatrix();
      glLineWidth(sizes[0]);
      glEnableClientState(GL_VERTEX_ARRAY);
        glColor4fv((*spectra)[i]->color);
        glVertexPointer( 2, GL_FLOAT, 0, (*spectra)[i]->data);
        glDrawArrays( GL_LINE_STRIP, 0, (*spectra)[i]->npts);
      glDisableClientState(GL_VERTEX_ARRAY);
      }
      glPopMatrix();
    }
    else {
      glPushMatrix();
      glLoadIdentity();
      glClearColor( 1,1,1, 0 );
      glClear( GL_COLOR_BUFFER_BIT );


      if (UpdateYaxis) { DrawYaxis(); /*glPopMatrix();*/ }
      if (UpdateXaxis) { DrawXaxis(); /*glPopMatrix();*/ }

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      pixscale_x=(dlimits[0]-dlimits[1])/(cW-offset_x);
      pixscale_y=(dlimits[3]-dlimits[2])/(cH-offset_y);
      adjpos_x=offset_x*pixscale_x;
      adjpos_y=offset_y*pixscale_y;
      glOrtho( dlimits[0],dlimits[1]-adjpos_x,dlimits[2],dlimits[3]+adjpos_y, 0, 1);
      glMatrixMode(GL_MODELVIEW);

      glViewport(offset_x, offset_y, (GLint) cW, (GLint) cH);
      glPushMatrix();
      glLineWidth(sizes[0]);
      glEnableClientState(GL_VERTEX_ARRAY);
      for (unsigned int i=0;i<spectra->size();i++)
      {
	if ( (*spectra)[i]->hidden ) continue;
        glColor4fv((*spectra)[i]->color);
        glVertexPointer( 2, GL_FLOAT, 0, (*spectra)[i]->data);
        glDrawArrays( GL_LINE_STRIP, 0, (*spectra)[i]->npts);
      }
      glDisableClientState(GL_VERTEX_ARRAY);
      glPopMatrix();
    }
    glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
    glGetIntegerv(GL_VIEWPORT,viewport);
    if (zoom_rect) {
	glColor4f(0.0f,0.0f,0.5f,0.3f);
	//glRectd( startpos[0],1.0-startpos[1],currpos[0],1.0-currpos[1] );
	glRectd( currpos[0],currpos[1],startpos[0],startpos[1] );
    }

    glFlush();
    SwapBuffers();
}

void SpecRender::OnEnterWindow( wxMouseEvent& WXUNUSED(event) )
{
    SetFocus();
}
void SpecRender::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    SetCurrent();
    Render();
    SwapBuffers();
}

void SpecRender::OnSize(wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize(event);

	// set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	GetClientSize(&cW, &cH);

	if (GetContext())
	{
	SetCurrent();
	glViewport(offset_x, offset_y, (GLint) cW, (GLint) cH);
	}
	pixscale_x=(dlimits[0]-dlimits[1])/(cW-offset_x);
	pixscale_y=(dlimits[3]-dlimits[2])/(cH-offset_y);
	adjpos_x=offset_x*pixscale_x;
	adjpos_y=offset_y*pixscale_y;
}

void SpecRender::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
// Do nothing, to avoid flashing.
}

void SpecRender::InitGL()
{
	SetCurrent();
	GetClientSize(&cW, &cH);
	glGetFloatv(GL_LINE_WIDTH_RANGE,sizes);
	glGetFloatv(GL_LINE_WIDTH_GRANULARITY,&step);
	// set viewing projection 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	pixscale_x=(dlimits[0]-dlimits[1])/(cW-offset_x);
	pixscale_y=(dlimits[3]-dlimits[2])/(cH-offset_y);
	adjpos_x=offset_x*pixscale_x;
	adjpos_y=offset_y*pixscale_y;
	glOrtho( dlimits[0],dlimits[1]-adjpos_x,dlimits[2],dlimits[3]+adjpos_y, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	UpdateXaxis=true;
	UpdateYaxis=true;
	our_font.init("Test.ttf", 9);
}
void SpecRender::DrawXaxis()
{
	glLoadIdentity();
	// Draw x-Axes
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0, (GLint) cW, 0, offset_y, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, (GLint) cW, offset_y );
	glLineWidth((sizes[0]+sizes[1]/5));
	glBegin(GL_LINES);
	glColor3f(0.0f,0.0f,0.0f);
	glVertex2f( offset_x, offset_y ); glVertex2f( cW, offset_y );
	// Draw tics
	glEnd();
	glLineWidth((sizes[0]));
	glBegin(GL_LINES);
	for ( int i=0;i<20;i++ ) {
	glVertex2f((GLint) (cW*(i/20.)+40), offset_y); glVertex2f((GLint) (cW*(i/20.)+40),offset_y-6); 
	}
	glEnd();

	std::string form="%4.0f";
	float stepsize = (dlimits[0]-dlimits[1])/19.;
	for ( int i=0;i<20;i++ ) {
	freetype::print(our_font, (cW*(i/20.)+20), 10, form.c_str(), dlimits[0]-i*stepsize);
	}
}
void SpecRender::DrawYaxis()
{
	glLoadIdentity();
	// Draw y-Axes
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0, offset_x, 0, (GLint) cH, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, offset_x, (GLint) cH );
	glLineWidth((sizes[0]));
	glBegin(GL_LINES);
	glColor3f(0.0f,0.0f,0.0f);
	glVertex2f( offset_x, offset_y ); glVertex2f( offset_x, cH );
	// Draw tics
	glEnd();
	glLineWidth((sizes[0]));
	glBegin(GL_LINES);
	for ( int i=0;i<20;i++ ) {
	glVertex2f( offset_x, (GLint) (cH*(i/20.)+30)); glVertex2f(offset_x-6,(GLint) (cH*(i/20.)+30)); 
	}
	glEnd();
	std::string form="%1.2f";
	float stepsize = (dlimits[3]-dlimits[2])/19.;
	for ( int i=0;i<20;i++ ) {
	freetype::print(our_font, 1,cH*(i/20.)+26 , form.c_str(), dlimits[2]+i*stepsize);
	}
}
void SpecRender::OnMove( wxMouseEvent &event )
{
    if ( event.Dragging() && event.LeftIsDown() && ( IR_ZOOM_IN || IR_ZOOM_OUT ) ) {
	zoom_rect=true;
	gluUnProject(event.m_x,cH-event.m_y,0,
		     modelMatrix,projMatrix,viewport,
		     &currpos[0],&currpos[1],&currpos[2]);
	Render();
    }
}
void SpecRender::OnMLeftUp( wxMouseEvent &event )
{
   if (zoom_rect && IR_ZOOM_IN) {
       dlimits[0]= ( startpos[0] < currpos[0] ) ? currpos[0]: startpos[0] ;
       dlimits[1]= ( startpos[0] > currpos[0] ) ? currpos[0]: startpos[0] ;
       dlimits[2]= ( startpos[1] > currpos[1] ) ? currpos[1]: startpos[1] ;
       dlimits[3]= ( startpos[1] < currpos[1] ) ? currpos[1]: startpos[1] ;
       zoom_rect=false;
   } else if (zoom_rect && IR_ZOOM_OUT) {
       dlimits[0] = dlimits[0] + abs(startpos[0] - currpos[0]) ;
       dlimits[1] = dlimits[1] + abs(startpos[0] - currpos[0]) ;
       dlimits[2] = dlimits[2] + abs(startpos[1] - currpos[1]) ;
       dlimits[3] = dlimits[3] + abs(startpos[1] - currpos[1]) ;
       zoom_rect=false;
   }
   Render();
}
void SpecRender::OnMLeftDown( wxMouseEvent &event )
{
   if (IR_ZOOM_IN || IR_ZOOM_OUT) {
	gluUnProject(event.m_x,cH-event.m_y,0,
		     modelMatrix,projMatrix,viewport,
		     &startpos[0],&startpos[1],&startpos[2]);
   }
   event.Skip();
}
void SpecRender::ShowContextMenu(const wxPoint& pos)
{
     wxMenu menu;
 
     menu.Append(ID_ZOOM_IN, _T("Zoom In"));
     menu.Append(ID_ZOOM_OUT, _T("Zoom Out"));
     menu.Append(ID_FIT_ALL, _T("Scale to fit all"));
     menu.Append(ID_SCALE_Y, _T("Scale to y-axis"));
     menu.AppendSeparator();
     menu.Append(ID_GSETTINGS, _T("Graphics Settings"), PlotSettingsMenu());
 
     PopupMenu(&menu, pos.x, pos.y);
}
void SpecRender::OnContextMenu(wxContextMenuEvent& event)
{
    wxPoint point = event.GetPosition();
    // If from keyboard
    if (point.x == -1 && point.y == -1) {
        wxSize size = GetSize();
        point.x = size.x / 2;
        point.y = size.y / 2;
    } else {
        point = ScreenToClient(point);
    }
    ShowContextMenu(point);
}
wxMenu *SpecRender::PlotSettingsMenu()
{
    wxMenu *menu = new wxMenu;
    menu->AppendCheckItem(ID_AA,_T("Antialising On/Off"));
    menu->AppendCheckItem(ID_AAQUAL,_T("Antialising Qualty High/Low"));
    menu->Check(ID_AA, AA);
    menu->Check(ID_AAQUAL, AAHigh);
    return menu;
}
void SpecRender::OnToggleAA(wxCommandEvent& event)
{
   AA = ( event.IsChecked() );
   Render();
}
void SpecRender::OnToggleAAQuality(wxCommandEvent& event)
{
   if( event.IsChecked() ) {
	AAHigh=true;
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
   } else {
	AAHigh=false;
	glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
   }
   Render();
}
void SpecRender::OnZoomIn( wxCommandEvent& WXUNUSED(event))
{
	IR_ZOOM_IN=true;
	IR_ZOOM_OUT=false;
}
void SpecRender::OnZoomOut( wxCommandEvent& WXUNUSED(event))
{
	IR_ZOOM_IN=false;
	IR_ZOOM_OUT=true;
}
void SpecRender::ZoomAll()
{
	float xmin,xmax,ymin,ymax;
	if (spectra->size()==0)
		return;
	else if (spectra->size()==1) {
	dlimits[0]=( (*spectra)[0]->xmin > (*spectra)[0]->xmax ) ? (*spectra)[0]->xmin : (*spectra)[0]->xmax;
	dlimits[1]=( (*spectra)[0]->xmin < (*spectra)[0]->xmax ) ? (*spectra)[0]->xmin : (*spectra)[0]->xmax;
	dlimits[2]=(*spectra)[0]->ymin;
	dlimits[3]=(*spectra)[0]->ymax;
	} else {
	    xmin=( (*spectra)[0]->xmin > (*spectra)[0]->xmax ) ? (*spectra)[0]->xmin : (*spectra)[0]->xmax;
	    xmax=( (*spectra)[0]->xmin < (*spectra)[0]->xmax ) ? (*spectra)[0]->xmin : (*spectra)[0]->xmax;
	    ymin=(*spectra)[0]->ymin;ymax=(*spectra)[0]->ymax;
	    for (unsigned int i=1;i<spectra->size();i++ ) {
		xmin = ( xmin > (*spectra)[i]->xmin ) ? xmin : (*spectra)[i]->xmin;
		xmax = ( xmax < (*spectra)[i]->xmax ) ? xmax : (*spectra)[i]->xmax;
		ymin = ( ymin < (*spectra)[i]->ymin ) ? ymin : (*spectra)[i]->ymin;
		ymax = ( ymax > (*spectra)[i]->ymax ) ? ymax : (*spectra)[i]->ymax;
	    }
	dlimits[0]=xmin;
	dlimits[1]=xmax;
	dlimits[2]=ymin;
	dlimits[3]=ymax;
	}
	Render();
}
void SpecRender::OnScaleY( wxCommandEvent& WXUNUSED(event) )
{
        // UPDATE to only count within display limits !!
	float ymin,ymax,xmin,xmax;
        xmin=dlimits[0];
        xmax=dlimits[1];
        ymin=dlimits[3];
        ymax=dlimits[2];
	if (spectra->size()==0)
		return;
	else if (spectra->size()==1) {
            for ( unsigned i=0;i<(*spectra)[0]->npts;++i ) {
                if ( ( (*spectra)[0]->data[i][0] > xmax ) &&  ( (*spectra)[0]->data[i][0] < xmin ) ) {
		    ymin = ( ymin < (*spectra)[0]->data[i][1] ) ? ymin : (*spectra)[0]->data[i][1];
		    ymax = ( ymax > (*spectra)[0]->data[i][1] ) ? ymax : (*spectra)[0]->data[i][1];
                }
            }
	dlimits[2]=ymin*0.98;
	dlimits[3]=ymax*1.02;
	} else {
	    for (unsigned j=0;j<spectra->size();j++ ) {
                for ( unsigned i=0;i<(*spectra)[j]->npts;++i ) {
                    if ( ( (*spectra)[j]->data[i][0] > xmax ) &&  ( (*spectra)[j]->data[i][0] < xmin ) ) {
		        ymin = ( ymin < (*spectra)[j]->data[i][1] ) ? ymin : (*spectra)[j]->data[i][1];
		        ymax = ( ymax > (*spectra)[j]->data[i][1] ) ? ymax : (*spectra)[j]->data[i][1];
                    }
                }
	    }
	dlimits[2]=ymin*0.98;
	dlimits[3]=ymax*1.02;
	}
	Render();
}
