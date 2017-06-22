///////////////////////////////////////////////
// Name:    HalIR wx starting point
// Author:  Jimmy Heimdal
///////////////////////////////////////////////
//
#include <iostream>
#include "halirwx.h"
#include "mdraw.h"

#define POS(r, c)        wxGBPosition(r,c)
#define SPAN(r, c)       wxGBSpan(r,c)

#ifndef wxHAS_IMAGES_IN_RESOURCES
  #include "Bitmaps/halir.xpm"
#endif
/*
enum
{
	ID_Quit = 1,
	ID_Load,
	ID_LoadTab,
	ID_NewTab,
	ID_Check,
	ID_About,
	ID_Sim,
	ID_Fit,
	ID_MFit,
	ID_Cut,
	ID_Stack,
	ID_TreeCtrl=1000
};
*/
// Main Frame
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_SIZE(MyFrame::OnSize)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_Load, MyFrame::OnLoad)
    EVT_MENU(ID_LoadTab, MyFrame::OnLoadTab)
    EVT_MENU(ID_NewTab, MyFrame::OnNewTab)
    EVT_MENU(ID_Stack, MyFrame::OnStack)
//    EVT_MENU(ID_Sim, MyFrame::OnSimulate)
    EVT_MENU(ID_Cut, MyFrame::OnCut)
    EVT_MENU(IDM_TOOLBAR_TOGGLE_TOOLBAR, MyFrame::OnToggleToolbar)
    EVT_MENU_RANGE(IDM_TOOLBAR_TOP_ORIENTATION, IDM_TOOLBAR_RIGHT_ORIENTATION, MyFrame::OnChangeOrientation)
    EVT_MOTION( MyFrame::OnMove )
END_EVENT_TABLE()

IMPLEMENT_APP(wxHalIR)

bool wxHalIR::OnInit()
{
        if (!wxApp::OnInit())
            return false;

        long style = wxDEFAULT_FRAME_STYLE|wxCLIP_CHILDREN|wxNO_FULL_REPAINT_ON_RESIZE;
	MyFrame *frame = new MyFrame( _T("HalIR"), LoadFiles, wxPoint(0,0), wxSize(1224,800), style );
        frame->Center();
	frame->Show(true);
        wxInitAllImageHandlers();
	SetTopWindow( frame );
	return true;
}
/*
void wxHalIR::OnInitCmdLine(wxCmdLineParser& parser)
{
    wxApp::OnInitCmdLine(parser);
    parser.AddParam("input files",
            wxCMD_LINE_VAL_STRING,
            wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_PARAM_MULTIPLE);

    //parser.SetDesc( g_cmdLineDesc );
    //parser.SetSwitchChars(wxT("-"));
}
bool wxHalIR::OnCmdLineParsed(wxCmdLineParser& parser)
{
   if ( !wxApp::OnCmdLineParsed(parser) )
       return false;
   for ( size_t paramNr=0;paramNr < parser.GetParamCount(); ++paramNr )
       LoadFiles.push_back(parser.GetParam(paramNr));

   return true;
}
*/
MyFrame::MyFrame(const wxString& title, wxVector<wxString>& LoadFiles, const wxPoint& pos, const wxSize& size,long style)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size, style)
{
    // Set application icon
    SetIcon(wxICON(halir));
    // Generate the menu
    wxMenu *menuFile = new wxMenu;

    menuFile->Append( ID_Load, _T("&Load"), _T("Load File"));
    menuFile->Append( ID_LoadTab, _T("Load in &New Tab"), _T("Load File into a new tab"));
    menuFile->Append( ID_NewTab, _T("&New Tab"), _T("Open a new tab"));
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, _T("E&xit"), _T("Exit Program") );

    wxMenu *viewMenu = new wxMenu;
    viewMenu->AppendSeparator();
    viewMenu->AppendRadioItem(IDM_TOOLBAR_TOP_ORIENTATION,
                              _T("Set toolbar at the top of the window"),
                              _T("Set toolbar at the top of the window"));
    viewMenu->AppendRadioItem(IDM_TOOLBAR_LEFT_ORIENTATION,
                              _T("Set toolbar at the left of the window"),
                              _T("Set toolbar at the left of the window"));
    viewMenu->AppendRadioItem(IDM_TOOLBAR_BOTTOM_ORIENTATION,
                              _T("Set toolbar at the bottom of the window"),
                              _T("Set toolbar at the bottom of the window"));
    viewMenu->AppendRadioItem(IDM_TOOLBAR_RIGHT_ORIENTATION,
                              _T("Set toolbar at the right edge of the window"),
                              _T("Set toolbar at the right edge of the window"));
    viewMenu->AppendSeparator();
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _T("&File") );
    menuBar->Append( viewMenu, _T("&View") );

    SetMenuBar( menuBar );
    
    // Generate Toolbar
    m_smallToolbar = true;
    m_rows = 1;
    m_toolbarPosition = TOOLBAR_TOP;
    RecreateToolbar();


    notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP|wxEXPAND,wxT("No file"));
    //mainpages.push_back( MainDrawPtr(new MainDraw(this, notebook)) ); 
    
    notebook->AddPage(new MainDraw(this, notebook),_T("Main"),true,-1 );
    if ( !LoadFiles.empty() ) {
        MainDraw *active = (MainDraw*)notebook->GetPage(notebook->GetSelection());
        active->LoadFiles(LoadFiles);
    }
    CreateStatusBar(2);
    SetStatusText( _T("Welcome to HalIR"),0 );
    SetStatusText( _T("Welcome to HalIR"),1 );
}
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close( true );
}

void MyFrame::OnLoad(wxCommandEvent& event)
{
  std::cout << "OnLoad\n";
/*
	MainDraw *active = (MainDraw*)notebook->GetPage(notebook->GetSelection());
	active->ProcessCmdEvent(event);
*/
}
void MyFrame::OnLoadTab(wxCommandEvent& event)
{
  std::cout << "OnLoadTab\n";
  /*
        notebook->AddPage(new MainDraw(this, notebook),_T("Main"),true,-1 );
	MainDraw *active = (MainDraw*)notebook->GetPage(notebook->GetSelection());
	active->ProcessCmdEvent(event);
  */
}
void MyFrame::OnNewTab(wxCommandEvent& WXUNUSED(event))
{
  std::cout << "OnNewTab\n";
  /*
	    wxString msg;
	    msg = _T("New tab opened!");
	    SetStatusText( msg,0 );
            notebook->AddPage(new MainDraw(this, notebook),_T("Main"),true,-1 );
  */
}
void MyFrame::RecreateToolbar()
{
    // delete and recreate the toolbar
    wxToolBarBase *toolBar = GetToolBar();
    long style = toolBar ? toolBar->GetWindowStyle() : TOOLBAR_STYLE;

    delete toolBar;

    SetToolBar(NULL);

    style &= ~(wxTB_HORIZONTAL | wxTB_VERTICAL | wxTB_BOTTOM | wxTB_RIGHT | wxTB_HORZ_LAYOUT);
    switch( m_toolbarPosition )
    {
        case TOOLBAR_LEFT:
            style |= wxTB_LEFT;
            break;
        case TOOLBAR_TOP:
            style |= wxTB_TOP;
            break;
        case TOOLBAR_RIGHT:
            style |= wxTB_RIGHT;
            break;
        case TOOLBAR_BOTTOM:
        style |= wxTB_BOTTOM;
        break;
    }
    toolBar = CreateToolBar(style, ID_TOOLBAR);

    PopulateToolbar(toolBar);
}
void MyFrame::PopulateToolbar(wxToolBarBase* toolBar)
{
    // Set up toolbar
    enum
    {
        Tool_new,
        Tool_open,
        Tool_save,
        Tool_print,
        Tool_sim,
        Tool_fit,
        Tool_mfit,
        Tool_stack,
        Tool_cut,
        Tool_Max
    };

    wxBitmap toolBarBitmaps[Tool_Max];

#if USE_XPM_BITMAPS
    #define INIT_TOOL_BMP(bmp) \
        toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
#else // !USE_XPM_BITMAPS
    #define INIT_TOOL_BMP(bmp) \
        toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

    INIT_TOOL_BMP(new);
    INIT_TOOL_BMP(open);
    INIT_TOOL_BMP(save);
    INIT_TOOL_BMP(print);
    INIT_TOOL_BMP(sim);
    INIT_TOOL_BMP(fit);
    INIT_TOOL_BMP(mfit);
    INIT_TOOL_BMP(stack);
    INIT_TOOL_BMP(cut);

    int w = toolBarBitmaps[Tool_new].GetWidth(),
        h = toolBarBitmaps[Tool_new].GetHeight();

    if ( !m_smallToolbar )
    {
        w *= 2;
        h *= 2;

        for ( size_t n = Tool_new; n < WXSIZEOF(toolBarBitmaps); n++ )
        {
            toolBarBitmaps[n] =
                wxBitmap(toolBarBitmaps[n].ConvertToImage().Scale(w, h));
        }
    }

    toolBar->SetToolBitmapSize(wxSize(w, h));

    toolBar->AddTool(ID_Load, _T("Load"),
                     toolBarBitmaps[Tool_new], wxNullBitmap, wxITEM_NORMAL,
                     _T("New file"), _T("This is help for new file tool"));
    toolBar->AddTool(wxID_OPEN, _T("Open"),
                     toolBarBitmaps[Tool_open], wxNullBitmap, wxITEM_NORMAL,
                     _T("Open file"), _T("This is help for open file tool"));
    toolBar->AddTool(wxID_SAVE, _T("Save"), 
		    toolBarBitmaps[Tool_save], wxNullBitmap, wxITEM_NORMAL,
		    _T("Save file"), _T("This is help for save file tool")); 
    toolBar->AddSeparator();
    toolBar->AddTool(ID_Sim, _T("Simulate"), 
		    toolBarBitmaps[Tool_sim], wxNullBitmap, wxITEM_NORMAL,
		    _T("Simulate"), _T("This is help for simulate tool")); 
    toolBar->AddTool(ID_Fit, _T("Fit"), 
		    toolBarBitmaps[Tool_fit], wxNullBitmap, wxITEM_NORMAL,
		    _T("Fit"), _T("This is help for fitting tool")); 
    toolBar->AddTool(ID_MFit, _T("MFit"), 
		    toolBarBitmaps[Tool_mfit], wxNullBitmap, wxITEM_NORMAL,
		    _T("MFit"), _T("This is help for multifitting tool")); 
    toolBar->AddTool(ID_Stack, _T("Stack"), 
		    toolBarBitmaps[Tool_stack], wxNullBitmap, wxITEM_NORMAL,
		    _T("Stack"), _T("Toggle spectra in stacked/unstacked mode")); 
    toolBar->AddTool(ID_Cut, _T("Cut"), 
		    toolBarBitmaps[Tool_cut], wxNullBitmap, wxITEM_NORMAL,
		    _T("Cut"), _T("Cut out part of the spectra")); 
    toolBar->Realize();

    toolBar->SetRows(!(toolBar->IsVertical()) ? m_rows : 10 / m_rows);
}
void MyFrame::LayoutChildren()
{
    wxSize size = GetClientSize();

    int offset;
    if ( m_tbar )
    {
        m_tbar->SetSize(0, 0, wxDefaultCoord, size.y);

        offset = m_tbar->GetSize().x;
    }
    else
    {
        offset = 0;
    }

    SetSize(offset, 0, size.x - offset, size.y);
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    if ( m_tbar )
    {
        LayoutChildren();
    }
    else
    {
        event.Skip();
    }
}

void MyFrame::OnToggleToolbar(wxCommandEvent& WXUNUSED(event))
{
    wxToolBar *tbar = GetToolBar();

    if ( !tbar )
    {
        RecreateToolbar();
    }
    else
    {
        delete tbar;

        SetToolBar(NULL);
    }
}
void MyFrame::OnChangeOrientation(wxCommandEvent& event)
{
    switch( event.GetId() )
    {
        case IDM_TOOLBAR_LEFT_ORIENTATION:
            m_toolbarPosition = TOOLBAR_LEFT;
            break;
        case IDM_TOOLBAR_TOP_ORIENTATION:
            m_toolbarPosition = TOOLBAR_TOP;
            break;
        case IDM_TOOLBAR_RIGHT_ORIENTATION:
            m_toolbarPosition = TOOLBAR_RIGHT;
            break;
        case IDM_TOOLBAR_BOTTOM_ORIENTATION:
            m_toolbarPosition = TOOLBAR_BOTTOM;
            break;
    }
    RecreateToolbar();
}
void MyFrame::OnMove( wxMouseEvent &event )
{
	    wxString msg;
	    msg << event.m_x << _T(",") << event.m_y;
	    SetStatusText( msg,1 );
}
void MyFrame::OnStack( wxCommandEvent& event )
{
    std::cout << "OnStack\n";
    /*
	MainDraw *active = (MainDraw*)notebook->GetPage(notebook->GetSelection());
	active->ProcessCmdEvent(event);
	*/
}
void MyFrame::OnCut( wxCommandEvent& event )
{
    std::cout << "OnCut\n";
    /*
	MainDraw *active = (MainDraw*)notebook->GetPage(notebook->GetSelection());
	active->ProcessCmdEvent(event);
	*/
}
