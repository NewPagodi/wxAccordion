/////////////////////////////////////////////////////////////////////////////
// Name:        simplesample.cpp
// Purpose:     wxAccordion Simple Sample
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
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/radiobut.h>
#endif

#include <wx/accordion/accordion.h>

#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "bitmaps/sample.xpm"
#endif

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

	private:
		wxAccordion* m_accordion;
};


bool SimpleSampleApp::OnInit()
{
    SimpleSampleFrame* frame = new SimpleSampleFrame(0L);
    frame->SetIcon(wxICON(sample));
    frame->Show();

    return true;
}

wxIMPLEMENT_APP(SimpleSampleApp);

SimpleSampleFrame::SimpleSampleFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	//add a panel to serve as the frame's background
	wxPanel* panel1 = new wxPanel( this );

	//create the accordion and set a few style options
	m_accordion = new wxAccordion( panel1 );

	m_accordion->SetAccordionPadding(1);
	m_accordion->SetCaptionRadius(3);
	m_accordion->SetCaptionBorderStyle(wxAC_BORDERS_NO_BOTTOM_WHEN_EXPANDED);

	m_accordion->GetCollapsedStyle().SetColour1( wxColour(46,25,164) );
	m_accordion->GetCollapsedStyle().SetColour2( wxColour(12,22,144) );
	m_accordion->GetCollapsedStyle().SetBorderColour( wxColour(40,10,134) );
	m_accordion->GetCollapsedStyle().SetTextColour( wxColour(225,219,255) );

    m_accordion->GetExpandedStyle().SetColour1( wxColour(248,246,253) );
    m_accordion->GetExpandedStyle().SetColour2( wxColour(246,244,250) );
    m_accordion->GetExpandedStyle().SetBorderColour( wxColour(12,22,144) );
    m_accordion->GetExpandedStyle().SetTextColour( wxColour(40,10,134) );

	m_accordion->SetPageRadius(3);
	m_accordion->GetPageStyle().SetBorderColour( wxColour(12,22,144) );

    //Add a text control to the accordion
	m_accordion->AddPage( new wxTextCtrl( m_accordion, wxID_ANY), wxT("A Text Control"), false );

    //add a panel containing a button to the accordion
	wxPanel* panel2 = new wxPanel( m_accordion );
	wxButton* button1 = new wxButton( panel2, wxID_ANY, wxT("MyButton") );
	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );
	bSizer1->Add( button1, 0, wxALL, 5 );
	panel2->SetSizer( bSizer1 );

	m_accordion->AddPage( panel2, wxT("A Button"), false );

    //add a list box to the accordion
	wxListBox* listBox = new wxListBox( m_accordion, wxID_ANY );
	listBox->Append( wxT("Item 1") );
	listBox->Append( wxT("Item 2") );
	listBox->Append( wxT("Item 3") );
	m_accordion->AddPage( listBox, wxT("A Listbox"), false );

	//add a panel with 3 radio buttons to the accordion
	wxPanel* panel3 = new wxPanel( m_accordion );
	wxRadioButton* radioBtn1 = new wxRadioButton( panel3, wxID_ANY, wxT("Option 1") );
	radioBtn1->SetValue( true );
	wxRadioButton* radioBtn2 = new wxRadioButton( panel3, wxID_ANY, wxT("Option 2") );
	wxRadioButton* radioBtn3 = new wxRadioButton( panel3, wxID_ANY, wxT("Option 3") );
	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );
	bSizer2->Add( radioBtn1, 0, wxALL, 5 );
	bSizer2->Add( radioBtn2, 0, wxALL, 5 );
	bSizer2->Add( radioBtn3, 0, wxALL, 5 );
	panel3->SetSizer( bSizer2 );
	m_accordion->AddPage(panel3,wxT("Some Radio Buttons"),false,1);

	//finish setting up the frame
	wxBoxSizer* bSizer3 = new wxBoxSizer( wxVERTICAL );
	bSizer3->Add( m_accordion, 1, wxALL|wxEXPAND, 5 );
	panel1->SetSizer( bSizer3 );
	wxBoxSizer* bSizer4 = new wxBoxSizer( wxVERTICAL );
	bSizer4->Add( panel1, 1, wxEXPAND, 5 );
	this->SetSizer( bSizer4 );
	this->Layout();

	//center the frame on the screen
	this->Centre( wxBOTH );
}

SimpleSampleFrame::~SimpleSampleFrame()
{
}
