#ifndef _MDRAW_H_
#define _MDRAW_H_
#include "wx/wx.h"
#include "wx/splitter.h"
#include "wx/treectrl.h"
#include "wx/intl.h"
#include "wx/math.h"
#include "wx/filename.h"
#include "wx/gbsizer.h"
#include "wx/vector.h"
#include <vector>
#include "specrender.h"

struct DisplayStatus {
    double xmin;
    double xmax;
};
//struct CutInfo {
//    double xmin;
//    double xmax;
//    bool newfil;
//};
class CutDiag: public wxDialog
{
public:
    CutDiag(wxWindow *parent, DisplayStatus *DispStat);
    void GetCut(CutInfo& ans);
private:
    wxBoxSizer *mainSizer;
    wxTextCtrl *low_freq;
    wxTextCtrl *high_freq;
DECLARE_EVENT_TABLE()
};
class TreeCtrl: public wxTreeCtrl
{
public:
    enum
    {
        Tree_About,
        Tree_Delete
    };
    TreeCtrl(wxWindow* parent, wxWindowID id );
    ~TreeCtrl(){};
    void Append( wxString &name,wxColor &color, int num );
    void OnItemMenu(wxTreeEvent& event);
    void ShowMenu( wxTreeItemId id,const wxPoint& pt );
    void Clear() { DeleteChildren(rootId); };
    wxTreeItemId select0;
    //void OnDelete(wxCommandEvent& WXUNUSED(event));


private:
    wxTreeItemId rootId;
    int nrecords;



DECLARE_EVENT_TABLE()
};

class TreeData: public wxTreeItemData
{
private:
   wxString name;
public:
   TreeData( const wxString &filen, int num );
   ~TreeData() {};
   float color[4];
   int Id;
};

struct filetrans { 
   wxArrayString files;
   int type;
};
class MainDraw: public wxSplitterWindow 
{
private:
   typedef float color4fv[4];
   color4fv *colors;
   int ncolors;
   vector<color4fv> colorlist;
   typedef boost::shared_ptr<Spectra> Spectra_ptr;
   TreeCtrl *p_TreeCtrl;
   wxBoxSizer *p_BoxSizer;
   SpecRender *plotwindow;
   bool NoSpectra;
   void updatetree();
public:
   MainDraw(wxFrame *frame,wxWindow *parent);
   ~MainDraw(){};
   std::vector<Spectra_ptr> spectras;
   void OnPositionChanged(wxSplitterEvent& event);
   void OnPositionChanging(wxSplitterEvent& event);
   void ProcessCmdEvent(wxCommandEvent& event);
   void LoadSpec();
   void CutSpec();
   void LoadFiles(wxVector<wxString>& files);
   void OnItemMenu(wxTreeEvent& event);
   void ShowMenu( wxTreeItemId id,const wxPoint& pt );
   void OnHide( wxCommandEvent& WXUNUSED(event) );
   void OnCColor( wxCommandEvent& WXUNUSED(event) );
   void OnDivide( wxCommandEvent& WXUNUSED(event) );
   void OnDelete( wxCommandEvent& WXUNUSED(event) );
   void GetColor(color4fv &color,int n) {
       color[0]=colors[n][0];
       color[1]=colors[n][1];
       color[2]=colors[n][2];
       color[3]=colors[n][3];
   }
   void Update() {
       updatetree();
       plotwindow->Render();
   }
   filetrans FileOpen();
   DisplayStatus* GetDisplayStatus();

   DECLARE_EVENT_TABLE()
};

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
	ID_Stack,
        ID_Cut,
	ID_Delete,
	ID_Divide,
        ID_ShowHide,
	ID_CColor,
	ID_TreeCtrl=1000
};
#endif
