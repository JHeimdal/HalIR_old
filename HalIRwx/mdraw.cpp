#include "mdraw.h"

#define POS(r, c)        wxGBPosition(r,c)
#define SPAN(r, c)       wxGBSpan(r,c)

BEGIN_EVENT_TABLE(CutDiag, wxDialog)
END_EVENT_TABLE()

CutDiag::CutDiag(wxWindow *parent, DisplayStatus *DispStat)
	: wxDialog(parent, wxID_ANY, wxString(_T("Cut Spectra")))
{

    low_freq  = new wxTextCtrl( this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(75,20));
    (*low_freq) << DispStat->xmin;
    high_freq = new wxTextCtrl( this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(75,20));
    (*high_freq) << DispStat->xmax;
    // Build the window
    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxGridBagSizer *topgrid = new wxGridBagSizer();
    topgrid->Add(new wxStaticText(this,-1,_T("Low")),POS(1,1));
    topgrid->Add(new wxStaticText(this,-1,_T("High")),POS(1,2));
    topgrid->Add(low_freq, POS(2,1), SPAN(1,1));
    topgrid->Add(high_freq, POS(2,2), SPAN(1,1));
    mainSizer->Add(topgrid,wxSizerFlags(0).Expand().Border(wxALL,10));
    mainSizer->Add(CreateButtonSizer(wxOK|wxCANCEL),wxSizerFlags(0).Center().Border(wxALL,10) );
    SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
    mainSizer->Fit(this);
};
void CutDiag::GetCut(CutInfo& ans)
{
    ans.xmin = atof( low_freq->GetValue().mb_str());
    ans.xmax = atof( high_freq->GetValue().mb_str());
    ans.newfile=true;
}
    
BEGIN_EVENT_TABLE(TreeCtrl, wxTreeCtrl)
    EVT_TREE_ITEM_MENU(ID_TreeCtrl, TreeCtrl::OnItemMenu)
END_EVENT_TABLE()

TreeCtrl::TreeCtrl(wxWindow* parent, wxWindowID id)
	 : wxTreeCtrl(parent,id,wxDefaultPosition,wxDefaultSize,wxEXPAND|wxTR_EXTENDED|wxTR_MULTIPLE|wxTR_HIDE_ROOT|wxTR_NO_LINES)
{
    rootId = AddRoot(wxT("Empty"),-1,-1, new TreeData(_T("Dummy"),-1));
    Layout();
}

void TreeCtrl::Append(wxString &name,wxColor &color, int num)
{
    // Add a spectra to the list.
    wxTreeItemId itemId = AppendItem(rootId,name ,-1,-1,new TreeData( name, num ) );
    SetItemBold(itemId);
    SetItemTextColour(itemId,color);
}

void TreeCtrl::OnItemMenu(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    TreeData *item = itemId.IsOk() ? (TreeData *)GetItemData(itemId)
                                         : NULL;
    wxPoint clientpt = event.GetPoint();
    wxPoint screenpt = ClientToScreen(clientpt);
    select0=itemId;
    ShowMenu(itemId, clientpt);
    event.Skip();
}

void TreeCtrl::ShowMenu( wxTreeItemId id,const wxPoint& pt )
{
    wxString title;
    if ( id.IsOk() )
    {    
        title << wxT("Menu for ") << GetItemText(id);
    }    
    else 
    {    
        title = wxT("Menu for no particular item");
    }    

    wxMenu menu(title);
    menu.Append(Tree_About, wxT("&About..."));
    menu.AppendSeparator();
    menu.Append(ID_Divide, wxT("&Divide"));
    menu.AppendSeparator();
    menu.Append(ID_ShowHide, wxT("&Show / Hide"));
    menu.Append(ID_CColor, wxT("&Change Color"));
    menu.Append(ID_Delete, wxT("&Delete item"));

    PopupMenu(&menu, pt); 
}
/*
void TreeCtrl::OnDelete( wxCommandEvent& WXUNUSED(event) )
{
    int nitems;
    wxArrayTreeItemIds itemList;
    nitems = GetSelections(itemList);

    if ( nitems > 0 ) {
        for ( int i=0;i<nitems;i++ ) {
            TreeData *item = (TreeData *)GetItemData(itemList[i]);
            Delete(itemList[i]);
	}
    }
}
*/

TreeData::TreeData(const wxString &file, int num)
{
	name=file;
	Id=num;
	color[0]=0.0;
	color[1]=0.0;
	color[2]=0.0;
	color[3]=1.0;
}

BEGIN_EVENT_TABLE(MainDraw, wxSplitterWindow)
    EVT_MENU(ID_Divide, MainDraw::OnDivide)
    EVT_MENU(ID_Delete, MainDraw::OnDelete)
    EVT_MENU(ID_ShowHide, MainDraw::OnHide)
    EVT_MENU(ID_CColor, MainDraw::OnCColor)
END_EVENT_TABLE()	

MainDraw::MainDraw(wxFrame *frame, wxWindow *parent)
	: wxSplitterWindow( parent,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN )
{
    // Fill the colors vector
    ncolors=15;
    colors = new color4fv[ncolors];
    //        RED               GREEN           BLUE               ALPHA
    colors[ 0][0]=1.0; colors[ 0][1]=0.0; colors[ 0][2]=0.0; colors[ 0][3]=1.0; // Light red
    colors[ 1][0]=0.0; colors[ 1][1]=0.0; colors[ 1][2]=1.0; colors[ 1][3]=1.0; // Light blue
    colors[ 2][0]=1.0; colors[ 2][1]=0.0; colors[ 2][2]=1.0; colors[ 2][3]=1.0; // Light magneta
    colors[ 3][0]=0.0; colors[ 3][1]=1.0; colors[ 3][2]=0.0; colors[ 3][3]=1.0; // Light green
    colors[ 4][0]=0.0; colors[ 4][1]=1.0; colors[ 4][2]=1.0; colors[ 4][3]=1.0; // Light cyan
    colors[ 5][0]=0.41;colors[ 5][1]=0.23;colors[ 5][2]=0.08;colors[ 5][3]=1.0; // Brown
    colors[ 6][0]=0.5; colors[ 6][1]=0.5; colors[ 6][2]=0.5; colors[ 6][3]=1.0; // Gray
    colors[ 7][0]=0.5; colors[ 7][1]=0.0; colors[ 7][2]=0.0; colors[ 7][3]=1.0; // Dark red
    colors[ 8][0]=0.0; colors[ 8][1]=0.0; colors[ 8][2]=0.5; colors[ 8][3]=1.0; // Dark blue
    colors[ 9][0]=0.5; colors[ 9][1]=0.0; colors[ 9][2]=0.5; colors[ 9][3]=1.0; // Dark magneta (purple)
    colors[10][0]=0.0; colors[10][1]=0.5; colors[10][2]=0.0; colors[10][3]=1.0; // Dark green
    colors[11][0]=0.0; colors[11][1]=0.5; colors[11][2]=0.5; colors[11][3]=1.0; // Dark cyan
    colors[12][0]=1.0; colors[12][1]=1.0; colors[12][2]=1.0; colors[12][3]=1.0; // Blac
    colors[13][0]=1.0; colors[13][1]=0.5; colors[13][2]=0.0; colors[13][3]=1.0; // Orange
    colors[14][0]=1.0; colors[14][1]=0.0; colors[14][2]=0.5; colors[14][3]=1.0;

    wxPanel *treepanel = new wxPanel(this,wxID_ANY);
    wxPanel *plotpanel = new wxPanel(this,wxID_ANY);
    wxBoxSizer *treebox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *plotbox = new wxBoxSizer(wxVERTICAL);
    p_TreeCtrl = new TreeCtrl(treepanel,ID_TreeCtrl);
    treebox->Add(p_TreeCtrl,-1,wxEXPAND|wxALL);
    treepanel->SetSizer(treebox);
    int gl_attrib[20] = { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
         WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 0, WX_GL_DOUBLEBUFFER, None };
    plotwindow = new SpecRender(plotpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxEXPAND, _T(" "),gl_attrib, &spectras);
    plotbox->Add(plotwindow,-1,wxEXPAND|wxALL);
    plotpanel->SetSizer(plotbox);
    plotwindow->SetCursor(wxCURSOR_CROSS);
    SplitVertically(treepanel, plotpanel,  260);
    SetSize(GetClientSize());
    UpdateSize();
}
void MainDraw::LoadSpec()
{
    unsigned initspec = spectras.size();
    filetrans FT = FileOpen();
    for ( int i=0;i<FT.files.size();i++) {
            wxFileName file(FT.files[i]);
            spectras.push_back( Spectra_ptr( new Spectra( file.GetFullPath().mb_str(),colors[i%ncolors], FT.type )) );
    }
    updatetree();
    if ( initspec == 0)
    	plotwindow->ZoomAll();
    else
    	plotwindow->Render();

}
void MainDraw::CutSpec()
{
    if ( spectras.size() == 0 ) {
        cerr << "No spectra loaded\n";
        return;
    }
    //MainDraw *active = (MainDraw*)notebook->GetPage(notebook->GetSelection());
    CutDiag dlg(this, GetDisplayStatus() );
    CutInfo cutinfo;
    if ( dlg.ShowModal() == wxID_OK ) {
        dlg.GetCut(cutinfo);
    }
    spectras[0]->Cut(cutinfo);
}
void MainDraw::LoadFiles(wxVector<wxString>& files)
{
    if ( files.size() > 0 ) {
        for ( int i=0;i<files.size();i++) {
            wxFileName file(files[i]);
            spectras.push_back( Spectra_ptr( new Spectra( file.GetFullPath().mb_str(),colors[i%ncolors], 0 )) );
        }
        updatetree();
    }
}
void MainDraw::ProcessCmdEvent(wxCommandEvent& event)
{
    switch( event.GetId() )
    {
	case ID_Load:
	    LoadSpec();
	    break;
	case ID_LoadTab:
	    LoadSpec();
	    break;
	case ID_Cut:
	    CutSpec();
	    break;
	case ID_Stack:
	    if (spectras.size()>1)
	    	plotwindow->ToggleStack();
	    break;
	default:
	    break;
    }
}

void MainDraw::updatetree()
{
    wxString name;
    wxColor color;
    p_TreeCtrl->Clear();
    for ( unsigned i=0;i<spectras.size();++i ) {
	    name = wxFileNameFromPath( wxString( spectras[i]->GetName().c_str(), wxConvUTF8 ) );
	    color = wxColor (spectras[i]->color[0]*255, spectras[i]->color[1]*255, spectras[i]->color[2]*255, spectras[i]->color[3]*255 );
	    p_TreeCtrl->Append( name, color, i );
    }
    wxSize size = p_TreeCtrl->GetSize();
    cout << "Size: " << size.GetWidth() << endl;
    //SetSashPosition( size.GetWidth(), true );
}
void MainDraw::OnHide( wxCommandEvent& WXUNUSED(event) )
{
    int nitems;
    wxArrayTreeItemIds itemList;
    nitems = p_TreeCtrl->GetSelections(itemList);
    if ( nitems > 0 ) {
        for ( int i=nitems-1;i>=0;i-- ) {
            TreeData *item = (TreeData *)p_TreeCtrl->GetItemData(itemList[i]);
	    spectras[item->Id]->toggleHidden();
	}
    	plotwindow->Render();
    }
}
void MainDraw::OnDivide( wxCommandEvent& WXUNUSED(event) )
{
    int nitems;
    wxArrayTreeItemIds itemList;
    nitems = p_TreeCtrl->GetSelections(itemList);
    if ( nitems == 2 && (itemList[0]==p_TreeCtrl->select0 || itemList[1]==p_TreeCtrl->select0 ) ) {
        cout << "Divide\n";
        TreeData *item1 = (TreeData *)p_TreeCtrl->GetItemData(itemList[0]);
        TreeData *item2 = (TreeData *)p_TreeCtrl->GetItemData(p_TreeCtrl->select0);
        if (itemList[0]==p_TreeCtrl->select0)
	    (*spectras[item1->Id])/=(*spectras[item2->Id]) ;
        else if (itemList[1]==p_TreeCtrl->select0)
	    (*spectras[item2->Id])/=(*spectras[item1->Id]) ;
        else
            cerr << "Error in Divide\n";
        plotwindow->Render();
    }
}
void MainDraw::OnDelete( wxCommandEvent& WXUNUSED(event) )
{
    int nitems;
    wxArrayTreeItemIds itemList;
    nitems = p_TreeCtrl->GetSelections(itemList);
    if ( nitems > 0 ) {
        for ( int i=nitems-1;i>=0;i-- ) {
            TreeData *item = (TreeData *)p_TreeCtrl->GetItemData(itemList[i]);
	    spectras.erase( spectras.begin()+item->Id );
            p_TreeCtrl->Delete(itemList[i]);
	}
    	plotwindow->Render();
    }
}
void MainDraw::OnCColor( wxCommandEvent& WXUNUSED(event) )
{
    cout << "TEST\n";
}
filetrans MainDraw::FileOpen() 
{
    filetrans ans;
    wxFileDialog dialog
                 (
                    this,
                    _T("Open Files"),
                    wxEmptyString,
                    wxEmptyString,
                    _T("SPC |*.spc;*.SPC|J-DX |*.jdx;*.dx|PXY |*.pxy;*.PXY"),
		    wxFD_OPEN|wxFD_MULTIPLE
                 );

    dialog.CentreOnParent();

    if (dialog.ShowModal() == wxID_OK)
    {
        dialog.GetPaths(ans.files);
	ans.type = dialog.GetFilterIndex();
	return ans;
    }
    else {
	return ans;
    }
}
DisplayStatus* MainDraw::GetDisplayStatus()
{
    DisplayStatus *ans = new DisplayStatus;
    ans->xmin=plotwindow->dlimits[0];
    ans->xmax=plotwindow->dlimits[1];
    return ans;
}
