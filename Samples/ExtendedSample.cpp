/***************************************************************
 * Name:      ExtendedSample.cpp
 * Purpose:   wxAccordion Extended Sample
 * Author:    ()
 * Created:   2015-10-10
 * Copyright: ()
 * License:   wxWindows licence
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/accordion/accordion.h>
#include <wx/html/htmlwin.h>
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/msgdlg.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/clrpicker.h>
#include <wx/fontpicker.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/dcclient.h>
#include <wx/renderer.h>
#include <wx/dcmemory.h>
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
	    void OnAbout( wxCommandEvent& event ){wxMessageBox( wxT("the wxAccordion extended sample."), wxT("Welcome to...") );}
	    void OnClose( wxCloseEvent& event ) { Destroy(); }
        void OnQuit( wxCommandEvent& event ){ Destroy(); }

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
		wxCheckBox* page1CheckBox;
		wxCheckBox* page2CheckBox;
		wxCheckBox* page3CheckBox;

		wxAccordion* m_accordion3;
		wxCheckBox* blockMouseCheckBox;
		wxTextCtrl* m_log3;

		wxAccordion* m_accordion5;
		int lastFound;

		wxPanel* sample8LeftPanel;
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

void setHTMLwin( wxHtmlWindow* m_htmlWin, const wxString& s, const wxColour& c ,const wxColour& c2 = *wxBLACK)
{
    wxString s2 = "<html><body bgcolor=\"";
    s2 << c.GetAsString();
    s2 << "\">";
    s2 << "<font color=\"";
    s2 << c2.GetAsString();
    s2 << "\">";
    s2 << s;
    s2 << "</font></body></html>";
    m_htmlWin->SetPage(s2);
}

void paintSash( wxDC* myDC, wxSplitterWindow* m_splitter, const wxColor& col )
{
    myDC->SetBrush(col);
    myDC->SetPen(*wxTRANSPARENT_PEN);

    myDC->DrawRectangle
    (
        m_splitter->GetSashPosition(),
        0,
        m_splitter->GetSashSize(),
        m_splitter->GetSize().GetHeight()
    );
}

void ExtendedSampleFrame::onSplitterPaint( wxPaintEvent& event )
{
    if( wxSplitterWindow* m_split = wxDynamicCast(event.GetEventObject(),wxSplitterWindow) )
    {
        wxPaintDC myDC(m_split);
        paintSash(&myDC, m_split, m_split->GetWindow1()->GetBackgroundColour() );
    }
}

void ExtendedSampleFrame::onSplitterSash( wxSplitterEvent& event )
{
    if( wxSplitterWindow* m_split = wxDynamicCast(event.GetEventObject(),wxSplitterWindow))
    {
        m_split->Refresh();
    }
}

void ExtendedSampleFrame::addSample1()
{
    wxPanel* m_panel = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer = new wxBoxSizer( wxVERTICAL );

    wxSplitterWindow* m_splitter = new wxSplitterWindow( m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    m_accordion1 = new wxAccordion(m_splitter);
    m_accordion1->AddPage( new wxTextCtrl(m_accordion1,wxID_ANY), "Page 1", true );
    m_accordion1->AddPage( new wxTextCtrl(m_accordion1,wxID_ANY), "Page 2", false );
    m_accordion1->AddPage( new wxTextCtrl(m_accordion1,wxID_ANY), "Page 3", false );
    m_accordion1->AddPage( new wxTextCtrl(m_accordion1,wxID_ANY), "Page 4", false );

    wxHtmlWindow* m_htmlWin = new wxHtmlWindow( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    m_htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>This sample shows an unmodified accordion. "
        "You can open or close the pages by clicking on the page’s caption bar.</p>"
        "<p>In this case, each page contains only a single text control, "
        "but any wxWindow can be added to a page.</p>"
        "<p>You can also use the buttons below to expand, collapse, toggle, enable, "
        "or disable any of the pages in the accordion.</p>"
        ;
    setHTMLwin( m_htmlWin, s, m_accordion1->GetBackgroundColour() );

    m_splitter->SplitVertically( m_accordion1, m_htmlWin, GetSize().GetWidth()*.25 );
    bSizer->Add( m_splitter, 1, wxEXPAND, 5 );

	bSizer->Add( new wxStaticLine(m_panel), 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	bSizer2->Add( new wxStaticText( m_panel, wxID_ANY, wxT("Page") ), 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_spinCtrl = new wxSpinCtrl( m_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxSP_ARROW_KEYS, 1, 4, 1 );
	bSizer2->Add( m_spinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	wxButton* m_button1 = new wxButton( m_panel, id_EXPANDBUTTON, wxT("Expand"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( m_button1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxButton* m_button2 = new wxButton( m_panel, id_COLLAPSEBUTTON, wxT("Collapse"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( m_button2, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	wxButton* m_button3 = new wxButton( m_panel, id_TOGGLEBUTTON, wxT("Toggle"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( m_button3, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	wxButton* m_button4 = new wxButton( m_panel, id_ENABLEBUTTON, wxT("Enable"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( m_button4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	wxButton* m_button5 = new wxButton( m_panel, id_DISABLEBUTTON, wxT("Disable"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( m_button5, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	bSizer->Add( bSizer2, 0, wxEXPAND, 5 );
    m_panel->SetBackgroundColour( m_accordion1->GetBackgroundColour() );

    m_panel->SetSizer( bSizer );
	m_panel->Layout();
	bSizer->Fit( m_panel );

    m_notebook->AddPage( m_panel, wxT("Sample 1"), true );

	m_button1->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
	m_button2->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
	m_button3->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
	m_button4->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
	m_button5->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &ExtendedSampleFrame::onButton, this );
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
	wxSplitterWindow* m_splitter1 = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_splitter1->SetSashGravity( 1 );

	wxSplitterWindow* m_splitter2 = new wxSplitterWindow( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    wxAccordion* m_accordion = new wxAccordion(m_splitter2);
    m_accordion->SetAccordionPadding(3);
    m_accordion->SetCollapsedColour1( wxColour(46,77,123) );
    m_accordion->SetCollapsedColour2( wxColour(46,77,123) );
    m_accordion->SetCollapsedBorderColour( wxColour(47,79,79) );
    m_accordion->SetCollapsedTextColour( wxColour(255,255,255) );
    m_accordion->SetExpandedColour1( wxColour(80,120,179) );
    m_accordion->SetExpandedColour2( wxColour(80,120,179) );
    m_accordion->SetExpandedBorderColour( wxColour(47,79,79) );
    m_accordion->SetExpandButton( wxBitmap(expand02_xpm) );
    m_accordion->SetCollapseButton( wxBitmap(collapse02_xpm) );
    m_accordion->SetButtonMargin( wxSize(0,0) );

	wxPanel* m_panel1 = new wxPanel( m_accordion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	bSizer1->Add( new wxColourPickerCtrl( m_panel1, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE ), 0, wxALL, 5 );
	bSizer1->Add( new wxFontPickerCtrl( m_panel1, wxID_ANY, wxNullFont, wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE ), 0, wxALL, 5 );
	m_panel1->SetSizer( bSizer1 );
	m_panel1->Layout();
	m_accordion->AddPage(m_panel1,"Page 1",false);
	m_accordion->SetFixedHeight(0);

    wxPanel* m_panel2 = new wxPanel( m_accordion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );
	wxString m_choice1Choices[] = { wxT("Choice 1"), wxT("Choice 2"), wxT("Choice 3") };
	wxChoice* m_choice = new wxChoice( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, m_choice1Choices, 0 );
	m_choice->SetSelection( 0 );
	bSizer2->Add( m_choice, 0, wxALL|wxEXPAND, 5 );
	bSizer2->Add( new wxSlider( m_panel2, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL ), 0, wxALL|wxEXPAND, 5 );
	bSizer2->Add( new wxButton( m_panel2, wxID_ANY, wxT("A Button"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	m_panel2->SetSizer( bSizer2 );
	m_panel2->Layout();
	m_accordion->AddPage(m_panel2,"Page 2",false);
	m_accordion->SetFixedHeight(1);

	wxPanel* m_panel3 = new wxPanel( m_accordion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	wxRadioButton* m_radioBtn1 = new wxRadioButton( m_panel3, wxID_ANY, wxT("Option 1"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_radioBtn1->SetValue( true );
	fgSizer->Add( m_radioBtn1, 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( m_panel3, wxID_ANY, wxT("Option 2"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( m_panel3, wxID_ANY, wxT("Option 3"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( m_panel3, wxID_ANY, wxT("Option 4"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( m_panel3, wxID_ANY, wxT("Option 5"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	fgSizer->Add( new wxRadioButton( m_panel3, wxID_ANY, wxT("Option 6"), wxDefaultPosition, wxDefaultSize, 0 ), 0, wxALL, 5 );
	m_panel3->SetSizer( fgSizer );
	m_panel3->Layout();
	m_accordion->AddPage(m_panel3,"Page 3",false);
	m_accordion->SetFixedHeight(2);

    wxHtmlWindow* m_htmlWin = new wxHtmlWindow( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    m_htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>In this accordion, a whole panel of controls has been added to each fold.  Those controls are, however, just for demonstration purposes; none of them do anything.</p>"
        "<p>This accordion is also set up with a simpler, flatter style.  This type of style might be more desirable in some situations.</p>"
        "<p>Simple expand and collapse buttons have been added on the right side of the caption bars.  When those buttons are present, it is necessary to click on the buttons to open or close a page.  You can however change that behavior so that any click on the caption bar will toggle even when the button is present by changing the accordion’s settings if you want.  A double click at any place on the caption bar will also open or close the page.</p>"
        "<p>The events generated by opening and closing this accordion's pages are logged in the text control below.  You can prevent certain pages from opening or closing with the check boxes in the lower left of this page.</p>"
        ;
    setHTMLwin( m_htmlWin, s, m_accordion->GetBackgroundColour() );

	m_splitter2->SplitVertically( m_accordion, m_htmlWin, GetSize().GetWidth()*.25 );

    wxPanel* m_panel4 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4 = new wxBoxSizer( wxVERTICAL );

	wxStaticLine* m_staticline = new wxStaticLine( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer4->Add( m_staticline, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* bSizer6 = new wxBoxSizer( wxVERTICAL );

	page1CheckBox = new wxCheckBox( m_panel4, wxID_ANY, wxT("Page 1 can be opened"), wxDefaultPosition, wxDefaultSize, 0 );
	page1CheckBox->SetValue(true);
	bSizer6->Add( page1CheckBox, 0, wxALL, 5 );

	page2CheckBox = new wxCheckBox( m_panel4, wxID_ANY, wxT("Page 2 can be closed"), wxDefaultPosition, wxDefaultSize, 0 );
	page2CheckBox->SetValue(true);
	bSizer6->Add( page2CheckBox, 0, wxALL, 5 );

	page3CheckBox = new wxCheckBox( m_panel4, wxID_ANY, wxT("Page 3 can be closed"), wxDefaultPosition, wxDefaultSize, 0 );
	page3CheckBox->SetValue(true);
	bSizer6->Add( page3CheckBox, 0, wxALL, 5 );

	bSizer5->Add( bSizer6, 0, wxRIGHT, 5 );

	m_log2 = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER );
	bSizer5->Add( m_log2, 1, wxEXPAND, 5 );

    bSizer4->Add( bSizer5, 1, wxEXPAND, 5 );

	m_panel4->SetSizer( bSizer4 );
	m_panel4->Layout();

    int splitPos =
        m_notebook->GetPage(0)->GetSize().GetHeight()
        - bSizer4->GetMinSize().GetHeight()
        - m_splitter1->GetSashSize()
        + 1;

	m_splitter1->SplitHorizontally( m_splitter2, m_panel4, splitPos );
	m_splitter1->SetMinimumPaneSize( bSizer4->GetMinSize().GetHeight() );
	m_notebook->AddPage( m_splitter1, wxT("Sample 2"), false );

    m_accordion->Bind( wxEVT_ACCORDION_COLLAPSED,  &ExtendedSampleFrame::onAccordion2Collapsed,  this );
    m_accordion->Bind( wxEVT_ACCORDION_COLLAPSING, &ExtendedSampleFrame::onAccordion2Collapsing, this );
    m_accordion->Bind( wxEVT_ACCORDION_EXPANDED,   &ExtendedSampleFrame::onAccordion2Expanded,   this );
    m_accordion->Bind( wxEVT_ACCORDION_EXPANDING,  &ExtendedSampleFrame::onAccordion2Expanding,  this );
}

void ExtendedSampleFrame::onAccordion2Expanding(wxBookCtrlEvent& event)
{
    (*m_log2) << wxDateTime::Now().Format("%H:%M:%S: ");
    (*m_log2) << "wxEVT_ACCORDION_EXPANDING\t";
    (*m_log2) << "Selection:" << event.GetSelection() << "\t";
    (*m_log2) << "Old Selection:" << event.GetOldSelection() << "\n";

    if(event.GetSelection()==0 && !page1CheckBox->IsChecked())
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

    if(event.GetSelection()==1 && !page2CheckBox->IsChecked())
    {
        event.Veto();
        (*m_log2) << wxDateTime::Now().Format("%H:%M:%S: ");
        (*m_log2) << "wxEVT_ACCORDION_COLLAPSING event vetoed.\n";
    }

    if(event.GetSelection()==2 && !page3CheckBox->IsChecked())
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
	wxSplitterWindow* m_splitter1 = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_splitter1->SetSashGravity( 1 );

	wxSplitterWindow* m_splitter2 = new wxSplitterWindow( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    wxImageList* il = new wxImageList(16,15);
    il->Add(wxBitmap(home_xpm));
    il->Add(wxBitmap(repview_xpm));
    il->Add(wxBitmap(htmsidep_xpm));
    il->Add(wxBitmap(listview_xpm));

    m_accordion3 = new wxAccordion(m_splitter2);
    m_accordion3->AssignImageList(il);
    m_accordion3->SetAccordionPadding(0);

    m_accordion3->SetCollapsedColour1( wxColour(129,160,53) );
    m_accordion3->SetCollapsedColour2( wxColour(129,160,53) );
    m_accordion3->AddCollapsedGradientStop( wxColour(217,240,160), 0.25f);
    m_accordion3->AddCollapsedGradientStop( wxColour(171,200,100), 0.70f);
    m_accordion3->SetCollapsedBorderColour( wxColour(57,80,0) );
    m_accordion3->SetCollapsedTextColour( wxColour(57,80,0) );

    m_accordion3->SetExpandedColour1( wxColour(170,167,57) );
    m_accordion3->SetExpandedColour2( wxColour(170,167,57) );
    m_accordion3->AddExpandedGradientStop( wxColour(255,253,170), 0.25f);
    m_accordion3->AddExpandedGradientStop( wxColour(212,210,106), 0.75f);
    m_accordion3->SetExpandedBorderColour( wxColour(85,83,0) );
    m_accordion3->SetExpandedTextColour( wxColour(85,83,0) );

    m_accordion3->SetExpandButton( wxBitmap(expand03_xpm) );
    m_accordion3->SetCollapseButton( wxBitmap(collapse03_xpm) );
    m_accordion3->SetButtonMargin( wxSize(8,0) );

    m_accordion3->AddPage( new wxPanel(m_accordion3), "Page 1", true,  0 );
    m_accordion3->AddPage( new wxPanel(m_accordion3), "Page 2", false, 1 );
    m_accordion3->AddPage( new wxPanel(m_accordion3), "Page 3", false, 2 );
    m_accordion3->AddPage( new wxPanel(m_accordion3), "Page 4", false, 3 );

    wxHtmlWindow* m_htmlWin = new wxHtmlWindow( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    m_htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>In this sample, the caption bar’s gradient stops have been used to create a double transition effect.  The caption bar’s icons are also shown.</p>"
        "<p>When you left click on the accordion, a hit test will be performed, and the result will be logged in the text control below.  You can use the first check box below to block mouse clicks from reaching the accordion after the hit test is performed.  You can use the other 2 check boxes to toggle the caption bar’s icons and buttons.</p>";
        ;
    setHTMLwin( m_htmlWin, s, m_accordion3->GetBackgroundColour() );

	m_splitter2->SplitVertically( m_accordion3, m_htmlWin, GetSize().GetWidth()*.25 );

    wxPanel* m_panel1 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxStaticLine* m_staticline = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* bSizer3 = new wxBoxSizer( wxVERTICAL );

	blockMouseCheckBox =  new wxCheckBox( m_panel1, wxID_ANY, wxT("Block mouse clicks"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( blockMouseCheckBox, 0, wxALL, 5 );

	wxCheckBox* m_checkBox2 = new wxCheckBox( m_panel1, id_SHOWICONSCHECK, wxT("Show icons"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox2->SetValue(true);
	bSizer3->Add( m_checkBox2, 0, wxALL, 5 );

	wxCheckBox* m_checkBox3 = new wxCheckBox( m_panel1, id_SHOWBUTTONSCHECK, wxT("Show buttons"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox3->SetValue(true);
	bSizer3->Add( m_checkBox3, 0, wxALL, 5 );

	bSizer2->Add( bSizer3, 0, wxRIGHT, 5 );

	m_log3 = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER );
	bSizer2->Add( m_log3, 1, wxEXPAND, 5 );

    bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

	m_panel1->SetSizer( bSizer1 );
	m_panel1->Layout();

    int splitPos =
        m_notebook->GetPage(0)->GetSize().GetHeight()
        - bSizer1->GetMinSize().GetHeight()
        - m_splitter1->GetSashSize()
        + 1;

	m_splitter1->SplitHorizontally( m_splitter2, m_panel1, splitPos );
	m_splitter1->SetMinimumPaneSize( bSizer1->GetMinSize().GetHeight() );
	m_notebook->AddPage( m_splitter1, wxT("Sample 3"), false );

    m_accordion3->Bind( wxEVT_LEFT_UP,     &ExtendedSampleFrame::onAccordion3MouseLeftUp, this );
    m_accordion3->Bind( wxEVT_LEFT_DCLICK, &ExtendedSampleFrame::onAccordion3MouseLeftDC, this );

    m_accordion3->GetPage(0)->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion3PageMouseLeftUp, this );
    m_accordion3->GetPage(1)->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion3PageMouseLeftUp, this );
    m_accordion3->GetPage(2)->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion3PageMouseLeftUp, this );
    m_accordion3->GetPage(3)->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion3PageMouseLeftUp, this );

	m_checkBox2->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,  &ExtendedSampleFrame::onChecks , this );
	m_checkBox3->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,  &ExtendedSampleFrame::onChecks , this );
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


    if( !blockMouseCheckBox->IsChecked() )
    {
        event.Skip();
    }
}

void ExtendedSampleFrame::onAccordion3MouseLeftDC( wxMouseEvent& event )
{
    if( !blockMouseCheckBox->IsChecked() )
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
	wxSplitterWindow* m_splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    wxAccordion* m_accordion = new wxAccordion( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxACCORDION_SINGLE_FOLD );

    m_accordion->SetAccordionPadding(2);
    m_accordion->SetCollapsedBGBitmap( wxBitmap(back04_xpm) );
    m_accordion->SetCollapsedBorderColour(wxTransparentColour);
    m_accordion->SetCollapsedTextColour( wxColour(64,0,128) );
    m_accordion->SetCollapsedFont( GetFont().MakeBold() );

    m_accordion->SetExpandedBGBitmap( wxBitmap(back04_xpm) );
    m_accordion->SetExpandedBorderColour(wxTransparentColour);
    m_accordion->SetExpandedTextColour( wxColour(64,0,128) );
    m_accordion->SetExpandedFont( GetFont().MakeBold() );

    m_accordion->SetUseHighlighting(true);
    m_accordion->SetCollapsedHLBGBitmap( wxBitmap(back04hl_xpm) );
    m_accordion->SetCollapsedHLBorderColour(wxTransparentColour);
    m_accordion->SetCollapsedHLTextColour( *wxWHITE );
    m_accordion->SetCollapsedHLFont( GetFont().MakeBold() );

    m_accordion->SetExpandedHLBGBitmap( wxBitmap(back04hl_xpm) );
    m_accordion->SetExpandedHLBorderColour(wxTransparentColour);
    m_accordion->SetExpandedHLFont( GetFont().MakeBold() );

    m_accordion->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 1",false);
    m_accordion->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 2",false);
    m_accordion->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 3",false);
    m_accordion->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 4",true);

    wxHtmlWindow* m_htmlWin = new wxHtmlWindow( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    m_htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>In this sample, an image file is used for the caption bar background.</p>"
        "<p>This sample also uses highlighting so that the style changes when the mouse is over a caption bar.</p>"
        "<p>And finally, this sample demonstrates the wxACCORDION_SINGLE_FOLD style.  When a page is expanded, all other pages are collapsed. </p>"
        ;
    setHTMLwin( m_htmlWin, s, m_accordion->GetBackgroundColour() );

	m_splitter->SplitVertically( m_accordion, m_htmlWin, GetSize().GetWidth()*.25 );

	m_notebook->AddPage( m_splitter, wxT("Sample 4"), false );
}

void ExtendedSampleFrame::addSample5()
{
	wxSplitterWindow* m_splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    m_accordion5 = new wxAccordion( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxACCORDION_SINGLE_FOLD );

    m_accordion5->SetCaptionGradientAngle(90);
    m_accordion5->SetCollapsedColour1( wxColour(64,128,128) );
    m_accordion5->SetCollapsedColour2( wxColour(157,185,235) );
    m_accordion5->SetCollapsedBorderColour(wxTransparentColour);
    m_accordion5->SetCollapsedTextColour( *wxWHITE );
    m_accordion5->SetExpandedColour1( wxColour(64,128,128) );
    m_accordion5->SetExpandedColour2( wxColour(157,185,235) );
    m_accordion5->SetExpandedBorderColour(wxTransparentColour);
    m_accordion5->SetExpandedTextColour( *wxWHITE );

    m_accordion5->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 1",true);
    m_accordion5->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 2",false);
    m_accordion5->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 3",false);
    m_accordion5->AddPage( new wxTextCtrl(this,wxID_ANY),"Page 4",false);

    wxHtmlWindow* m_htmlWin = new wxHtmlWindow( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    m_htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>In this sample, the gradient angle was set to  90 degrees so that the gradient runs from left to right instead of top to bottom.</p>"
        "<p>This sample also tracks mouse cursor movements and opens a page whenever the mouse moves over a caption bar.  This type of accordion is sometimes called a “roll-over accordion”.</p>"
        ;
    setHTMLwin( m_htmlWin, s, m_accordion5->GetBackgroundColour() );

	m_splitter->SplitVertically( m_accordion5, m_htmlWin, GetSize().GetWidth()*.25 );

	m_notebook->AddPage( m_splitter, wxT("Sample 5"), false );
	lastFound=wxNOT_FOUND;

    m_accordion5->Bind(wxEVT_ACCORDION_COLLAPSING, &ExtendedSampleFrame::block, this );
    m_accordion5->Bind(wxEVT_ACCORDION_EXPANDING, &ExtendedSampleFrame::block, this );
	m_accordion5->Bind( wxEVT_MOTION, &ExtendedSampleFrame::onAccordion5Mouse, this );
}

void ExtendedSampleFrame::onAccordion5Mouse( wxMouseEvent& event )
{
    int result = m_accordion5->HitTest( wxPoint( event.GetX(), event.GetY() ) );

    if( result!=wxNOT_FOUND && result!=lastFound )
    {
        m_accordion5->Collapse(lastFound);
        m_accordion5->Expand(result);
        lastFound=result;
    }
}

void ExtendedSampleFrame::addSample6()
{
	wxSplitterWindow* m_splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE );
	m_splitter->SetBackgroundStyle(wxBG_STYLE_PAINT);

    wxAccordion* m_accordion = new wxAccordion( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxACCORDION_BUTTON_LEFT|wxACCORDION_COLLAPSE_TO_BOTTOM );
    m_accordion->SetBackgroundColour( *wxWHITE );

    m_accordion->SetAccordionPadding(1);
    m_accordion->SetMinCaptionHeight(28);
    m_accordion->SetCaptionRadius(10);
    m_accordion->SetCaptionGradientAngle(2);
    m_accordion->SetExpandButton( wxBitmap(expand06_xpm) );
    m_accordion->SetCollapseButton( wxBitmap(collapse06_xpm) );
    m_accordion->SetCollapsedColour1( wxColour(182,9,0) );
    m_accordion->SetCollapsedColour2( wxColour(160,8,0) );
    m_accordion->AddCollapsedGradientStop( wxColour(182,9,0), 0.49f);
    m_accordion->AddCollapsedGradientStop( wxColour(160,8,0), 0.51f);
    m_accordion->SetCollapsedBorderColour( wxColour(142,10,0) );
    m_accordion->SetCollapsedTextColour( wxColour(255,255,255) );
    m_accordion->SetCollapsedFont( GetFont().Bold() );
    m_accordion->SetExpandedColour1( wxColour(255,255,255) );
    m_accordion->SetExpandedColour2( wxColour(254,254,254) );
    m_accordion->SetExpandedBorderColour( wxColour(142,10,0) );
    m_accordion->SetExpandedTextColour( wxColour(142,10,0) );
    m_accordion->SetExpandedFont( GetFont().Bold() );
    m_accordion->SetPageRadius(10);
    m_accordion->SetPageColour1( wxColour(254,248,230) );
    m_accordion->SetPageColour2( wxColour(254,238,189) );
    m_accordion->SetPageBorderColour( wxColour(142,10,0) );

    m_accordion->AddPage( new wxTextCtrl(m_accordion,wxID_ANY),"Page 1",false);
    m_accordion->AddPage( new wxButton( m_accordion, wxID_ANY, wxT("A Button") ), "Page 2 (Fixed Width)", false);
    m_accordion->AddPage( new wxButton( m_accordion, wxID_ANY, wxT("A Button") ), "Page 3 (Fixed Height)", false);
    m_accordion->AddPage( new wxButton( m_accordion, wxID_ANY, wxT("A Button") ), "Page 4 (Fixed Size)", false);

    m_accordion->SetFixedWidth(1);
    m_accordion->SetFixedHeight(2);
    m_accordion->SetFixedSize(3);

    wxHtmlWindow* m_htmlWin = new wxHtmlWindow( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    m_htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
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
    setHTMLwin( m_htmlWin, s, m_accordion->GetBackgroundColour() );

	m_splitter->SplitVertically( m_accordion, m_htmlWin, GetSize().GetWidth()*.25 );
	m_notebook->AddPage( m_splitter, wxT("Sample 6"), false );

    m_splitter->Bind(wxEVT_PAINT,&ExtendedSampleFrame::onSplitterPaint,this);
    m_splitter->Bind(wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED,&ExtendedSampleFrame::onSplitterSash,this);
}

void ExtendedSampleFrame::addSample7()
{
	wxSplitterWindow* m_splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE );
	m_splitter->SetBackgroundStyle(wxBG_STYLE_PAINT);

    wxPanel* m_panel = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel->SetBackgroundColour( *wxBLACK );

	wxBoxSizer* bSizerH = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* bSizerV = new wxBoxSizer( wxVERTICAL );

    wxStaticText* m_staticText = new wxStaticText( m_panel, wxID_ANY, wxT("My Accordion"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText->SetFont( GetFont().MakeBold() );
	m_staticText->SetForegroundColour( *wxWHITE );
    bSizerV->Add( m_staticText, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxRIGHT|wxLEFT, 5 );

    wxDelegateRendererNative rn = wxRendererNative::Get();
    wxSize sz = rn.GetCheckBoxSize(NULL);

    wxImageList* m_ImageList = new wxImageList(sz.GetWidth(), sz.GetHeight());
    wxBitmap unchecked  = wxBitmap(sz);
    wxBitmap checked  = wxBitmap(sz);

    wxMemoryDC temp_dc;
    temp_dc.SelectObject(unchecked);
    rn.DrawCheckBox(m_panel, temp_dc, wxRect(0,0,sz.GetWidth(),sz.GetHeight()),0 );
    temp_dc.SelectObject(checked);
    rn.DrawCheckBox(m_panel, temp_dc, wxRect(0,0,sz.GetWidth(),sz.GetHeight()),wxCONTROL_CHECKED );
    temp_dc.SelectObject(wxNullBitmap);

    m_ImageList->Add( wxBitmap(unchecked) );
    m_ImageList->Add( wxBitmap(checked) );

    wxAccordion* m_accordion = new wxAccordion( m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxACCORDION_FLOAT_TO_TOP );
    m_accordion->AssignImageList(m_ImageList);
    m_accordion->SetAccordionPadding(1);
    m_accordion->SetDisabledBrightness(150);

    m_accordion->SetCollapsedColour1( wxColour(50,50,50) );
    m_accordion->SetCollapsedColour2( wxColour(50,50,50) );
    m_accordion->SetCollapsedBorderColour( wxTransparentColor );
    m_accordion->SetCollapsedTextColour( *wxWHITE );

    m_accordion->SetExpandedColour1( wxColour(50,50,50) );
    m_accordion->SetExpandedColour2( wxColour(50,50,50) );
    m_accordion->SetExpandedBorderColour( wxTransparentColor );
    m_accordion->SetExpandedTextColour( *wxWHITE );

    m_accordion->SetPageColour1( *wxBLACK );
    m_accordion->SetPageColour2( *wxBLACK );
    m_accordion->SetPageBorderColour( wxTransparentColor );

    m_accordion->SetCollapseButton( wxBitmap(collapse07_xpm) );
    m_accordion->SetExpandButton( wxBitmap(expand07_xpm) );

    wxTextCtrl* m_textCtrl;
    m_textCtrl = new wxTextCtrl( m_accordion, wxID_ANY );
	m_textCtrl->SetForegroundColour( *wxWHITE );
	m_textCtrl->SetBackgroundColour( *wxBLACK );
    m_accordion->AddPage( m_textCtrl, "Page 1", false, 1);

    m_textCtrl = new wxTextCtrl( m_accordion, wxID_ANY );
	m_textCtrl->SetForegroundColour( *wxWHITE );
	m_textCtrl->SetBackgroundColour( *wxBLACK  );
    m_accordion->AddPage( m_textCtrl, "Page 2", false, 1);

    m_textCtrl = new wxTextCtrl( m_accordion, wxID_ANY );
	m_textCtrl->SetForegroundColour( *wxWHITE );
	m_textCtrl->SetBackgroundColour( *wxBLACK  );
    m_accordion->AddPage( m_textCtrl, "Page 3", false, 1);

    bSizerV->Add( m_accordion, 1, wxEXPAND, 5 );
    bSizerH->Add( bSizerV, 1, wxEXPAND, 5 );

    wxPanel* m_RightBorder = new wxPanel( m_panel, wxID_ANY, wxDefaultPosition, wxSize( 1,-1 ), wxTAB_TRAVERSAL );
	m_RightBorder->SetBackgroundColour( wxColour( 71, 71, 71 ) );
    bSizerH->Add( m_RightBorder, 0, wxEXPAND, 5 );
    m_panel->SetSizer( bSizerH );
	m_panel->Layout();

    wxHtmlWindow* m_htmlWin = new wxHtmlWindow( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    m_htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
        "<p>This sample shows an accordion styled to look like a control on the android platform.  </p>"
        "<p>This sample also demonstrates the wxACCORDION_FLOAT_TO_TOP style.  When a page is expanded, it is moved towards the top of the accordion. </p>"
        "<p>This accordion also uses the caption bar's icons to simulate check boxes.  Clicking on a check box will toggle the panel between enabled and disabled states. </p>"
        ;
    setHTMLwin( m_htmlWin, s, m_accordion->GetBackgroundColour(), *wxWHITE );

	m_splitter->SplitVertically( m_panel, m_htmlWin, GetSize().GetWidth()*.25 );
	m_notebook->AddPage( m_splitter, wxT("Sample 7"), false );

    m_accordion->Bind( wxEVT_LEFT_UP, &ExtendedSampleFrame::onAccordion7MouseLeftUp, this );
    m_splitter->Bind(wxEVT_PAINT,&ExtendedSampleFrame::onSplitterPaint,this);
    m_splitter->Bind(wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED,&ExtendedSampleFrame::onSplitterSash,this);
}

void ExtendedSampleFrame::onAccordion7MouseLeftUp( wxMouseEvent& event )
{
    if( wxAccordion* m_accordion = wxDynamicCast( event.GetEventObject(), wxAccordion) )
    {
        long flags(0);
        int res = m_accordion->HitTest(wxPoint(event.GetX(),event.GetY()),&flags);;

        if(flags & wxACCORDION_HITTEST_ONCAPTIONICON)
        {
            int ic = m_accordion->GetPageImage(res);

            if(ic==0)
            {
                m_accordion->SetPageImage(res,1);
                m_accordion->Enable(res);
            }
            else
            {
                m_accordion->SetPageImage(res,0);
                m_accordion->Disable(res);
            }
        }
        else
        {
            event.Skip();
        }
    }
}

void styleSubAccordion( wxAccordion* m_accordion, int level, bool useImageList)
{
    wxImageList* il = new wxImageList(13,13);

    if(useImageList)
    {
        il->Add(wxBitmap(subempty_xpm));
        il->Add(wxBitmap(subcollapse_xpm));
        il->Add(wxBitmap(subexpand_xpm));
    }

    m_accordion->AssignImageList(il);
    m_accordion->SetIconMargin( wxSize(5 +13*(level-1),0) );

    m_accordion->SetPagePadding(0);
    m_accordion->SetAccordionPadding(0);

    m_accordion->SetUseHighlighting(true);
    m_accordion->SetCollapsedColour1( wxColour(255,255,255) );
    m_accordion->SetCollapsedColour2( wxColour(255,255,255) );
    m_accordion->SetCollapsedBorderColour(wxTransparentColour);
    m_accordion->SetCollapsedTextColour( wxColour(59,59,59) );
    m_accordion->SetCollapsedHLColour1( wxColour(59,184,204) );
    m_accordion->SetCollapsedHLColour2( wxColour(59,184,204) );
    m_accordion->AddCollapsedHLGradientStop( wxColour(59,183,235), 0.01f);
    m_accordion->AddCollapsedHLGradientStop( wxColour(59,183,235), 0.99f);
    m_accordion->SetCollapsedHLBorderColour(wxTransparentColour);
    m_accordion->SetCollapsedHLTextColour( wxColour(59,59,59) );
    m_accordion->SetExpandedColour1( wxColour(255,255,255) );
    m_accordion->SetExpandedColour2( wxColour(255,255,255) );
    m_accordion->SetExpandedBorderColour(wxTransparentColour);
    m_accordion->SetExpandedTextColour( wxColour(59,59,59) );
    m_accordion->SetExpandedHLColour1( wxColour(59,184,204) );
    m_accordion->SetExpandedHLColour2( wxColour(59,184,204) );
    m_accordion->AddExpandedHLGradientStop( wxColour(59,183,235), 0.01f);
    m_accordion->AddExpandedHLGradientStop( wxColour(59,183,235), 0.99f);
    m_accordion->SetExpandedHLBorderColour(wxTransparentColour);
    m_accordion->SetExpandedHLTextColour( wxColour(59,59,59) );

    m_accordion->SetName("subaccordion");
}

void setSizes( wxAccordion* m_accordion)
{
    wxAccordion *curAccordion,*childAccordion;
    std::stack<wxAccordion*> myStack;
    bool canSetHeight(true);
    int height(0);

    myStack.push(m_accordion);

    while( !myStack.empty() )
    {
        height=0;
        canSetHeight=true;
        curAccordion = myStack.top();
        myStack.pop();

        for( size_t i=0; i<curAccordion->GetPageCount(); i++ )
        {
            if( childAccordion = wxDynamicCast(curAccordion->GetPage(i),wxAccordion) )
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

void setHTMLwin8(wxHtmlWindow* m_htmlWin, const wxString& s2, const wxColour& c )
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

   setHTMLwin( m_htmlWin, s, c );
}

void ExtendedSampleFrame::addSample8()
{
	wxSplitterWindow* m_splitter = new wxSplitterWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE );
	m_splitter->SetBackgroundStyle(wxBG_STYLE_PAINT);

	sample8LeftPanel = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sample8LeftPanel->SetBackgroundColour( wxColour( 240, 240, 240 ) );

	wxBoxSizer* bSizer = new wxBoxSizer( wxVERTICAL );

	wxPanel* m_border = new wxPanel( sample8LeftPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_border->SetBackgroundColour( wxColour( 128, 128, 128 ) );

	wxBoxSizer* borderSizer = new wxBoxSizer( wxHORIZONTAL );

	wxAccordion* m_accordion = new wxAccordion(m_border);

    m_accordion->SetPagePadding(0);
    m_accordion->SetAccordionPadding(0);
    m_accordion->SetCollapseButton(collapse08_xpm);
    m_accordion->SetExpandButton(expand08_xpm);
    m_accordion->SetButtonMargin( wxSize(5,0) );
    m_accordion->SetAccordionPadding(0);
    m_accordion->SetOnlyToggleWithButton(false);
    m_accordion->SetCollapsedColour1( wxColour(115,115,115) );
    m_accordion->SetCollapsedColour2( wxColour(82,82,82) );
    m_accordion->SetCollapsedBorderColour(wxTransparentColour);
    m_accordion->SetCollapsedTextColour( wxColour(255,255,255) );
    m_accordion->SetExpandedColour1( wxColour(115,115,115) );
    m_accordion->SetExpandedColour2( wxColour(82,82,82) );
    m_accordion->SetExpandedBorderColour(wxTransparentColour);

	wxAccordion* m_subaccordion1 = new wxAccordion(m_accordion);
	styleSubAccordion( m_subaccordion1, 1, true );

	wxAccordion* m_subaccordion2 = new wxAccordion(m_accordion);
	styleSubAccordion( m_subaccordion2, 1, false );

	wxAccordion* m_subaccordion3 = new wxAccordion(m_accordion);
	styleSubAccordion( m_subaccordion3, 2, false );

    m_subaccordion3->AddPage( new wxPanel( m_subaccordion3, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ), "Subitem 1-1-1",false,0);
    m_subaccordion3->AddPage( new wxPanel( m_subaccordion3, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ), "Subitem 1-1-2",false,0);

    m_subaccordion2->AddPage( new wxPanel( m_subaccordion2, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ),"Subitem 2-1",false,0);
    m_subaccordion2->AddPage( new wxPanel( m_subaccordion2, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ), "Subitem 2-2",false,0);

    m_subaccordion1->AddPage( m_subaccordion3 ,"Subitem 1-1",false,2);
    m_subaccordion1->AddPage( new wxPanel( m_subaccordion1, wxID_ANY, wxDefaultPosition, wxSize( -1,0 ), wxTAB_TRAVERSAL ),"Subitem 1-2",false,0);

    m_accordion->AddPage(m_subaccordion1,"Item 1",true);
    m_accordion->AddPage(m_subaccordion2,"Item 2",true);

    m_subaccordion3->SetFixedHeight(0);
    m_subaccordion3->SetFixedHeight(1);
    m_subaccordion1->SetFixedHeight(0);
    m_subaccordion1->SetFixedHeight(1);
    m_subaccordion2->SetFixedHeight(0);
    m_subaccordion2->SetFixedHeight(1);
    m_accordion->SetFixedHeight(0);
    m_accordion->SetFixedHeight(1);

    setSizes( m_accordion);

	borderSizer->Add( m_accordion, 1, wxEXPAND|wxALL, 1 );
	m_border->SetSizer( borderSizer );
	m_border->Layout();
	borderSizer->Fit( m_border );

	bSizer->Add( m_border, 0, wxALL|wxEXPAND, 5 );
	sample8LeftPanel->SetSizer( bSizer );
	sample8LeftPanel->Layout();

    m_htmlWin8 = new wxHtmlWindow( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_NEVER );
    m_htmlWin8->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    setHTMLwin8( m_htmlWin8, "" , sample8LeftPanel->GetBackgroundColour() );

	m_splitter->SplitVertically( sample8LeftPanel, m_htmlWin8, GetSize().GetWidth()*.25 );
	m_notebook->AddPage( m_splitter, wxT("Sample 8"), false );

    m_splitter->Bind( wxEVT_PAINT, &ExtendedSampleFrame::onSplitterPaint, this );
    m_splitter->Bind( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, &ExtendedSampleFrame::onSplitterSash, this );
    m_accordion->Bind( wxEVT_ACCORDION_COLLAPSED, &ExtendedSampleFrame::onAccordion8, this );
    m_accordion->Bind( wxEVT_ACCORDION_EXPANDED, &ExtendedSampleFrame::onAccordion8, this );
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

            sample8LeftPanel->Layout();
            message << (collapsed?" was collapsed.":" was expanded.");
        }
        else
        {
            message << " was selected.";
        }

        setHTMLwin8( m_htmlWin8, message, sample8LeftPanel->GetBackgroundColour() );
    }
}
