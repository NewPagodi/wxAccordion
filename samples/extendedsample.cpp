/////////////////////////////////////////////////////////////////////////////
// Name:        extendedsample.cpp
// Purpose:     wxAccordion Extended Sample
// Author:      ()
// Created:     2015-10-10
// Copyright:   ()
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/msgdlg.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#endif

#include <wx/accordion/accordion.h>
#include <wx/html/htmlwin.h>
#include <wx/splitter.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include <wx/fontpicker.h>
#include <wx/renderer.h>

#include <stack>

#include "bitmaps/home.xpm"
#include "bitmaps/htmsidep.xpm"
#include "bitmaps/listview.xpm"
#include "bitmaps/repview.xpm"
#include "bitmaps/expand02.xpm"
#include "bitmaps/collapse02.xpm"
#include "bitmaps/expand03.xpm"
#include "bitmaps/collapse03.xpm"
#include "bitmaps/back04.xpm"
#include "bitmaps/back04hl.xpm"
#include "bitmaps/expand06.xpm"
#include "bitmaps/collapse06.xpm"
#include "bitmaps/expand07.xpm"
#include "bitmaps/collapse07.xpm"
#include "bitmaps/expand08.xpm"
#include "bitmaps/collapse08.xpm"
#include "bitmaps/subexpand.xpm"
#include "bitmaps/subcollapse.xpm"
#include "bitmaps/subempty.xpm"

///////////////////////////////////////////////////////////////////////////

#define id_MENUQUIT 1000
#define id_MENUABOUT 1001
#define id_EXPANDBUTTON 1002
#define id_COLLAPSEBUTTON 1003
#define id_TOGGLEBUTTON 1004
#define id_ENABLEBUTTON 1005
#define id_DISABLEBUTTON 1006
#define id_SHOWICONSCHECK 1007
#define id_SHOWBUTTONSCHECK 1008

class ExtendedSampleApp : public wxApp
{
    public:
        virtual bool OnInit();
};

class ExtendedSampleFrame : public wxFrame
{
	public:
		ExtendedSampleFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxAccordion Extended Sample"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxGetDisplaySize().Scale(0.75, 0.75), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~ExtendedSampleFrame();

	private:
	    void OnAbout( wxCommandEvent& ){wxMessageBox( wxT("the wxAccordion extended sample."), wxT("Welcome to...") );}
	    void OnClose( wxCloseEvent& ) { Destroy(); }
        void OnQuit( wxCommandEvent& ){ Destroy(); }

        void onAccordion2Expanding(wxBookCtrlEvent& event);
        void onAccordion2Expanded(wxBookCtrlEvent& event);
        void onAccordion2Collapsing(wxBookCtrlEvent& event);
        void onAccordion2Collapsed(wxBookCtrlEvent& event);
        void onButton(wxCommandEvent& event);

        void onAccordion3MouseLeftUp( wxMouseEvent& event );
        void onAccordion3MouseLeftDC( wxMouseEvent& event );
        void onAccordion3PageMouseLeftUp( wxMouseEvent& event );
        void onChecks(wxCommandEvent& event);

        void onAccordion5Mouse( wxMouseEvent& event );
        void block(wxBookCtrlEvent& event){event.Veto();}

        void onAccordion7MouseLeftUp( wxMouseEvent& event );
        void onAccordion8(wxBookCtrlEvent& event);

        void onSplitterSash( wxSplitterEvent& event );
        void onSplitterPaint( wxPaintEvent& event );

		void addSample1();
		void addSample2();
		void addSample3();
		void addSample4();
		void addSample5();
		void addSample6();
		void addSample7();
		void addSample8();

        wxNotebook* m_notebook;
		wxAccordion* m_accordion1;
		wxSpinCtrl* m_spinCtrl;

		wxTextCtrl* m_log2;
		wxCheckBox* m_page1CheckBox;
		wxCheckBox* m_page2CheckBox;
		wxCheckBox* m_page3CheckBox;

		wxAccordion* m_accordion3;
		wxCheckBox* m_blockMouseCheckBox;
		wxTextCtrl* m_log3;

		wxAccordion* m_accordion5;
		int m_lastFound;

		wxPanel* m_leftPanel8;
		wxHtmlWindow* m_htmlWin8;
};

bool ExtendedSampleApp::OnInit()
{
    ExtendedSampleFrame* frame = new ExtendedSampleFrame(0L);
    frame->SetIcon(wxICON(aaaa));
    frame->Show();

    return true;
}

IMPLEMENT_APP(ExtendedSampleApp);


ExtendedSampleFrame::ExtendedSampleFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxMenuBar* mbar = new wxMenuBar( 0 );
	wxMenu* fileMenu = new wxMenu();
	wxMenu* helpMenu = new wxMenu();
	wxMenuItem* menuFileQuit = new wxMenuItem( fileMenu, id_MENUQUIT, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	wxMenuItem* menuHelpAbout = new wxMenuItem( helpMenu, id_MENUABOUT, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );
	helpMenu->Append( menuHelpAbout );
	mbar->Append( fileMenu, wxT("&File") );
	mbar->Append( helpMenu, wxT("&Help") );
	this->SetMenuBar( mbar );

	wxBoxSizer* bSizer = new wxBoxSizer( wxVERTICAL );
	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
    bSizer->Add( m_notebook, 1, wxEXPAND, 0 );

	this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );

	this->SetSizer( bSizer );
	this->Layout();

	addSample1();
	addSample2();
	addSample3();
	addSample4();
	addSample5();
	addSample6();
	addSample7();
	addSample8();

    this->Bind( wxEVT_COMMAND_MENU_SELECTED, &ExtendedSampleFrame::OnQuit, this, menuFileQuit->GetId() );
	this->Bind( wxEVT_COMMAND_MENU_SELECTED, &ExtendedSampleFrame::OnAbout, this, menuHelpAbout->GetId() );
	this->Bind( wxEVT_CLOSE_WINDOW, &ExtendedSampleFrame::OnClose, this );

	this->Centre( wxBOTH );
}

ExtendedSampleFrame::~ExtendedSampleFrame()
{
}

void setHTMLwin( wxHtmlWindow* htmlWin, const wxString& s, const wxColour& c ,const wxColour& c2 = *wxBLACK)
{
    wxString s2 = "<html><body bgcolor=\"";
    s2 << c.GetAsString();
    s2 << "\">";
    s2 << "<font color=\"";
    s2 << c2.GetAsString();
    s2 << "\">";
    s2 << s;
    s2 << "</font></body></html>";
    htmlWin->SetPage(s2);
}

void paintSash( wxDC* myDC, wxSplitterWindow* splitter, const wxColor& col )
{
    myDC->SetBrush(col);
    myDC->SetPen(*wxTRANSPARENT_PEN);

    myDC->DrawRectangle
    (
        splitter->GetSashPosition(),
        0,
        splitter->GetSashSize(),
        splitter->GetSize().GetHeight()
    );
}

void ExtendedSampleFrame::onSplitterPaint( wxPaintEvent& event )
{
    if( wxSplitterWindow* splitter = wxDynamicCast(event.GetEventObject(),wxSplitterWindow) )
    {
        wxPaintDC myDC(splitter);
        paintSash(&myDC, splitter, splitter->GetWindow1()->GetBackgroundColour() );
    }
}

void ExtendedSampleFrame::onSplitterSash( wxSplitterEvent& event )
{
    if( wxSplitterWindow* splitter = wxDynamicCast(event.GetEventObject(),wxSplitterWindow))
    {
        splitter->Refresh();
    }
}

void ExtendedSampleFrame::addSample1()
{
    wxPanel* panel = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer = new wxBoxSizer( wxVERTICAL );

    wxSplitterWindow* splitter = new wxSplitterWindow( panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    m_accordion1 = new wxAccordion(splitter);
    m_accordion1->AddPage( new wxTextCtrl(m_accordion1,wxID_ANY), "Page 1", true );
    m_accordion1->AddPage( new wxTextCtrl(m_accordion1,wxID_ANY), "Page 2", false );
    m_accordion1->AddPage( new wxTextCtrl(m_accordion1,wxID_ANY), "Page 3", false );
    m_accordion1->AddPage( new wxTextCtrl(m_accordion1,wxID_ANY), "Page 4", false );

    wxHtmlWindow* htmlWin = new wxHtmlWindow( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>This sample shows an unmodified accordion. "
        "You can open or close the pages by clicking on the page’s caption bar.</p>"
        "<p>In this case, each page contains only a single text control, "
        "but any wxWindow can be added to a page.</p>"
        "<p>You can also use the buttons below to expand, collapse, toggle, enable, "
        "or disable any of the pages in the accordion.</p>"
        ;
    setHTMLwin( htmlWin, s, m_accordion1->GetBackgroundColour() );

    splitter->SplitVertically( m_accordion1, htmlWin, GetSize().GetWidth()*.25 );
    bSizer->Add( splitter, 1, wxEXPAND, 5 );

	bSizer->Add( new wxStaticLine(panel), 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	bSizer2->Add( new wxStaticText( panel, wxID_ANY, wxT("Page") ), 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_spinCtrl = new wxSpinCtrl( panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxSP_ARROW_KEYS, 1, 4, 1 );
	bSizer2->Add( m_spinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	wxButton* button1 = new wxButton( panel, id_EXPANDBUTTON, wxT("Expand"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( button1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxButton* button2 = new wxButton( panel, id_COLLAPSEBUTTON, wxT("Collapse"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( button2, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	wxButton* button3 = new wxButton( panel, id_TOGGLEBUTTON, wxT("Toggle"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( button3, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	wxButton* button4 = new wxButton( panel, id_ENABLEBUTTON, wxT("Enable"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( button4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	wxButton* button5 = new wxButton( panel, id_DISABLEBUTTON, wxT("Disable"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( button5, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	bSizer->Add( bSizer2, 0, wxEXPAND, 5 );
    panel->SetBackgroundColour( m_accordion1->GetBackgroundColour() );

    panel->SetSizer( bSizer );
	panel->Layout();
	bSizer->Fit( panel );

    m_notebook->AddPage( panel, wxT("Sample 1"), true );

	button1->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
	button2->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
	button3->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
	button4->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
	button5->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
}

void ExtendedSampleFrame::onButton(wxCommandEvent& event)
{
    int i = m_spinCtrl->GetValue()-1;

    if(event.GetId()==id_EXPANDBUTTON)
    {
        m_accordion1->Expand(i);
    }
    else if(event.GetId()==id_COLLAPSEBUTTON)
    {
        m_accordion1->Collapse(i);
    }
    else if(event.GetId()==id_TOGGLEBUTTON)
    {
        m_accordion1->Toggle(i);
    }
    else if(event.GetId()==id_ENABLEBUTTON)
    {
        m_accordion1->Enable(i);
    }
    else if(event.GetId()==id_DISABLEBUTTON)
    {
        m_accordion1->Disable(i);
    }
}

void ExtendedSampleFrame::addSample2()
{
	wxSplitterWindow* splitter1 = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	splitter1->SetSashGravity( 1 );

	wxSplitterWindow* splitter2 = new wxSplitterWindow( splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    wxAccordion* accordion = new wxAccordion(splitter2);
    accordion->SetAccordionPadding(3);
    accordion->GetCollapsedStyle().SetColour1( wxColour(46,77,123) );
    accordion->GetCollapsedStyle().SetColour2( wxColour(46,77,123) );
    accordion->GetCollapsedStyle().SetBorderColour( wxColour(47,79,79) );
    accordion->GetCollapsedStyle().SetTextColour( wxColour(255,255,255) );
    accordion->GetExpandedStyle().SetColour1( wxColour(80,120,179) );
    accordion->GetExpandedStyle().SetColour2( wxColour(80,120,179) );
    accordion->GetExpandedStyle().SetBorderColour( wxColour(47,79,79) );
    accordion->SetExpandButton( wxBitmap(expand02_xpm) );
    accordion->SetCollapseButton( wxBitmap(collapse02_xpm) );
    accordion->SetButtonMargin( wxSize(0,0) );

	wxPanel* panel1 = new wxPanel( accordion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	bSizer1->Add( new wxColourPickerCtrl( panel1, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE ), 0, wxALL, 5 );
	bSizer1->Add( new wxFontPickerCtrl( panel1, wxID_ANY, wxNullFont, wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE ), 0, wxALL, 5 );
	panel1->SetSizer( bSizer1 );
	panel1->Layout();
	accordion->AddPage(panel1,"Page 1",false);
	accordion->SetFixedHeight(0);

    wxPanel* panel2 = new wxPanel( accordion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );
	wxString m_choice1Choices[] = { wxT("Choice 1"), wxT("Choice 2"), wxT("Choice 3") };
	wxChoice* m_choice = new wxChoice( panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, m_choice1Choices, 0 );
	m_choice->SetSelection( 0 );
	bSizer2->Add( m_choice, 0, wxALL|wxEXPAND, 5 );
	bSizer2->Add( new wxSlider( panel2, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL ), 0, wxALL|wxEXPAND, 5 );
	bSizer2->Add( new wxButton( panel2, wxID_ANY, wxT("A Button"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	panel2->SetSizer( bSizer2 );
	panel2->Layout();
	accordion->AddPage(panel2,"Page 2",false);
	accordion->SetFixedHeight(1);

	wxPanel* panel3 = new wxPanel( accordion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	wxRadioButton* m_radioBtn1 = new wxRadioButton( panel3, wxID_ANY, wxT("Option 1"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_radioBtn1->SetValue( true );
	fgSizer->Add( m_radioBtn1, 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( panel3, wxID_ANY, wxT("Option 2"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( panel3, wxID_ANY, wxT("Option 3"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( panel3, wxID_ANY, wxT("Option 4"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( panel3, wxID_ANY, wxT("Option 5"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( panel3, wxID_ANY, wxT("Option 6"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	panel3->SetSizer( fgSizer );
	panel3->Layout();
	accordion->AddPage(panel3,"Page 3",false);
	accordion->SetFixedHeight(2);

    wxHtmlWindow* htmlWin = new wxHtmlWindow( splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>In this accordion, a whole panel of controls has been added to each fold.  Those controls are, however, just for demonstration purposes; none of them do anything.</p>"
        "<p>This accordion is also set up with a simpler, flatter style.  This type of style might be more desirable in some situations.</p>"
        "<p>Simple expand and collapse buttons have been added on the right side of the caption bars.  When those buttons are present, it is necessary to click on the buttons to open or close a page.  You can however change that behavior so that any click on the caption bar will toggle even when the button is present by changing the accordion’s settings if you want.  A double click at any place on the caption bar will also open or close the page.</p>"
        "<p>The events generated by opening and closing this accordion's pages are logged in the text control below.  You can prevent certain pages from opening or closing with the check boxes in the lower left of this page.</p>"
        ;
    setHTMLwin( htmlWin, s, accordion->GetBackgroundColour() );

	splitter2->SplitVertically( accordion, htmlWin, GetSize().GetWidth()*.25 );

    wxPanel* panel4 = new wxPanel( splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4 = new wxBoxSizer( wxVERTICAL );

	bSizer4->Add( new wxStaticLine( panel4), 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_page1CheckBox = new wxCheckBox( panel4, wxID_ANY, wxT("Page 1 can be opened"), wxDefaultPosition, wxDefaultSize, 0 );
	m_page1CheckBox->SetValue(true);
	bSizer6->Add( m_page1CheckBox, 0, wxALL, 5 );

	m_page2CheckBox = new wxCheckBox( panel4, wxID_ANY, wxT("Page 2 can be closed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_page2CheckBox->SetValue(true);
	bSizer6->Add( m_page2CheckBox, 0, wxALL, 5 );

	m_page3CheckBox = new wxCheckBox( panel4, wxID_ANY, wxT("Page 3 can be closed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_page3CheckBox->SetValue(true);
	bSizer6->Add( m_page3CheckBox, 0, wxALL, 5 );

	bSizer5->Add( bSizer6, 0, wxRIGHT, 5 );

	m_log2 = new wxTextCtrl( panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER );
	bSizer5->Add( m_log2, 1, wxEXPAND, 5 );

    bSizer4->Add( bSizer5, 1, wxEXPAND, 5 );

	panel4->SetSizer( bSizer4 );
	panel4->Layout();

    int splitPos =
        m_notebook->GetPage(0)->GetSize().GetHeight()
        - bSizer4->GetMinSize().GetHeight()
        - splitter1->GetSashSize()
        + 1;

	splitter1->SplitHorizontally( splitter2, panel4, splitPos );
	splitter1->SetMinimumPaneSize( bSizer4->GetMinSize().GetHeight() );
	m_notebook->AddPage( splitter1, wxT("Sample 2"), false );

    accordion->Bind( wxEVT_ACCORDION_COLLAPSED,  &ExtendedSampleFrame::onAccordion2Collapsed,  this );
    accordion->Bind( wxEVT_ACCORDION_COLLAPSING, &ExtendedSampleFrame::onAccordion2Collapsing, this );
    accordion->Bind( wxEVT_ACCORDION_EXPANDED,   &ExtendedSampleFrame::onAccordion2Expanded,   this );
    accordion->Bind( wxEVT_ACCORDION_EXPANDING,  &ExtendedSampleFrame::onAccordion2Expanding,  this );
}

void ExtendedSampleFrame::onAccordion2Expanding(wxBookCtrlEvent& event)
{
    (*m_log2) << wxDateTime::Now().Format("%H:%M:%S: ");
    (*m_log2) << "wxEVT_ACCORDION_EXPANDING\t";
    (*m_log2) << "Selection:" << event.GetSelection() << "\t";
    (*m_log2) << "Old Selection:" << event.GetOldSelection() << "\n";

    if(event.GetSelection()==0 && !m_page1CheckBox->IsChecked())
    {
        event.Veto();
        (*m_log2) << wxDateTime::Now().Format("%H:%M:%S: ");
        (*m_log2) << "wxEVT_ACCORDION_EXPANDING event vetoed.\n";
    }
}

void ExtendedSampleFrame::onAccordion2Expanded(wxBookCtrlEvent& event)
{
    (*m_log2) << wxDateTime::Now().Format("%H:%M:%S: ");
    (*m_log2) << "wxEVT_ACCORDION_EXPANDED\t";
    (*m_log2) << "Selection:" << event.GetSelection() << "\t";
    (*m_log2) << "Old Selection:" << event.GetOldSelection() << "\n";
}

void ExtendedSampleFrame::onAccordion2Collapsing(wxBookCtrlEvent& event)
{
    (*m_log2) << wxDateTime::Now().Format("%H:%M:%S: ");
    (*m_log2) << "wxEVT_ACCORDION_COLLAPSING\t";
    (*m_log2) << "Selection:" << event.GetSelection() << "\t";
    (*m_log2) << "Old Selection:" << event.GetOldSelection() << "\n";

    if(event.GetSelection()==1 && !m_page2CheckBox->IsChecked())
    {
        event.Veto();
        (*m_log2) << wxDateTime::Now().Format("%H:%M:%S: ");
        (*m_log2) << "wxEVT_ACCORDION_COLLAPSING event vetoed.\n";
    }

    if(event.GetSelection()==2 && !m_page3CheckBox->IsChecked())
    {
        event.Veto();
        (*m_log2) << wxDateTime::Now().Format("%H:%M:%S: ");
        (*m_log2) << "wxEVT_ACCORDION_COLLAPSING event vetoed.\n";
    }
}

void ExtendedSampleFrame::onAccordion2Collapsed(wxBookCtrlEvent& event)
{
    (*m_log2) << wxDateTime::Now().Format("%H:%M:%S: ");
    (*m_log2) << "wxEVT_ACCORDION_COLLAPSED\t";
    (*m_log2) << "Selection:" << event.GetSelection() << "\t";
    (*m_log2) << "Old Selection:" << event.GetOldSelection() << "\n";
}

void ExtendedSampleFrame::addSample3()
{
	wxSplitterWindow* splitter1 = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	splitter1->SetSashGravity( 1 );

	wxSplitterWindow* splitter2 = new wxSplitterWindow( splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    wxImageList* imageList = new wxImageList(16,15);
    imageList->Add(wxBitmap(home_xpm));
    imageList->Add(wxBitmap(repview_xpm));
    imageList->Add(wxBitmap(htmsidep_xpm));
    imageList->Add(wxBitmap(listview_xpm));

    m_accordion3 = new wxAccordion(splitter2);
    m_accordion3->AssignImageList(imageList);
    m_accordion3->SetAccordionPadding(0);

    m_accordion3->GetCollapsedStyle().SetColour1( wxColour(129,160,53) );
    m_accordion3->GetCollapsedStyle().SetColour2( wxColour(129,160,53) );
    m_accordion3->GetCollapsedStyle().AddGradientStop( wxColour(217,240,160), 0.25f);
    m_accordion3->GetCollapsedStyle().AddGradientStop( wxColour(171,200,100), 0.70f);
    m_accordion3->GetCollapsedStyle().SetBorderColour( wxColour(57,80,0) );
    m_accordion3->GetCollapsedStyle().SetTextColour( wxColour(57,80,0) );

    m_accordion3->GetExpandedStyle().SetColour1( wxColour(170,167,57) );
    m_accordion3->GetExpandedStyle().SetColour2( wxColour(170,167,57) );
    m_accordion3->GetExpandedStyle().AddGradientStop( wxColour(255,253,170), 0.25f);
    m_accordion3->GetExpandedStyle().AddGradientStop( wxColour(212,210,106), 0.75f);
    m_accordion3->GetExpandedStyle().SetBorderColour( wxColour(85,83,0) );
    m_accordion3->GetExpandedStyle().SetTextColour( wxColour(85,83,0) );

    m_accordion3->SetExpandButton( wxBitmap(expand03_xpm) );
    m_accordion3->SetCollapseButton( wxBitmap(collapse03_xpm) );
    m_accordion3->SetButtonMargin( wxSize(8,0) );

    m_accordion3->AddPage( new wxPanel(m_accordion3), "Page 1", false,  0 );
    m_accordion3->AddPage( new wxPanel(m_accordion3), "Page 2", false, 1 );
    m_accordion3->AddPage( new wxPanel(m_accordion3), "Page 3", true, 2 );
    m_accordion3->AddPage( new wxPanel(m_accordion3), "Page 4", false, 3 );

    wxHtmlWindow* htmlWin = new wxHtmlWindow( splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>In this sample, the caption bar’s gradient stops have been used to create a double transition effect.  The caption bar’s icons are also shown.</p>"
        "<p>When you left click on the accordion, a hit test will be performed, and the result will be logged in the text control below.  You can use the first check box below to block mouse clicks from reaching the accordion after the hit test is performed.  You can use the other 2 check boxes to toggle the caption bar’s icons and buttons.</p>";
        ;
    setHTMLwin( htmlWin, s, m_accordion3->GetBackgroundColour() );

	splitter2->SplitVertically( m_accordion3, htmlWin, GetSize().GetWidth()*.25 );

    wxPanel* panel1 = new wxPanel( splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );

	bSizer1->Add( new wxStaticLine( panel1), 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_blockMouseCheckBox =  new wxCheckBox( panel1, wxID_ANY, wxT("Block mouse clicks"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_blockMouseCheckBox, 0, wxALL, 5 );

	wxCheckBox* checkBox2 = new wxCheckBox( panel1, id_SHOWICONSCHECK, wxT("Show icons"), wxDefaultPosition, wxDefaultSize, 0 );
	checkBox2->SetValue(true);
	bSizer3->Add( checkBox2, 0, wxALL, 5 );

	wxCheckBox* checkBox3 = new wxCheckBox( panel1, id_SHOWBUTTONSCHECK, wxT("Show buttons"), wxDefaultPosition, wxDefaultSize, 0 );
	checkBox3->SetValue(true);
	bSizer3->Add( checkBox3, 0, wxALL, 5 );

	bSizer2->Add( bSizer3, 0, wxRIGHT, 5 );

	m_log3 = new wxTextCtrl( panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER );
	bSizer2->Add( m_log3, 1, wxEXPAND, 5 );

    bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

	panel1->SetSizer( bSizer1 );
	panel1->Layout();

    int splitPos =
        m_notebook->GetPage(0)->GetSize().GetHeight()
        - bSizer1->GetMinSize().GetHeight()
        - splitter1->GetSashSize()
        + 1;

	splitter1->SplitHorizontally( splitter2, panel1, splitPos );
	splitter1->SetMinimumPaneSize( bSizer1->GetMinSize().GetHeight() );
	m_notebook->AddPage( splitter1, wxT("Sample 3"), false );

    m_accordion3->Bind( wxEVT_LEFT_UP,     &ExtendedSampleFrame::onAccordion3MouseLeftUp, this );
    m_accordion3->Bind( wxEVT_LEFT_DCLICK, &ExtendedSampleFrame::onAccordion3MouseLeftDC, this );

    m_accordion3->GetPage(0)->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion3PageMouseLeftUp, this );
    m_accordion3->GetPage(1)->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion3PageMouseLeftUp, this );
    m_accordion3->GetPage(2)->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion3PageMouseLeftUp, this );
    m_accordion3->GetPage(3)->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion3PageMouseLeftUp, this );

	checkBox2->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,  &ExtendedSampleFrame::onChecks , this );
	checkBox3->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,  &ExtendedSampleFrame::onChecks , this );
}

void ExtendedSampleFrame::onAccordion3MouseLeftUp( wxMouseEvent& event )
{
    long flags;
    int result = m_accordion3->HitTest(wxPoint(event.GetX(),event.GetY()),&flags);

    (*m_log3) << wxDateTime::Now().Format("%H:%M:%S: HitTest result: ");
    if( result==wxNOT_FOUND)
    {
        (*m_log3) << "wxNOT_FOUND";
    }
    else
    {
        (*m_log3) << result;
    }

    (*m_log3) << "\tHitTest flags: ";

    bool first(true);

    if(flags&wxACCORDION_HITTEST_NOWHERE)
    {
        first = false;
        (*m_log3) << "wxACCORDION_HITTEST_NOWHERE";
    }

    if(flags&wxACCORDION_HITTEST_ONCAPTION)
    {
        if(first) first=false;
        else (*m_log3) << "|";

        (*m_log3) << "wxACCORDION_HITTEST_ONCAPTION";
    }

    if(flags&wxACCORDION_HITTEST_ONCAPTIONBORDER)
    {
        if(first) first=false;
        else (*m_log3) << "|";

        (*m_log3) << "wxACCORDION_HITTEST_ONCAPTIONBORDER";
    }

    if(flags&wxACCORDION_HITTEST_ONCAPTIONLABEL)
    {
        if(first) first=false;
        else (*m_log3) << "|";

        (*m_log3) << "wxACCORDION_HITTEST_ONCAPTIONLABEL";
    }

    if(flags&wxACCORDION_HITTEST_ONCAPTIONICON)
    {
        if(first) first=false;
        else (*m_log3) << "|";

        (*m_log3) << "wxACCORDION_HITTEST_ONCAPTIONICON";
    }

    if(flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON)
    {
        if(first) first=false;
        else (*m_log3) << "|";

        (*m_log3) << "wxACCORDION_HITTEST_ONCAPTIONBUTTON";
    }

    if(flags&wxACCORDION_HITTEST_ONPAGE)
    {
        if(first) first=false;
        else (*m_log3) << "|";

        (*m_log3) << "wxACCORDION_HITTEST_ONPAGE";
    }

    if(flags&wxACCORDION_HITTEST_ONPAGEBORDER)
    {
        if(first) first=false;
        else (*m_log3) << "|";

        (*m_log3) << "wxACCORDION_HITTEST_ONPAGEBORDER";
    }

    if(flags&wxACCORDION_HITTEST_INPAGEPADDING)
    {
        if(first) first=false;
        else (*m_log3) << "|";

        (*m_log3) << "wxACCORDION_HITTEST_INPAGEPADDING";
    }

    (*m_log3) << "\n";


    if( !m_blockMouseCheckBox->IsChecked() )
    {
        event.Skip();
    }
}

void ExtendedSampleFrame::onAccordion3MouseLeftDC( wxMouseEvent& event )
{
    if( !m_blockMouseCheckBox->IsChecked() )
    {
        event.Skip();
    }
}

void ExtendedSampleFrame::onAccordion3PageMouseLeftUp( wxMouseEvent& event )
{
    if( wxPanel* m_panel = wxDynamicCast(event.GetEventObject(),wxPanel) )
    {
        wxWindow* parent= m_panel->GetParent();

        event.SetX(event.GetX()+m_panel->GetRect().x);
        event.SetY(event.GetY()+m_panel->GetRect().y);
        event.SetEventObject(parent);
        event.SetId(parent->GetId());
        parent->ProcessWindowEvent(event);
    }
}

void ExtendedSampleFrame::onChecks(wxCommandEvent& event)
{
    if(event.IsChecked())
    {
        if(event.GetId()==id_SHOWICONSCHECK)
        {
            wxImageList* il = new wxImageList(16, 15);

            il->Add(wxBitmap(home_xpm));
            il->Add(wxBitmap(repview_xpm));
            il->Add(wxBitmap(htmsidep_xpm));
            il->Add(wxBitmap(listview_xpm));
            m_accordion3->AssignImageList(il);
        }
        else if(event.GetId()==id_SHOWBUTTONSCHECK)
        {
            m_accordion3->SetExpandButton( wxBitmap(expand03_xpm) );
            m_accordion3->SetCollapseButton( wxBitmap(collapse03_xpm) );
        }
    }
    else
    {
        if(event.GetId()==id_SHOWICONSCHECK)
        {
            m_accordion3->AssignImageList(NULL);
        }
        else if(event.GetId()==id_SHOWBUTTONSCHECK)
        {
            m_accordion3->SetExpandButton( wxNullBitmap );
            m_accordion3->SetCollapseButton( wxNullBitmap );
        }
    }

    m_accordion3->Layout();
    m_accordion3->Refresh();
}

void ExtendedSampleFrame::addSample4()
{
	wxSplitterWindow* splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    wxAccordion* accordion = new wxAccordion( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAC_SINGLE_FOLD );

    accordion->SetAccordionPadding(2);
    accordion->SetHighlighting(wxAC_HIGHLIGHTING_MOUSEOVER);

    accordion->GetCollapsedStyle().SetBGBitmap( wxBitmap(back04_xpm) );
    accordion->GetCollapsedStyle().SetBorderColour(wxTransparentColour);
    accordion->GetCollapsedStyle().SetTextColour( wxColour(64,0,128) );
    accordion->GetCollapsedStyle().SetFont( GetFont().MakeBold() );
    accordion->SetExpandedStyle( accordion->GetCollapsedStyle() );

    accordion->GetCollapsedHLStyle().SetBGBitmap( wxBitmap(back04hl_xpm) );
    accordion->GetCollapsedHLStyle().SetBorderColour(wxTransparentColour);
    accordion->GetCollapsedHLStyle().SetTextColour( *wxWHITE );
    accordion->GetCollapsedHLStyle().SetFont( GetFont().MakeBold() );
    accordion->SetExpandedHLStyle( accordion->GetCollapsedHLStyle() );

    accordion->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 1",false);
    accordion->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 2",false);
    accordion->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 3",false);
    accordion->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 4",true);

    wxHtmlWindow* htmlWin = new wxHtmlWindow( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>In this sample, an image file is used for the caption bar background.</p>"
        "<p>This sample also uses highlighting so that the style changes when the mouse is over a caption bar.</p>"
        "<p>And finally, this sample demonstrates the wxACCORDION_SINGLE_FOLD style.  When a page is expanded, all other pages are collapsed. </p>"
        ;
    setHTMLwin( htmlWin, s, accordion->GetBackgroundColour() );

	splitter->SplitVertically( accordion, htmlWin, GetSize().GetWidth()*.25 );

	m_notebook->AddPage( splitter, wxT("Sample 4"), false );
}

void ExtendedSampleFrame::addSample5()
{
	wxSplitterWindow* splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    m_accordion5 = new wxAccordion( splitter );

    m_accordion5->SetCaptionGradientAngle(90);
    m_accordion5->GetCollapsedStyle().SetColour1( wxColour(64,128,128) );
    m_accordion5->GetCollapsedStyle().SetColour2( wxColour(157,185,235) );
    m_accordion5->GetCollapsedStyle().SetBorderColour(wxTransparentColour);
    m_accordion5->GetCollapsedStyle().SetTextColour( *wxWHITE );
    m_accordion5->SetExpandedStyle( m_accordion5->GetCollapsedStyle() );

    m_accordion5->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 1",true);
    m_accordion5->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 2",false);
    m_accordion5->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 3",false);
    m_accordion5->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 4",false);

    wxHtmlWindow* htmlWin = new wxHtmlWindow( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>In this sample, the gradient angle was set to  90 degrees so that the gradient runs from left to right instead of top to bottom.</p>"
        "<p>This sample also tracks mouse cursor movements and opens a page whenever the mouse moves over a caption bar.  This type of accordion is sometimes called a “roll-over accordion”.</p>"
        ;
    setHTMLwin( htmlWin, s, m_accordion5->GetBackgroundColour() );

	splitter->SplitVertically( m_accordion5, htmlWin, GetSize().GetWidth()*.25 );

	m_notebook->AddPage( splitter, wxT("Sample 5"), false );
	m_lastFound=wxNOT_FOUND;

    m_accordion5->Bind(wxEVT_ACCORDION_COLLAPSING, &ExtendedSampleFrame::block, this );
    m_accordion5->Bind(wxEVT_ACCORDION_EXPANDING, &ExtendedSampleFrame::block, this );
	m_accordion5->Bind( wxEVT_MOTION, &ExtendedSampleFrame::onAccordion5Mouse, this );
}

void ExtendedSampleFrame::onAccordion5Mouse( wxMouseEvent& event )
{
    int result = m_accordion5->HitTest( wxPoint( event.GetX(), event.GetY() ) );

    if( result!=wxNOT_FOUND && result!=m_lastFound )
    {
        m_accordion5->Collapse(m_lastFound);
        m_accordion5->Expand(result);
        m_lastFound=result;
    }
}

void ExtendedSampleFrame::addSample6()
{
	wxSplitterWindow* splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE );
	splitter->SetBackgroundStyle(wxBG_STYLE_PAINT);

    wxAccordion* accordion = new wxAccordion( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAC_BUTTON_LEFT|wxAC_COLLAPSE_TO_BOTTOM );
    accordion->SetBackgroundColour( *wxWHITE );

    accordion->SetAccordionPadding(1);
    accordion->SetMinCaptionHeight(28);
    accordion->SetCaptionRadius(10);
    accordion->SetCaptionGradientAngle(2);
    accordion->SetExpandButton( wxBitmap(expand06_xpm) );
    accordion->SetCollapseButton( wxBitmap(collapse06_xpm) );

    accordion->GetCollapsedStyle().SetColour1( wxColour(182,9,0) );
    accordion->GetCollapsedStyle().SetColour2( wxColour(160,8,0) );
    accordion->GetCollapsedStyle().AddGradientStop( wxColour(182,9,0), 0.49f);
    accordion->GetCollapsedStyle().AddGradientStop( wxColour(160,8,0), 0.51f);
    accordion->GetCollapsedStyle().SetBorderColour( wxColour(142,10,0) );
    accordion->GetCollapsedStyle().SetTextColour( wxColour(255,255,255) );
    accordion->GetCollapsedStyle().SetFont( GetFont().Bold() );

    accordion->GetExpandedStyle().SetColour1( wxColour(255,255,255) );
    accordion->GetExpandedStyle().SetColour2( wxColour(254,254,254) );
    accordion->GetExpandedStyle().SetBorderColour( wxColour(142,10,0) );
    accordion->GetExpandedStyle().SetTextColour( wxColour(142,10,0) );
    accordion->GetExpandedStyle().SetFont( GetFont().Bold() );

    accordion->SetPageRadius(10);
    accordion->GetPageStyle().SetColour1( wxColour(254,248,230) );
    accordion->GetPageStyle().SetColour2( wxColour(254,238,189) );
    accordion->GetPageStyle().SetBorderColour( wxColour(142,10,0) );

    accordion->AddPage( new wxTextCtrl(accordion,wxID_ANY),"Page 1",false);
    accordion->AddPage( new wxButton( accordion, wxID_ANY, wxT("A Button") ), "Page 2 (Fixed Width)", false);
    accordion->AddPage( new wxButton( accordion, wxID_ANY, wxT("A Button") ), "Page 3 (Fixed Height)", false);
    accordion->AddPage( new wxButton( accordion, wxID_ANY, wxT("A Button") ), "Page 4 (Fixed Size)", false);

    accordion->SetFixedWidth(1);
    accordion->SetFixedHeight(2);
    accordion->SetFixedSize(3);

    wxHtmlWindow* htmlWin = new wxHtmlWindow( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>In this sample:</p>"
        "<ul>"
        "<li>Both the caption bar and the window are given a radius of 10 so that the corners are rounded instead of square. </li>"
        "<li>The caption bar\'s gradient stops were used to create a 2 tone look instead of a smooth gradient. </li>"
        "<li>The gradient angle was set to 1.4 degrees so that the division between the 2 colors is drawn with a small slope. </li>"
        "<li>The page background is also drawn with a slight gradient. </li>"
        "<li>The wxACCORDION_BUTTON_LEFT style is used so that the collapse and expand buttons are drawn on the left side of the caption bar. </li>"
        "<li>This accordion also demonstrates the wxACCORDION_COLLAPSE_TO_BOTTOM style.</li>"
        "<li>Panels 2, 3, and 4 demonstrate setting a panel to have fixed width, height, and size respectively.  This can be used when the window added to the accordion is not meant to expand in one or both directions. </li>"
        "</ul>"
        ;
    setHTMLwin( htmlWin, s, accordion->GetBackgroundColour() );

	splitter->SplitVertically( accordion, htmlWin, GetSize().GetWidth()*.25 );
	m_notebook->AddPage( splitter, wxT("Sample 6"), false );

    splitter->Bind(wxEVT_PAINT,&ExtendedSampleFrame::onSplitterPaint,this);
    splitter->Bind(wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED,&ExtendedSampleFrame::onSplitterSash,this);
}

void ExtendedSampleFrame::addSample7()
{
	wxSplitterWindow* splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE );
	splitter->SetBackgroundStyle(wxBG_STYLE_PAINT);

    wxPanel* panel = new wxPanel( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	panel->SetBackgroundColour( *wxBLACK );

	wxBoxSizer* bSizerH = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* bSizerV = new wxBoxSizer( wxVERTICAL );

    wxStaticText* staticText = new wxStaticText( panel, wxID_ANY, wxT("My Accordion") );
	staticText->SetFont( GetFont().MakeBold() );
	staticText->SetForegroundColour( *wxWHITE );
    bSizerV->Add( staticText, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxDelegateRendererNative rn = wxRendererNative::Get();
    wxSize sz = rn.GetCheckBoxSize(NULL);

    wxImageList* imageList = new wxImageList(sz.GetWidth(), sz.GetHeight());
    wxBitmap unchecked  = wxBitmap(sz);
    wxBitmap checked  = wxBitmap(sz);

    wxMemoryDC temp_dc;
    temp_dc.SelectObject(unchecked);
    rn.DrawCheckBox(panel, temp_dc, wxRect(0,0,sz.GetWidth(),sz.GetHeight()),0 );
    temp_dc.SelectObject(checked);
    rn.DrawCheckBox(panel, temp_dc, wxRect(0,0,sz.GetWidth(),sz.GetHeight()),wxCONTROL_CHECKED );
    temp_dc.SelectObject(wxNullBitmap);

    imageList->Add( wxBitmap(unchecked) );
    imageList->Add( wxBitmap(checked) );

    wxAccordion* accordion = new wxAccordion( panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAC_FLOAT_TO_TOP );
    accordion->AssignImageList(imageList);
    accordion->SetAccordionPadding(1);
    accordion->SetDisabledBrightness(150);

    accordion->GetCollapsedStyle().SetColour1( wxColour(50,50,50) );
    accordion->GetCollapsedStyle().SetColour2( wxColour(50,50,50) );
    accordion->GetCollapsedStyle().SetBorderColour( wxTransparentColor );
    accordion->GetCollapsedStyle().SetTextColour( *wxWHITE );
    accordion->SetExpandedStyle(accordion->GetCollapsedStyle());

    accordion->GetPageStyle().SetColour1( *wxBLACK );
    accordion->GetPageStyle().SetColour2( *wxBLACK );
    accordion->GetPageStyle().SetBorderColour( wxTransparentColor );

    accordion->SetCollapseButton( wxBitmap(collapse07_xpm) );
    accordion->SetExpandButton( wxBitmap(expand07_xpm) );

    wxTextCtrl* textCtrl;
    textCtrl = new wxTextCtrl( accordion, wxID_ANY );
	textCtrl->SetForegroundColour( *wxWHITE );
	textCtrl->SetBackgroundColour( *wxBLACK );
    accordion->AddPage( textCtrl, "Page 1", false, 1);

    textCtrl = new wxTextCtrl( accordion, wxID_ANY );
	textCtrl->SetForegroundColour( *wxWHITE );
	textCtrl->SetBackgroundColour( *wxBLACK  );
    accordion->AddPage( textCtrl, "Page 2", false, 1);

    textCtrl = new wxTextCtrl( accordion, wxID_ANY );
	textCtrl->SetForegroundColour( *wxWHITE );
	textCtrl->SetBackgroundColour( *wxBLACK  );
    accordion->AddPage( textCtrl, "Page 3", false, 1);

    bSizerV->Add( accordion, 1, wxEXPAND, 5 );
    bSizerH->Add( bSizerV, 1, wxEXPAND, 5 );

    wxPanel* rightBorder = new wxPanel( panel, wxID_ANY, wxDefaultPosition, wxSize( 1,-1 ), wxTAB_TRAVERSAL );
	rightBorder->SetBackgroundColour( wxColour( 71, 71, 71 ) );
    bSizerH->Add( rightBorder, 0, wxEXPAND, 5 );
    panel->SetSizer( bSizerH );
	panel->Layout();

    wxHtmlWindow* htmlWin = new wxHtmlWindow( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>This sample shows an accordion styled to look like a control on the android platform.  </p>"
        "<p>This sample also demonstrates the wxACCORDION_FLOAT_TO_TOP style.  When a page is expanded, it is moved towards the top of the accordion. </p>"
        "<p>This accordion also uses the caption bar's icons to simulate check boxes.  Clicking on a check box will toggle the panel between enabled and disabled states. </p>"
        ;
    setHTMLwin( htmlWin, s, accordion->GetBackgroundColour(), *wxWHITE );

	splitter->SplitVertically( panel, htmlWin, GetSize().GetWidth()*.25 );
	m_notebook->AddPage( splitter, wxT("Sample 7"), false );

    accordion->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion7MouseLeftUp, this );
    splitter->Bind(wxEVT_PAINT,&ExtendedSampleFrame::onSplitterPaint,this);
    splitter->Bind(wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED,&ExtendedSampleFrame::onSplitterSash,this);
}

void ExtendedSampleFrame::onAccordion7MouseLeftUp( wxMouseEvent& event )
{
    if( wxAccordion* accordion = wxDynamicCast( event.GetEventObject(), wxAccordion) )
    {
        long flags(0);
        int res = accordion->HitTest(wxPoint(event.GetX(),event.GetY()),&flags);;

        if(flags & wxACCORDION_HITTEST_ONCAPTIONICON)
        {
            int ic = accordion->GetPageImage(res);

            if(ic==0)
            {
                accordion->SetPageImage(res,1);
                accordion->Enable(res);
            }
            else
            {
                accordion->SetPageImage(res,0);
                accordion->Disable(res);
            }
        }
        else
        {
            event.Skip();
        }
    }
}

void styleSubAccordion( wxAccordion* accordion, int level, bool useImageList)
{
    wxImageList* imageList = new wxImageList(13,13);

    if(useImageList)
    {
        imageList->Add(wxBitmap(subempty_xpm));
        imageList->Add(wxBitmap(subcollapse_xpm));
        imageList->Add(wxBitmap(subexpand_xpm));
    }

    accordion->AssignImageList(imageList);
    accordion->SetIconMargin( wxSize(5 +13*(level-1),0) );

    accordion->SetPagePadding(0);
    accordion->SetAccordionPadding(0);

    accordion->SetHighlighting(wxAC_HIGHLIGHTING_MOUSEOVER);
    accordion->GetCollapsedStyle().SetColour1( wxColour(255,255,255) );
    accordion->GetCollapsedStyle().SetColour2( wxColour(255,255,255) );
    accordion->GetCollapsedStyle().SetBorderColour(wxTransparentColour);
    accordion->GetCollapsedStyle().SetTextColour( wxColour(59,59,59) );
    accordion->SetExpandedStyle(accordion->GetCollapsedStyle());

    accordion->GetCollapsedHLStyle().SetColour1( wxColour(59,184,204) );
    accordion->GetCollapsedHLStyle().SetColour2( wxColour(59,184,204) );
    accordion->GetCollapsedHLStyle().AddGradientStop( wxColour(59,183,235), 0.01f);
    accordion->GetCollapsedHLStyle().AddGradientStop( wxColour(59,183,235), 0.99f);
    accordion->GetCollapsedHLStyle().SetBorderColour(wxTransparentColour);
    accordion->GetCollapsedHLStyle().SetTextColour( wxColour(59,59,59) );
    accordion->SetExpandedHLStyle(accordion->GetCollapsedHLStyle());

    accordion->SetName("subaccordion");
}

void setSizes( wxAccordion* accordion)
{
    wxAccordion *curAccordion;
    std::stack<wxAccordion*> myStack;
    bool canSetHeight(true);
    int height(0);

    myStack.push(accordion);

    while( !myStack.empty() )
    {
        height=0;
        canSetHeight=true;
        curAccordion = myStack.top();
        myStack.pop();

        for( size_t i=0; i<curAccordion->GetPageCount(); i++ )
        {
            if( wxAccordion* childAccordion = wxDynamicCast(curAccordion->GetPage(i),wxAccordion) )
            {
                if( childAccordion->GetMinSize()==wxDefaultSize )
                {
                    myStack.push(curAccordion);
                    myStack.push(childAccordion);
                    canSetHeight=false;
                    break;
                }
                else if( curAccordion->IsExpanded(i) )
                {
                    height+=childAccordion->GetMinSize().GetHeight();
                }
            }
        }

        if(canSetHeight)
        {
            height+=curAccordion->GetPageCount()*curAccordion->GetCaptionBarHeight();
            curAccordion->SetMinSize( wxSize(-1,height) );
        }
    }
}

void setHTMLwin8(wxHtmlWindow* htmlWin, const wxString& s2, const wxColour& c )
{
    wxString s = ""
    "<p>This sample demonstrates how an accordion might be used more as a content selector than as a control container. </p>"
    "<p>In this sample, the pages are other accordions, but the pages of those accordions don’t expand or collapse. Instead, when any of the caption bars are clicked, the message displayed here will change slightly. </p>"
    ;

    if(!s2.IsEmpty())
    {
        s<<"<p>";
        s<<s2;
        s <<"</p>";
    };

   setHTMLwin( htmlWin, s, c );
}

void ExtendedSampleFrame::addSample8()
{
	wxSplitterWindow* splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE );
	splitter->SetBackgroundStyle(wxBG_STYLE_PAINT);

	m_leftPanel8 = new wxPanel( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_leftPanel8->SetBackgroundColour( wxColour( 240, 240, 240 ) );

	wxBoxSizer* bSizer = new wxBoxSizer( wxVERTICAL );

	wxPanel* borderPanel = new wxPanel( m_leftPanel8 );
	borderPanel->SetBackgroundColour( wxColour( 128, 128, 128 ) );

	wxBoxSizer* borderSizer = new wxBoxSizer( wxHORIZONTAL );

	wxAccordion* accordion = new wxAccordion(borderPanel);

    accordion->SetPagePadding(0);
    accordion->SetAccordionPadding(0);
    accordion->SetCollapseButton(collapse08_xpm);
    accordion->SetExpandButton(expand08_xpm);
    accordion->SetButtonMargin( wxSize(5,0) );
    accordion->SetAccordionPadding(0);
    accordion->SetToggleStyle(wxAC_TOGGLE_ON_ANY_CLICK);
    accordion->GetCollapsedStyle().SetColour1( wxColour(115,115,115) );
    accordion->GetCollapsedStyle().SetColour2( wxColour(82,82,82) );
    accordion->GetCollapsedStyle().SetBorderColour(wxTransparentColour);
    accordion->GetCollapsedStyle().SetTextColour( wxColour(255,255,255) );
    accordion->GetExpandedStyle().SetColour1( wxColour(115,115,115) );
    accordion->GetExpandedStyle().SetColour2( wxColour(82,82,82) );
    accordion->GetExpandedStyle().SetBorderColour(wxTransparentColour);

	wxAccordion* subaccordion1 = new wxAccordion(accordion);
	styleSubAccordion( subaccordion1, 1, true );

	wxAccordion* subaccordion2 = new wxAccordion(accordion);
	styleSubAccordion( subaccordion2, 1, false );

	wxAccordion* subaccordion3 = new wxAccordion(accordion);
	styleSubAccordion( subaccordion3, 2, false );

    subaccordion3->AddPage( new wxPanel( subaccordion3, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ), "Subitem 1-1-1",false,0);
    subaccordion3->AddPage( new wxPanel( subaccordion3, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ), "Subitem 1-1-2",false,0);

    subaccordion2->AddPage( new wxPanel( subaccordion2, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ),"Subitem 2-1",false,0);
    subaccordion2->AddPage( new wxPanel( subaccordion2, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ), "Subitem 2-2",false,0);

    subaccordion1->AddPage( subaccordion3 ,"Subitem 1-1",false,2);
    subaccordion1->AddPage( new wxPanel( subaccordion1, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ),"Subitem 1-2",false,0);

    accordion->AddPage(subaccordion1,"Item 1",true);
    accordion->AddPage(subaccordion2,"Item 2",true);

    subaccordion3->SetFixedHeight(0);
    subaccordion3->SetFixedHeight(1);
    subaccordion1->SetFixedHeight(0);
    subaccordion1->SetFixedHeight(1);
    subaccordion2->SetFixedHeight(0);
    subaccordion2->SetFixedHeight(1);
    accordion->SetFixedHeight(0);
    accordion->SetFixedHeight(1);

    setSizes( accordion);

	borderSizer->Add( accordion, 1, wxEXPAND|wxALL, 1 );
	borderPanel->SetSizer( borderSizer );
	borderPanel->Layout();
	borderSizer->Fit( borderPanel );

	bSizer->Add( borderPanel, 0, wxALL|wxEXPAND, 5 );
	m_leftPanel8->SetSizer( bSizer );
	m_leftPanel8->Layout();

    m_htmlWin8 = new wxHtmlWindow( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    m_htmlWin8->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    setHTMLwin8( m_htmlWin8, "" , m_leftPanel8->GetBackgroundColour() );

	splitter->SplitVertically( m_leftPanel8, m_htmlWin8, GetSize().GetWidth()*.25 );
	m_notebook->AddPage( splitter, wxT("Sample 8"), false );

    splitter->Bind( wxEVT_PAINT, &ExtendedSampleFrame::onSplitterPaint, this );
    splitter->Bind( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, &ExtendedSampleFrame::onSplitterSash, this );
    accordion->Bind( wxEVT_ACCORDION_COLLAPSED, &ExtendedSampleFrame::onAccordion8, this );
    accordion->Bind( wxEVT_ACCORDION_EXPANDED, &ExtendedSampleFrame::onAccordion8, this );
}

void ExtendedSampleFrame::onAccordion8(wxBookCtrlEvent& event)
{
    //The problem is that when a subaccordion is toggled, we need to adjust the height
    //of its containing accordion and that accordion's containing accorion and so on.
    //We also need to adjust the icon simulating a button the collapsed or expanded version.

    bool collapsed(event.GetEventType() == wxEVT_ACCORDION_COLLAPSED);
    int selection = event.GetSelection();

    if( wxAccordion* eventAccordion = wxDynamicCast(event.GetEventObject(),wxAccordion) )
    {
        wxString message = eventAccordion->GetPageText(selection);

        if( wxAccordion* eventSubaccordion = wxDynamicCast( eventAccordion->GetPage(selection),wxAccordion) )
        {
            int height( eventSubaccordion->GetMinSize().GetHeight() ),parentHeight(0);
            wxWindow* parent(eventAccordion);

            if( eventAccordion->GetName() == "subaccordion" )
            {
                eventAccordion->SetPageImage( selection, (collapsed?2:1) );
            }

            while( parent!=NULL )
            {
                if( wxDynamicCast(parent,wxAccordion) )
                {
                    parentHeight = parent->GetMinSize().GetHeight();
                    parent->SetMinSize( wxSize( -1,(collapsed?parentHeight-height:parentHeight+height)) );
                }

                if( parent->GetName()=="accordion" )
                {
                    break;
                }

                parent=parent->GetParent();
            }

            m_leftPanel8->Layout();
            message << (collapsed?" was collapsed.":" was expanded.");
        }
        else
        {
            message << " was selected.";
        }

        setHTMLwin8( m_htmlWin8, message, m_leftPanel8->GetBackgroundColour() );
    }
}
