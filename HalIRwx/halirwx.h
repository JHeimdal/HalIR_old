#pragma once

#include "wx/wxprec.h"

#ifdef __BORLAND__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/wx.h"
/*
  #include "wx/splitter.h"
  #include "wx/filename.h"

  #include "wx/gbsizer.h"
  #include "wx/cmdline.h"
  #include "wx/vector.h"
*/
#endif
#include <vector>
#include "wx/notebook.h"
#include "wx/toolbar.h"
#ifdef __WINDOWS__
    #define USE_XPM_BITMAPS 0
#else
    #define USE_XPM_BITMAPS 1
#endif

#if USE_XPM_BITMAPS && defined(__WXMSW__) && !wxUSE_XPM_IN_MSW
    #error You need to enable XPM support to use XPM bitmaps with toolbar!
#endif // USE_XPM_BITMAPS

#if USE_XPM_BITMAPS
    #include "Bitmaps/new.xpm"
    #include "Bitmaps/open.xpm"
    #include "Bitmaps/save.xpm"
    #include "Bitmaps/print.xpm"
    #include "Bitmaps/sim.xpm"
    #include "Bitmaps/fit.xpm"
    #include "Bitmaps/mfit.xpm"
    #include "Bitmaps/stack.xpm"
    #include "Bitmaps/cut.xpm"
#endif // USE_XPM_BITMAPS
enum Positions
{
    TOOLBAR_LEFT,
    TOOLBAR_TOP,
    TOOLBAR_RIGHT,
    TOOLBAR_BOTTOM
};
class wxHalIR : public wxApp
{
public:
  virtual bool OnInit();
  /*
  virtual void OnInitCmdLine(wxCmdLineParser& parser);
  virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
  */
  wxVector<wxString> LoadFiles;
private:
  bool load_file;
};
/*
static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
    { wxCMD_LINE_SWITCH, "h","help","Display CLI help",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_OPTION, "f","file","Load file",
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_NONE }
};
*/
DECLARE_APP(wxHalIR)

class MyFrame : public wxFrame
{
public:
  MyFrame(const wxString& title,wxVector<wxString>& LoadFiles,const wxPoint& pos,const wxSize& size,long style);

  void PopulateToolbar(wxToolBarBase* toolBar);
  void RecreateToolbar();

  void OnToggleToolbar(wxCommandEvent& event);
  void OnToggleToolbarSize(wxCommandEvent& event);
  void OnToolbarStyle(wxCommandEvent& event);
  void OnChangeOrientation(wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnLoad(wxCommandEvent& event);
  void OnLoadTab(wxCommandEvent& event);
  void OnNewTab(wxCommandEvent& event);
  void OnSize(wxSizeEvent& event);
  void OnMove( wxMouseEvent &event );
  void OnStack( wxCommandEvent& event );
  void OnSimulate( wxCommandEvent& event );
  void OnCut( wxCommandEvent& event );

private:
  wxNotebook *notebook;
  Positions   m_toolbarPosition;
  wxToolBar  *m_tbar;
    
  void LayoutChildren();

  bool        m_smallToolbar;
  size_t      m_rows;             // 1 or 2 only

  DECLARE_EVENT_TABLE()
  DECLARE_NO_COPY_CLASS(MyFrame)
};

const int ID_TOOLBAR = 500;

static const long TOOLBAR_STYLE = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;

enum
{
    IDM_TOOLBAR_TOGGLETOOLBARSIZE = 200,
    IDM_TOOLBAR_TOGGLETOOLBARROWS,
    IDM_TOOLBAR_TOGGLETOOLTIPS,
    IDM_TOOLBAR_TOGGLE_TOOLBAR,
    IDM_TOOLBAR_TOGGLE_HORIZONTAL_TEXT,
    IDM_TOOLBAR_CHANGE_TOOLTIP,
    IDM_TOOLBAR_SHOW_TEXT,
    IDM_TOOLBAR_SHOW_ICONS,
    IDM_TOOLBAR_SHOW_BOTH,
    IDM_TOOLBAR_CUSTOM_PATH,
    IDM_TOOLBAR_TOP_ORIENTATION,
    IDM_TOOLBAR_LEFT_ORIENTATION,
    IDM_TOOLBAR_BOTTOM_ORIENTATION,
    IDM_TOOLBAR_RIGHT_ORIENTATION,
    IDM_TOOLBAR_OTHER_1,
    IDM_TOOLBAR_OTHER_2,
    IDM_TOOLBAR_OTHER_3,
};

