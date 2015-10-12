/***************************************************************
 * Name:      SimpleSample.cpp
 * Purpose:   wxAccordion Simple Sample
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
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/radiobut.h>

class SimpleSampleApp : public wxApp
{
    public:
        virtual bool OnInit();
};

class SimpleSampleFrame : public wxFrame
{
	public:
		SimpleSampleFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxAccordion Demo"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 250,400 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~SimpleSampleFrame();

	protected:
		void OnClose( wxCloseEvent& event ) { Destroy(); }
		wxAccordion* m_accordion;

	private:
};


bool SimpleSampleApp::OnInit()
{
    SimpleSampleFrame* frame = new SimpleSampleFrame(0L);
    frame->SetIcon(wxICON(aaaa));
    frame->Show();

    return true;
}

IMPLEMENT_APP(SimpleSampleApp);

SimpleSampleFrame::SimpleSampleFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	//add a panel to be the frame's background, and create a sizer for that panel
	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );
	wxPanel* m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );

	//create the accordion and set a few style options
	m_accordion = new wxAccordion( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxACCORDION_DEFAULT_STYLE );

	m_accordion->SetAccordionPadding(1);
	m_accordion->SetCaptionRadius(3);
	m_accordion->SetExpandedBottomBorder(false);
	m_accordion->SetCollapsedColour1( wxColour(46,25,164) );
	m_accordion->SetCollapsedColour2( wxColour(12,22,144) );
	m_accordion->SetCollapsedBorderColour( wxColour(40,10,134) );
	m_accordion->SetCollapsedTextColour( wxColour(225,219,255) );
	m_accordion->SetExpandedColour1( wxColour(248,246,253) );
	m_accordion->SetExpandedColour2( wxColour(246,244,250) );
	m_accordion->SetExpandedBorderColour( wxColour(12,22,144) );
	m_accordion->SetExpandedTextColour( wxColour(40,10,134) );
	m_accordion->SetPageRadius(3);
	m_accordion->SetPageBorderColour( wxColour(12,22,144) );

    //Add a text control to the accordion
	m_accordion->AddPage(new wxTextCtrl( m_panel1, wxID_ANY),wxT("A Text Control"),false);

    //add a panel containing a button to the accordion
	wxPanel* m_panel2 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3 = new wxBoxSizer( wxVERTICAL );
	wxButton* m_button1 = new wxButton( m_panel2, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button1, 0, wxALL, 5 );
	m_panel2->SetSizer( bSizer3 );

	m_accordion->AddPage(m_panel2,wxT("A Button"),false,1);

    //add a list box to the accordion
	wxListBox* m_listBox1 = new wxListBox( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_listBox1->Append( wxT("Item 1") );
	m_listBox1->Append( wxT("Item 2") );
	m_listBox1->Append( wxT("Item 3") );

	m_accordion->AddPage(m_listBox1,wxT("A Listbox"),false,1);

	//add a panel with 3 radio buttons to the accordion
	wxPanel* m_panel3 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4 = new wxBoxSizer( wxVERTICAL );
	wxRadioButton* m_radioBtn1 = new wxRadioButton( m_panel3, wxID_ANY, wxT("Option 1"), wxDefaultPosition, wxDefaultSize, 0 );
	wxRadioButton* m_radioBtn2 = new wxRadioButton( m_panel3, wxID_ANY, wxT("Option 2"), wxDefaultPosition, wxDefaultSize, 0 );
	wxRadioButton* m_radioBtn3 = new wxRadioButton( m_panel3, wxID_ANY, wxT("Option 3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_radioBtn1->SetValue( true );
	bSizer4->Add( m_radioBtn1, 0, wxALL, 5 );
	bSizer4->Add( m_radioBtn2, 0, wxALL, 5 );
	bSizer4->Add( m_radioBtn3, 0, wxALL, 5 );
	m_panel3->SetSizer( bSizer4 );

	m_accordion->AddPage(m_panel3,wxT("Some Radio Buttons"),false,1);

	//add the accordion to the panel's sizer
	bSizer2->Add( m_accordion, 1, wxALL|wxEXPAND, 5 );

	//finish setting up the frame
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer1 );
	this->Layout();

	this->Bind( wxEVT_CLOSE_WINDOW, &SimpleSampleFrame::OnClose, this );

	this->Centre( wxBOTH );
}

SimpleSampleFrame::~SimpleSampleFrame()
{
}
