/////////////////////////////////////////////////////////////////////////////
// Name:        builder.cpp
// Purpose:     wxAccordion Sample
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
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/dcmemory.h>
#endif

#include <wx/accordion/accordion.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/property.h>
#include <wx/propgrid/advprops.h>
#include <wx/html/htmlwin.h>
#include <wx/splitter.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>


///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuAbout 1001
#define wxID_CHECK1 1002
#define wxID_CHECK2 1003
#define wxID_CHECK3 1004
#define wxID_CHECK4 1005

// -----------------------------------------------------------------------
// wxSizeProperty - a custom property for the propgrid
// -----------------------------------------------------------------------
class wxSizeProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxSizeProperty)

    public:

        wxSizeProperty( const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
                        const wxSize& value = wxSize() ): wxPGProperty(label,name)
        {
            SetValueI(value);
            AddPrivateChild( new wxIntProperty(wxT("Width"),wxPG_LABEL,value.x) );
            AddPrivateChild( new wxIntProperty(wxT("Height"),wxPG_LABEL,value.y) );

            Item(0)->SetEditor( "SpinCtrl" );
            Item(0)->SetAttribute(  wxT("Step"), (long)1 );
            Item(0)->SetAttribute(  wxT("MotionSpin"), true );
            Item(0)->SetAttribute(  wxPG_ATTR_MIN, (long)0);

            Item(1)->SetEditor( "SpinCtrl" );
            Item(1)->SetAttribute(  wxT("Step"), (long)1 );
            Item(1)->SetAttribute(  wxT("MotionSpin"), true );
            Item(1)->SetAttribute(  wxPG_ATTR_MIN, (long)0);

            ChangeFlag( wxPG_PROP_READONLY, true );
        }

        virtual ~wxSizeProperty(){}

        virtual void RefreshChildren()
        {
            if ( !GetChildCount() ) return;
            const wxSize& size = wxSizeRefFromVariant(m_value);
            Item(0)->SetValue( (long)size.x );
            Item(1)->SetValue( (long)size.y );
        }

        virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
        {
            wxSize& size = wxSizeRefFromVariant(thisValue);
            int val = childValue.GetLong();
            switch ( childIndex )
            {
                case 0: size.x = val; break;
                case 1: size.y = val; break;
            }
            wxVariant newVariant;
            newVariant << size;
            return newVariant;
        }

    protected:
        void SetValueI( const wxSize& value ){m_value = WXVARIANT(value);}
};

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxSizeProperty,wxPGProperty, wxSize,const wxSize&,TextCtrl)

// -----------------------------------------------------------------------
// wxGradientStopProperty  - a custom property for the propgrid
// -----------------------------------------------------------------------
class GradientStop
{
    public:
        GradientStop():col(wxTransparentColour),pos(0){}
        GradientStop(const wxColour& c,int p):col(c),pos(p){}
        wxColour GetColour()const{return col;}
        int GetPosition()const{return pos;}
        void SetColour(const wxColour& c){col=c;}
        void SetPosition(int p){pos=p;}

    private:
        wxColour col;
        int pos;
};

bool operator == (const GradientStop& a, const GradientStop& b)
{
    return a.GetColour()==b.GetColour() && a.GetPosition()==b.GetPosition();
}

WX_PG_DECLARE_VARIANT_DATA(GradientStop)

class wxGradientStopProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxGradientStopProperty)

    public:
        wxGradientStopProperty( const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
                        const GradientStop& value = GradientStop() ) : wxPGProperty(label,name)
        {
            SetValueI(value);
            AddPrivateChild( new wxColourProperty(wxT("Colour"),wxPG_LABEL,value.GetColour() ) );
            AddPrivateChild( new wxIntProperty(wxT("Position*100"),wxPG_LABEL,value.GetPosition()) );

            Item(1)->SetEditor( "SpinCtrl" );
            Item(1)->SetAttribute(  wxT("Step"), (long)1 );
            Item(1)->SetAttribute(  wxT("MotionSpin"), true );
            Item(1)->SetAttribute(  wxPG_ATTR_MIN, (long)1);
            Item(1)->SetAttribute(  wxPG_ATTR_MAX, (long)99 );

            ChangeFlag( wxPG_PROP_READONLY, true );
        }

        virtual ~wxGradientStopProperty(){}

        virtual void RefreshChildren()
        {
            if ( !GetChildCount() ) return;
            const GradientStop& stop = GradientStopRefFromVariant(m_value);
            Item(0)->SetValue( wxVariant(stop.GetColour()) );
            Item(1)->SetValue( stop.GetPosition() );
        }

        virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
        {
            GradientStop& stop = GradientStopRefFromVariant(thisValue);
            switch ( childIndex )
            {
                case 0: stop.SetColour( childValue.GetAny().As<wxColour>() ); break;
                case 1: stop.SetPosition( childValue.GetDouble() ); break;
            }
            wxVariant newVariant;
            newVariant << stop;
            return newVariant;
        }

    protected:
        void SetValueI( const GradientStop& value ){m_value = WXVARIANT(value);}
};

WX_PG_IMPLEMENT_VARIANT_DATA(GradientStop)
WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGradientStopProperty, wxPGProperty, GradientStop, const GradientStop&, TextCtrl )


// -----------------------------------------------------------------------
// Helper Functions
// -----------------------------------------------------------------------
void addSizeString(wxString& s, const wxString& method, const wxSize& sz)
{
    s << "m_accordion->";
    s << method;
    s << "( wxSize(";
    s << sz.GetWidth();
    s << ",";
    s << sz.GetHeight();
    s << ") );\n";
}

void addBitmapString(wxString& s, const wxString& method, wxPGProperty* p)
{
    wxString s2 = p->GetValue().GetString();

    if(!s2.IsEmpty())
    {
        s << "m_accordion->";
        s << method;
        s << "( wxBitmap(\"";
        s2.Replace("\\","\\\\");
        s << s2;
        s << "\",wxBITMAP_TYPE_ANY) );\n";
    }
}

void addIntString(wxString& s, const wxString& method, int i)
{
    s << "m_accordion->";
    s << method;
    s << "(";
    s << i;
    s << ");\n";
}

void addGradientStopString(wxString& s, const wxString& method, wxPGProperty* p)
{
    const GradientStop& stop = GradientStopRefFromVariant(p->GetValue());

    wxColour c = stop.GetColour();
    float pos = static_cast<float>(stop.GetPosition())/100.0;

    s << "m_accordion->";
    s << method;
    s << "( wxColour(";
    s << (int) c.Red();
    s << ",";
    s << (int) c.Green();
    s << ",";
    s << (int) c.Blue();
    s << "), ";
    s << wxString::Format("%.2ff",pos);
    s << " );\n";
}

void addColourString(wxString& s, const wxString& method, const wxColour& c)
{
    s << "m_accordion->";
    s << method;
    s << "( wxColour(";
    s << (int) c.Red();
    s << ",";
    s << (int) c.Green();
    s << ",";
    s << (int) c.Blue();
    s << ") );\n";
}

void addFontString(wxString& s,const wxString& method, const wxFont& f)
{
//    wxFont(int pointSize,
//           wxFontFamily family,
//           wxFontStyle style,
//           wxFontWeight weight, bool underline=false,
//           const wxString &faceName=wxEmptyString,
//           wxFontEncoding encoding=wxFONTENCODING_DEFAULT)

    s << "m_accordion->";
    s << method;
    s << "( wxFont( ";
    s << f.GetPointSize();
    s << " , ";

    if( f.GetFamily()== wxFONTFAMILY_DECORATIVE )
    {
        s << "wxFONTFAMILY_DECORATIVE, ";
    }
    else if( f.GetFamily()== wxFONTFAMILY_ROMAN )
    {
        s << "wxFONTFAMILY_ROMAN, ";
    }
    else if( f.GetFamily()== wxFONTFAMILY_SCRIPT )
    {
        s << "wxFONTFAMILY_SCRIPT, ";
    }
    else if( f.GetFamily()== wxFONTFAMILY_SWISS )
    {
        s << "wxFONTFAMILY_SWISS, ";
    }
    else if( f.GetFamily()== wxFONTFAMILY_MODERN )
    {
        s << "wxFONTFAMILY_MODERN, ";
    }
    else if( f.GetFamily()== wxFONTFAMILY_TELETYPE )
    {
        s << "wxFONTFAMILY_TELETYPE, ";
    }
    else if( f.GetFamily()== wxFONTFAMILY_UNKNOWN )
    {
        s << "wxFONTFAMILY_UNKNOWN, ";
    }
    else
    {
        s << "wxFONTFAMILY_DEFAULT, ";
    }

    if( f.GetStyle()==wxFONTSTYLE_ITALIC )
    {
        s << "wxFONTSTYLE_ITALIC , ";
    }
    else if( f.GetStyle()==wxFONTSTYLE_SLANT)
    {
        s << "wxFONTSTYLE_SLANT, ";
    }
    else
    {
        s << "wxFONTSTYLE_NORMAL, ";
    }

    if( f.GetWeight()==wxFONTWEIGHT_LIGHT)
    {
        s << "wxFONTWEIGHT_LIGHT, ";
    }
    else if( f.GetWeight()==wxFONTWEIGHT_BOLD)
    {
        s << "wxFONTWEIGHT_BOLD, ";
    }
    else
    {
        s << "wxFONTWEIGHT_NORMAL, ";
    }

    if(f.GetUnderlined())
    {
        s << "true, ";
    }
    else
    {
        s << "false, \"";
    }

    s << f.GetFaceName();

    s << "\", wxFONTENCODING_DEFAULT ) );\n";
}

void setImageList(wxAccordion* accordion, const wxSize& sz, const wxColour& c )
{
    wxImageList* il = new wxImageList(sz.GetWidth(), sz.GetHeight());
    wxBitmap b(sz);
    wxMemoryDC myDC(b);

    myDC.SetBrush(c);
    myDC.SetPen(*wxTRANSPARENT_PEN);
    myDC.DrawRectangle(0,0,sz.GetWidth(), sz.GetHeight());
    myDC.SelectObject(wxNullBitmap);

    il->Add(b);
    accordion->AssignImageList(il);

    for(size_t i=0;i<accordion->GetPageCount();i++)
    {
        accordion->SetPageImage(i,0);
    }
}

int adjustStopPosition( wxPGProperty* stopProp, int pos)
{
    if( stopProp->GetChildCount()>0 )
    {
        long lastStopsPosition = stopProp->Item(stopProp->GetChildCount()-1)->Item(1)->GetValue().GetLong();

        if(lastStopsPosition>pos)
        {
            pos=lastStopsPosition;
        }
    }

    return pos;
}

void adjustStopBounds(wxPGProperty* stopProp)
{
    //This keeps the stops from getting out of order by setting
    //the mins and maxs so that a stop's position can not be
    //be set below the position of a preceeding stop or above
    //the position of a following stop.

    for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
    {
        if(i==0)
        {
            stopProp->Item(i)->Item(1)->SetAttribute(  wxPG_ATTR_MIN, (long) 0);
        }
        else
        {
            long val = stopProp->Item(i-1)->Item(1)->GetValue().GetLong();
            stopProp->Item(i)->Item(1)->SetAttribute(  wxPG_ATTR_MIN, val);
        }

        if(i==stopProp->GetChildCount()-1)
        {
            stopProp->Item(i)->Item(1)->SetAttribute(  wxPG_ATTR_MAX, (long) 99);
        }
        else
        {
            long val = stopProp->Item(i+1)->Item(1)->GetValue().GetLong();
            stopProp->Item(i)->Item(1)->SetAttribute(  wxPG_ATTR_MAX, val);
        }
    }
}


// -----------------------------------------------------------------------
// App and Frame declarations
// -----------------------------------------------------------------------
class BuilderApp : public wxApp
{
    public:
        virtual bool OnInit();
};

class BuilderFrame : public wxFrame
{
	public:
		BuilderFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxAccordion Builder"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxGetDisplaySize().Scale(0.75, 0.75), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~BuilderFrame();

	private:
	    void OnAbout( wxCommandEvent& ){wxMessageBox( wxT("the wxAccordion builder application."), wxT("Welcome to...") );}
	    void OnClose( wxCloseEvent& ) { Destroy(); }
        void OnQuit( wxCommandEvent& ){ Destroy(); }
        void onAddStop(wxCommandEvent& event);
        void onClearStops(wxCommandEvent& event);
        void onCheckBox(wxCommandEvent& event);
        void onReset(wxCommandEvent& event);
        void onGenerate(wxCommandEvent& event);
        void onPropertyGridChanged( wxPropertyGridEvent& event );

        void buildGrid();

        wxSplitterWindow* m_splitter;
		wxAccordion* m_accordion;
		wxPropertyGrid* m_propertyGrid;
		wxColourPickerCtrl* m_colourPicker;
		wxSpinCtrl* m_spinCtrl;
		wxListBox* m_listBox;
		wxTextCtrl* m_textCtrl;
		wxHtmlWindow* m_htmlWin;
		wxPanel* m_panel1;
		wxPanel* m_panel2;
		wxButton* m_resetButton;
		wxPanel* m_panel3;


        wxPGProperty* m_pgAccordionCat;
		wxPGProperty* m_pgAccordionStyle;
		wxPGProperty* m_pgAccordionPadding;
		wxPGProperty* m_pgAccordionOnlyToggleWithButton;
		wxPGProperty* m_pgAccordionBGProp;
		wxPGProperty* m_pgAccordionUseImage;
		wxPGProperty* m_pgAccordionImage;
		wxPGProperty* m_pgCaptionBarCat;
		wxPGProperty* m_pgCaptionMinHt;
		wxPGProperty* m_pgCaptionRadius;
		wxPGProperty* m_pgCaptionGradientAngle;
		wxPGProperty* m_pgCaptionExpandedBtmBrdr;
		wxPGProperty* m_pgCaptionTextPadding;
		wxPGProperty* m_pgCaptionUseHighlighting;
		wxPGProperty* m_pgCaptionCustomDisabled;
		wxPGProperty* m_pgCaptionTDisabledBrightness;
		wxPGProperty* m_pgButtonCat;
		wxPGProperty* m_pgUseButtons;
		wxPGProperty* m_pgExpButton;
		wxPGProperty* m_pgColButton;
		wxPGProperty* m_pgButtonPadding;
		wxPGProperty* m_pgIconCat;
		wxPGProperty* m_pgShowIcons;
		wxPGProperty* m_pgIconColour;
		wxPGProperty* m_pgIconSize;
		wxPGProperty* m_pgIconPadding;
		wxPGProperty* m_pgColCapBarCat;
		wxPGProperty* m_pgColCapBarCol1;
		wxPGProperty* m_pgColCapBarGradStops;
		wxPGProperty* m_pgColCapBarCol2;
		wxPGProperty* m_pgColCapBarUseImage;
		wxPGProperty* m_pgColCapBarImage;
		wxPGProperty* m_pgColCapBarBrdrTrans;
		wxPGProperty* m_pgColCapBarBrdrCol;
		wxPGProperty* m_pgColCapBarTextCol;
		wxPGProperty* m_pgColCapBarFont;
		wxPGProperty* m_pgExpCapBarCat;
		wxPGProperty* m_pgExpCapBarCol1;
		wxPGProperty* m_pgExpCapBarGradStops;
		wxPGProperty* m_pgExpCapBarCol2;
		wxPGProperty* m_pgExpCapBarUseImage;
		wxPGProperty* m_pgExpCapBarImage;
		wxPGProperty* m_pgExpCapBarBrdrTrans;
		wxPGProperty* m_pgExpCapBarBrdrCol;
		wxPGProperty* m_pgExpCapBarTextCol;
		wxPGProperty* m_pgExpCapBarFont;
		wxPGProperty* m_pgColHLCapBarCat;
		wxPGProperty* m_pgColHLCapBarCol1;
		wxPGProperty* m_pgColHLCapBarGradStops;
		wxPGProperty* m_pgColHLCapBarCol2;
		wxPGProperty* m_pgColHLCapBarUseImage;
		wxPGProperty* m_pgColHLCapBarImage;
		wxPGProperty* m_pgColHLCapBarBrdrTrans;
		wxPGProperty* m_pgColHLCapBarBrdrCol;
		wxPGProperty* m_pgColHLCapBarTextCol;
		wxPGProperty* m_pgColHLCapBarFont;
		wxPGProperty* m_pgExpHLCapBarCat;
		wxPGProperty* m_pgExpHLCapBarCol1;
		wxPGProperty* m_pgExpHLCapBarGradStops;
		wxPGProperty* m_pgExpHLCapBarCol2;
		wxPGProperty* m_pgExpHLCapBarUseImage;
		wxPGProperty* m_pgExpHLCapBarImage;
		wxPGProperty* m_pgExpHLCapBarBrdrTrans;
		wxPGProperty* m_pgExpHLCapBarBrdrCol;
		wxPGProperty* m_pgExpHLCapBarTextCol;
		wxPGProperty* m_pgExpHLCapBarFont;
		wxPGProperty* m_pgDisCapBarCat;
		wxPGProperty* m_pgDisCapBarCol1;
		wxPGProperty* m_pgDisCapBarGradStops;
		wxPGProperty* m_pgDisCapBarCol2;
		wxPGProperty* m_pgDisCapBarUseImage;
		wxPGProperty* m_pgDisCapBarImage;
		wxPGProperty* m_pgDisCapBarBrdrTrans;
		wxPGProperty* m_pgDisCapBarBrdrCol;
		wxPGProperty* m_pgDisCapBarTextCol;
		wxPGProperty* m_pgDisCapBarFont;
		wxPGProperty* m_pgPagesCat;
		wxPGProperty* m_pgPagesPadding;
		wxPGProperty* m_pgPagesRadius;
		wxPGProperty* m_pgPagesCol1;
		wxPGProperty* m_pgPagesGradStops;
		wxPGProperty* m_pgPagesCol2;
		wxPGProperty* m_pgPagesUseImage;
		wxPGProperty* m_pgPagesImage;
		wxPGProperty* m_pgPagesBrdrTrans;
		wxPGProperty* m_pgPagesBrdrCol;
};

// -----------------------------------------------------------------------
// BuilderApp
// -----------------------------------------------------------------------
bool BuilderApp::OnInit()
{
    wxInitAllImageHandlers();

    BuilderFrame* frame = new BuilderFrame(0L);
    frame->SetIcon(wxICON(aaaa));
    frame->Show();

    return true;
}

IMPLEMENT_APP(BuilderApp);


// -----------------------------------------------------------------------
// BuilderFrame
// -----------------------------------------------------------------------
BuilderFrame::BuilderFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	SetSizeHints( wxDefaultSize, wxDefaultSize );

	//create the menubar
	wxMenuBar* mbar = new wxMenuBar( 0 );
	wxMenu* fileMenu = new wxMenu();
	wxMenu* helpMenu = new wxMenu();
	wxMenuItem* menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	wxMenuItem* menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );
	helpMenu->Append( menuHelpAbout );
	mbar->Append( fileMenu, wxT("&File") );
	mbar->Append( helpMenu, wxT("&Help") );
	SetMenuBar( mbar );

	//create the splitter and the accordion
	m_splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_accordion = new wxAccordion(m_splitter);

	//create an HTMLWindow with the introduction and add it to the accordion
	m_htmlWin = new wxHtmlWindow( m_accordion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
    m_htmlWin->SetStandardFonts(GetFont().GetPointSize(), GetFont().GetFaceName(), GetFont().GetFaceName());
    wxString s = ""
    "<html><body>"
        "<ul><li>You can use the property grid on the right to change the properties of this accordion. "
        "<li>You can use the controls on the second page to add gradient stops to the property grid.</li>"
        "<li>There are check boxes on the third page that can enable or disable pages so that "
        "that the properties for the accordion's disabled state can be seen.</li>"
        "<li>There is a button on the forth page that will reset the grid and the accordion to their default states.</li>"
        "<li>If you find settings that you like, you can use the button on the fifth page to generate wxWidgets c++ "
        "code that will produce an accordion that uses those settings.</li></ul>"
    "</body></html>";
    m_htmlWin->SetPage(s);
    m_accordion->AddPage(m_htmlWin,"Introduction",true);

    //create a panel for working with gradient stops and add it to the accordion
	m_panel1 = new wxPanel( m_accordion, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_listBox = new wxListBox( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_NEEDED_SB );
	m_listBox->Append( wxT("Collapsed State") );
	m_listBox->Append( wxT("Collapsed Highlighted State") );
	m_listBox->Append( wxT("Expanded State") );
	m_listBox->Append( wxT("Expanded Highlighted State") );
	m_listBox->Append( wxT("Disabled State") );
	m_listBox->Append( wxT("Page") );
	m_listBox->SetSelection(0);
	m_colourPicker = new wxColourPickerCtrl( m_panel1, wxID_ANY, wxColour( 255, 255, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_spinCtrl = new wxSpinCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 99, 50 );
	wxButton* button1 = new wxButton( m_panel1, wxID_ANY, wxT("Clear Stops"), wxDefaultPosition, wxDefaultSize, 0 );
	wxButton* button2 = new wxButton( m_panel1, wxID_ANY, wxT("Add Stop"), wxDefaultPosition, wxDefaultSize, 0 );

	wxFlexGridSizer* fgSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer->Add( new wxStaticText( m_panel1, wxID_ANY, wxT("Stop")), 0, wxALIGN_RIGHT|wxTOP|wxBOTTOM|wxLEFT, 5 );
	fgSizer->Add( m_listBox, 0, wxALL, 5 );
	fgSizer->Add( new wxStaticText( m_panel1, wxID_ANY, wxT("Colour")), 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxTOP|wxBOTTOM|wxLEFT, 5 );
	fgSizer->Add( m_colourPicker, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	fgSizer->Add( new wxStaticText( m_panel1, wxID_ANY, wxT("Position")), 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	fgSizer->Add( m_spinCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	wxBoxSizer* bSizer1a = new wxBoxSizer( wxHORIZONTAL );
	bSizer1a->Add( button1, 0, wxALL, 5 );
	bSizer1a->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizer1a->Add( button2, 0, wxALL, 5 );

	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );
	bSizer1->Add( fgSizer, 0, 0, 5 );
	bSizer1->Add( bSizer1a, 1, wxEXPAND, 5 );

	m_panel1->SetSizer( bSizer1 );
	m_panel1->Layout();
    m_accordion->AddPage(m_panel1,"Gradient Stops",false);
    m_accordion->SetFixedSize(1);

    //create a panel for enabling/disabling pages and add it to the accordion
    m_panel2 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxCheckBox* checkBox1 = new wxCheckBox( m_panel2, wxID_CHECK1, wxT("Enable Page 1"), wxDefaultPosition, wxDefaultSize, 0 );
	checkBox1->SetValue(true);
	wxCheckBox* checkBox2 = new wxCheckBox( m_panel2, wxID_CHECK2, wxT("Enable Page 2"), wxDefaultPosition, wxDefaultSize, 0 );
	checkBox2->SetValue(true);
	wxCheckBox* checkBox3 = new wxCheckBox( m_panel2, wxID_CHECK3, wxT("Enable Page 4"), wxDefaultPosition, wxDefaultSize, 0 );
	checkBox3->SetValue(true);
	wxCheckBox* checkBox4 = new wxCheckBox( m_panel2, wxID_CHECK4, wxT("Enable Page 5"), wxDefaultPosition, wxDefaultSize, 0 );
	checkBox4->SetValue(true);

	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );
	bSizer2->Add( checkBox1, 0, wxALL, 5 );
	bSizer2->Add( checkBox2, 0, wxALL, 5 );
	bSizer2->Add( checkBox3, 0, wxALL, 5 );
	bSizer2->Add( checkBox4, 0, wxALL, 5 );

	m_panel2->SetSizer( bSizer2 );
	m_panel2->Layout();
    m_accordion->AddPage(m_panel2,"Enable/Disable Pages",false);
    m_accordion->SetFixedSize(2);

    //add a reset button to the accordion
	m_resetButton = new wxButton( m_panel1, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
    m_accordion->AddPage(m_resetButton,"Reset Accordion",false);
    m_accordion->SetFixedSize(3);

    //create a panel for code generation and add it to the accordion
	m_panel3 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_textCtrl = new wxTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
	m_textCtrl->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE , wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
	wxButton* button3 = new wxButton( m_panel3, wxID_ANY, wxT("Generate Code"), wxDefaultPosition, wxDefaultSize, 0 );

	wxBoxSizer* bSizer3 = new wxBoxSizer( wxVERTICAL );
	bSizer3->Add( m_textCtrl, 1, wxALL|wxEXPAND, 5 );
	bSizer3->Add( button3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	m_panel3->SetSizer( bSizer3 );
    m_accordion->AddPage(m_panel3,"Generate Code",false);

    //create and build the property grid
	m_propertyGrid = new wxPropertyGrid(m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER|wxNO_BORDER);
	buildGrid();

	//add the accordion and property grid to the splitter
	m_splitter->SplitVertically( m_accordion, m_propertyGrid, GetSize().GetWidth()*.5 );
	m_splitter->SetMinimumPaneSize( GetSize().GetWidth()*.025 );

	//set the sizer and finish setting up the form
	wxBoxSizer* bSizer = new wxBoxSizer( wxVERTICAL );
	bSizer->Add( m_splitter, 1, wxEXPAND, 5 );
	SetSizer( bSizer );
	CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	Layout();

	//connect the event handlers
    Bind( wxEVT_COMMAND_MENU_SELECTED, &BuilderFrame::OnQuit, this, menuFileQuit->GetId() );
	Bind( wxEVT_COMMAND_MENU_SELECTED, &BuilderFrame::OnAbout, this, menuHelpAbout->GetId() );
	Bind( wxEVT_CLOSE_WINDOW, &BuilderFrame::OnClose, this );
	button1->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &BuilderFrame::onClearStops, this );
	button2->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &BuilderFrame::onAddStop, this );
	checkBox1->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &BuilderFrame::onCheckBox, this );
	checkBox2->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &BuilderFrame::onCheckBox, this );
	checkBox3->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &BuilderFrame::onCheckBox, this );
	checkBox4->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &BuilderFrame::onCheckBox, this );
	m_resetButton->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &BuilderFrame::onReset, this );
	button3->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &BuilderFrame::onGenerate, this );
	m_propertyGrid->Bind( wxEVT_PG_CHANGED, &BuilderFrame::onPropertyGridChanged, this );

	//center the form on the screen
	Centre( wxBOTH );
}

BuilderFrame::~BuilderFrame()
{
}

void BuilderFrame::onAddStop( wxCommandEvent& WXUNUSED(event) )
{
    wxColour c = m_colourPicker->GetColour();
    long pos = m_spinCtrl->GetValue();
    int listItem = m_listBox->GetSelection();
    wxPGProperty* stopProp;

    if( listItem == 0 )
    {
        stopProp = m_pgColCapBarGradStops;
        pos = adjustStopPosition( stopProp, pos);
        m_accordion->GetCollapsedStyle().AddGradientStop(c,static_cast<float>(pos)/100.0);
    }
    else if( listItem == 1 )
    {
        stopProp = m_pgColHLCapBarGradStops;
        pos = adjustStopPosition( stopProp, pos);
        m_accordion->GetCollapsedHLStyle().AddGradientStop(c,static_cast<float>(pos)/100.0);
    }
    else if( listItem == 2 )
    {
        stopProp = m_pgExpCapBarGradStops;
        pos = adjustStopPosition( stopProp, pos);
        m_accordion->GetExpandedStyle().AddGradientStop(c,static_cast<float>(pos)/100.0);
    }
    else if( listItem == 3 )
    {
        stopProp = m_pgExpHLCapBarGradStops;
        pos = adjustStopPosition( stopProp, pos);
        m_accordion->GetExpandedHLStyle().AddGradientStop(c,static_cast<float>(pos)/100.0);
    }
    else if( listItem == 4 )
    {
        stopProp = m_pgDisCapBarGradStops;
        pos = adjustStopPosition( stopProp, pos);
        m_accordion->GetDisabledStyle().AddGradientStop(c,static_cast<float>(pos)/100.0);
    }
    else
    {
        stopProp = m_pgPagesGradStops;
        pos = adjustStopPosition( stopProp, pos);
        m_accordion->GetPageStyle().AddGradientStop(c,static_cast<float>(pos)/100.0);
    }

    stopProp->SetValue("");
    unsigned int i = stopProp->GetChildCount();

    wxPGProperty* p =  new wxGradientStopProperty( wxString::Format("Stop %d",i+1),wxPG_LABEL,GradientStop(c,pos) );
    m_propertyGrid->AppendIn(stopProp,p );
    m_propertyGrid->Refresh();
    m_accordion->Refresh();

    adjustStopBounds(stopProp);
}

void BuilderFrame::onClearStops( wxCommandEvent& WXUNUSED(event) )
{
    wxPGProperty* stopProp;
    int listItem = m_listBox->GetSelection();

    if( listItem == 0 )
    {
        stopProp=m_pgColCapBarGradStops;
        m_accordion->GetCollapsedStyle().ClearGradientStops();
    }
    else if( listItem == 1 )
    {
        stopProp=m_pgColHLCapBarGradStops;
        m_accordion->GetCollapsedHLStyle().ClearGradientStops();
    }
    else if( listItem == 2 )
    {
        stopProp=m_pgExpCapBarGradStops;
        m_accordion->GetExpandedStyle().ClearGradientStops();
    }
    else if( listItem == 3 )
    {
        stopProp=m_pgExpHLCapBarGradStops;
        m_accordion->GetExpandedHLStyle().ClearGradientStops();
    }
    else if( listItem == 4 )
    {
        stopProp=m_pgDisCapBarGradStops;
        m_accordion->GetDisabledStyle().ClearGradientStops();
    }
    else
    {
        stopProp=m_pgPagesGradStops;
        m_accordion->GetPageStyle().ClearGradientStops();
    }

    for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
    {
        //for some reason, if an property has ParentalType wxPG_PROP_AGGREGATE
        //and its value has been changed, the program will crach
        //when you delete that property.  So for all the gradient
        //stop properties, I'll change the ParentalType to wxPG_PROP_MISC_PARENT
        stopProp->Item(i)->SetParentalType(wxPG_PROP_MISC_PARENT);
    }

    stopProp->SetValue("None");
    stopProp->DeleteChildren();

    m_propertyGrid->Refresh();
    m_accordion->Refresh();
}

void BuilderFrame::onCheckBox(wxCommandEvent& event)
{
    if(event.IsChecked())
    {
        if( event.GetId()==wxID_CHECK1 )
        {
            m_accordion->Enable(0);
        }
        else if( event.GetId()==wxID_CHECK2 )
        {
            m_accordion->Enable(1);
        }
        else if( event.GetId()==wxID_CHECK3 )
        {
            m_accordion->Enable(3);
        }
        else if( event.GetId()==wxID_CHECK4 )
        {
            m_accordion->Enable(4);
        }
    }
    else
    {
        if( event.GetId()==wxID_CHECK1 )
        {
            m_accordion->Disable(0);
        }
        else if( event.GetId()==wxID_CHECK2 )
        {
            m_accordion->Disable(1);
        }
        else if( event.GetId()==wxID_CHECK3 )
        {
            m_accordion->Disable(3);
        }
        else if( event.GetId()==wxID_CHECK4 )
        {
            m_accordion->Disable(4);
        }
    }
}

void BuilderFrame::onReset( wxCommandEvent& WXUNUSED(event) )
{
    Freeze();

    bool b0 = m_accordion->IsExpanded(0);
    bool b1 = m_accordion->IsExpanded(1);
    bool b2 = m_accordion->IsExpanded(2);
    bool b3 = m_accordion->IsExpanded(3);
    bool b4 = m_accordion->IsExpanded(4);

    m_htmlWin->Reparent(this);
    m_panel1->Reparent(this);
    m_panel2->Reparent(this);
    m_resetButton->Reparent(this);
    m_panel3->Reparent(this);

    int splitPos = m_splitter->GetSashPosition();
    m_splitter->Unsplit(m_accordion);
    m_accordion->Destroy();
    m_accordion = new wxAccordion(m_splitter);

    m_accordion->AddPage(m_htmlWin,"Introduction",b0);
    m_accordion->AddPage(m_panel1,"Gradient Stops",b1);
    m_accordion->AddPage(m_panel2,"Enable/Disable Pages",b2);
    m_accordion->AddPage(m_resetButton,"Reset Accordion",b3);
    m_accordion->AddPage(m_panel3,"Generate Code",b4);

    m_accordion->SetFixedSize(1);
    m_accordion->SetFixedSize(2);
    m_accordion->SetFixedSize(3);

    m_splitter->SplitVertically( m_accordion, m_propertyGrid, splitPos );

    Layout();

    m_propertyGrid->Clear();
    buildGrid();

    Thaw();
}

void BuilderFrame::onGenerate( wxCommandEvent& WXUNUSED(event) )
{
    wxAccordion* temp = new wxAccordion(m_splitter);

    wxString s = "wxAccordion* m_accordion;\n";

    s << "m_accordion = new wxAccordion( <insert parent here>";
    long style = m_pgAccordionStyle->GetValue().GetLong();
    if(style==0)
    {
         s << " );\n";
    }
    else
    {
        s << ", wxID_ANY, wxDefaultPosition, wxDefaultSize, ";

        bool first(true);

        if(style & wxAC_BUTTON_LEFT)
        {
            first =false;
            s << "wxAC_BUTTON_LEFT";
        }
        if(style & wxAC_SINGLE_FOLD)
        {
            if(first) first =false;
            else s << "|";
            s << "wxAC_SINGLE_FOLD";
        }
        if(style & wxAC_COLLAPSE_TO_BOTTOM)
        {
            if(first) first =false;
            else s << "|";
            s << "wxAC_COLLAPSE_TO_BOTTOM";
        }
        if(style & wxAC_FLOAT_TO_TOP)
        {
            if(first) first =false;
            else s << "|";
            s << "wxAC_FLOAT_TO_TOP";
        }

        s << " );\n";
    }

    if( temp->GetAccordionPadding() != m_accordion->GetAccordionPadding() )
    {
        addIntString( s, "SetAccordionPadding", m_accordion->GetAccordionPadding() );
    }

    if( temp->GetToggleStyle() != m_accordion->GetToggleStyle() )
    {
        s << "m_accordion->SetToggleStyle( ";
        if( m_accordion->GetToggleStyle()==wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS )
        {
            s << "wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS );\n";
        }
        else
        {
            s << "wxAC_TOGGLE_ON_ANY_CLICK );\n";
        }
    }

    if( temp->GetBackgroundColour() != m_accordion->GetBackgroundColour() )
    {
        addColourString(s, "SetBackgroundColour", m_accordion->GetBackgroundColour());
    }

    if( m_pgAccordionUseImage->GetValue().GetBool() )
    {
        addBitmapString(s, "SetAccordionBGBitmap", m_pgAccordionImage );
    }

    if( temp->GetMinCaptionHeight() != m_accordion->GetMinCaptionHeight() )
    {
        addIntString( s, "SetMinCaptionHeight", m_accordion->GetMinCaptionHeight() );
    }

    if( temp->GetCaptionRadius() != m_accordion->GetCaptionRadius() )
    {
        addIntString( s, "SetCaptionRadius", m_accordion->GetCaptionRadius() );
    }

    if( temp->GetCaptionGradientAngle() != m_accordion->GetCaptionGradientAngle() )
    {
        addIntString( s, "SetCaptionGradientAngle", m_accordion->GetCaptionGradientAngle() );
    }

    if( temp->GetCaptionBorderStyle() != m_accordion->GetCaptionBorderStyle() )
    {
        s << "m_accordion->SetCaptionBorderStyle( ";
        if( m_accordion->GetCaptionBorderStyle()==wxAC_BORDERS_FULL )
        {
            s << "wxAC_BORDERS_FULL );\n";
        }
        else
        {
            s << "wxAC_BORDERS_NO_BOTTOM_WHEN_EXPANDED );\n";
        }
    }

    if( temp->GetTextMargin() != m_accordion->GetTextMargin() )
    {
        addSizeString( s, "SetTextMargin", m_accordion->GetTextMargin() );
    }

    if( temp->GetHighlighting() != m_accordion->GetHighlighting() )
    {
        s << "m_accordion->SetHighlighting( ";
        if( m_accordion->GetHighlighting()==wxAC_HIGHLIGHTING_MOUSEOVER )
        {
            s << "wxAC_HIGHLIGHTING_MOUSEOVER );\n";
        }
        else
        {
            s << "wxAC_HIGHLIGHTING_NONE );\n";
        }
    }

    if( temp->GetDisabledState() != m_accordion->GetDisabledState() )
    {
        s << "m_accordion->SetDisabledState( ";
        if( m_accordion->GetDisabledState()==wxAC_DISABLED_STATE_DEFAULT )
        {
            s << "wxAC_DISABLED_STATE_DEFAULT );\n";
        }
        else
        {
            s << "wxAC_DISABLED_STATE_CUSTOM );\n";
        }
    }

    if( temp->GetDisabledBrightness() != m_accordion->GetDisabledBrightness() )
    {
        addIntString( s, "SetDisabledBrightness", m_accordion->GetDisabledBrightness() );
    }

    if( m_pgUseButtons->GetValue().GetBool() )
    {
        addBitmapString(s, "SetExpandButton", m_pgExpButton );
        addBitmapString(s, "SetCollapseButton", m_pgColButton );
    }

    if( temp->GetButtonMargin() != m_accordion->GetButtonMargin() )
    {
        addSizeString( s, "SetButtonMargin", m_accordion->GetButtonMargin() );
    }

    if( m_pgShowIcons->GetValue().GetBool() )
    {
        const wxSize& sz=wxSizeRefFromVariant(m_pgIconSize->GetValue());

        s << "wxImageList* m_imagelist = new wxImageList(";
        s << sz.GetWidth();
        s << ",";
        s << sz.GetHeight();
        s << ");\n";
        s << "\\Add icons to the image list here.\n";
        s << "m_accordion->AssignImageList(m_imagelist);\n";
    }

    if( temp->GetIconMargin() != m_accordion->GetIconMargin() )
    {
        addSizeString( s, "SetIconMargin", m_accordion->GetIconMargin() );
    }

    if( temp->GetCollapsedStyle().GetColour1() != m_accordion->GetCollapsedStyle().GetColour1() )
    {
        addColourString(s, "GetCollapsedStyle().SetColour1", m_accordion->GetCollapsedStyle().GetColour1());
    }

    if( temp->GetCollapsedStyle().GetColour2() != m_accordion->GetCollapsedStyle().GetColour2() )
    {
        addColourString(s, "GetCollapsedStyle().SetColour2", m_accordion->GetCollapsedStyle().GetColour2());
    }

    wxPGProperty* stopProp = m_pgColCapBarGradStops;
    if( stopProp->GetChildCount() != 0 )
    {
        for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
        {
            addGradientStopString( s, "GetCollapsedStyle().AddGradientStop", stopProp->Item(i) );
        }
    }

    if( m_pgColCapBarUseImage->GetValue().GetBool() )
    {
        addBitmapString(s, "GetCollapsedStyle().SetBGBitmap", m_pgColCapBarImage );
    }

    if( m_pgColCapBarBrdrTrans->GetValue().GetBool() )
    {
        s << "m_accordion->GetCollapsedStyle().SetBorderColour(wxTransparentColour);\n";
    }
    else
    {
        if( temp->GetCollapsedStyle().GetBorderColour() != m_accordion->GetCollapsedStyle().GetBorderColour() )
        {
            addColourString(s, "GetCollapsedStyle().SetBorderColour", m_accordion->GetCollapsedStyle().GetBorderColour());
        }
    }

    if( temp->GetCollapsedStyle().GetTextColour() != m_accordion->GetCollapsedStyle().GetTextColour() )
    {
        addColourString(s, "GetCollapsedStyle().SetTextColour", m_accordion->GetCollapsedStyle().GetTextColour());
    }

    if( temp->GetCollapsedStyle().GetFont() != m_accordion->GetCollapsedStyle().GetFont() )
    {
        addFontString(s,"GetCollapsedStyle().SetFont",m_accordion->GetCollapsedStyle().GetFont());
    }



    if( temp->GetCollapsedHLStyle().GetColour1() != m_accordion->GetCollapsedHLStyle().GetColour1() )
    {
        addColourString(s, "GetCollapsedHLStyle().SetColour1", m_accordion->GetCollapsedHLStyle().GetColour1());
    }

    if( temp->GetCollapsedHLStyle().GetColour2() != m_accordion->GetCollapsedHLStyle().GetColour2() )
    {
        addColourString(s, "GetCollapsedHLStyle().SetColour2", m_accordion->GetCollapsedHLStyle().GetColour2());
    }

    stopProp = m_pgColHLCapBarGradStops;
    if( stopProp->GetChildCount() != 0 )
    {
        for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
        {
            addGradientStopString( s, "GetCollapsedHLStyle().AddGradientStop", stopProp->Item(i) );
        }
    }

    if( m_pgColHLCapBarUseImage->GetValue().GetBool() )
    {
        addBitmapString(s, "GetCollapsedHLStyle().SetBGBitmap", m_pgColHLCapBarImage );
    }

    if( m_pgColHLCapBarBrdrTrans->GetValue().GetBool() )
    {
        s << "m_accordion->GetCollapsedHLStyle().SetBorderColour(wxTransparentColour);\n";
    }
    else
    {
        if( temp->GetCollapsedHLStyle().GetBorderColour() != m_accordion->GetCollapsedHLStyle().GetBorderColour() )
        {
            addColourString(s, "GetCollapsedHLStyle().SetBorderColour", m_accordion->GetCollapsedHLStyle().GetBorderColour());
        }
    }

    if( temp->GetCollapsedHLStyle().GetTextColour() != m_accordion->GetCollapsedHLStyle().GetTextColour() )
    {
        addColourString(s, "GetCollapsedHLStyle().SetTextColour", m_accordion->GetCollapsedHLStyle().GetTextColour());
    }

    if( temp->GetCollapsedHLStyle().GetFont() != m_accordion->GetCollapsedHLStyle().GetFont() )
    {
        addFontString( s, "GetCollapsedHLStyle().SetFont", m_accordion->GetCollapsedHLStyle().GetFont() );
    }



    if( temp->GetExpandedStyle().GetColour1() != m_accordion->GetExpandedStyle().GetColour1() )
    {
        addColourString(s, "GetExpandedStyle().SetColour1", m_accordion->GetExpandedStyle().GetColour1());
    }

    if( temp->GetExpandedStyle().GetColour2() != m_accordion->GetExpandedStyle().GetColour2() )
    {
        addColourString(s, "GetExpandedStyle().SetColour2", m_accordion->GetExpandedStyle().GetColour2());
    }

    stopProp = m_pgExpCapBarGradStops;
    if( stopProp->GetChildCount() != 0 )
    {
        for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
        {
            addGradientStopString( s, "GetExpandedStyle().AddGradientStop", stopProp->Item(i) );
        }
    }

    if( m_pgExpCapBarUseImage->GetValue().GetBool() )
    {
        addBitmapString(s, "GetExpandedStyle().SetBGBitmap", m_pgExpCapBarImage );
    }

    if( m_pgExpCapBarBrdrTrans->GetValue().GetBool() )
    {
        s << "m_accordion->GetExpandedStyle().SetBorderColour(wxTransparentColour);\n";
    }
    else
    {
        if( temp->GetExpandedStyle().GetBorderColour() != m_accordion->GetExpandedStyle().GetBorderColour() )
        {
            addColourString(s, "GetExpandedStyle().SetBorderColour", m_accordion->GetExpandedStyle().GetBorderColour());
        }
    }

    if( temp->GetExpandedStyle().GetTextColour() != m_accordion->GetExpandedStyle().GetTextColour() )
    {
        addColourString(s, "GetExpandedStyle().SetTextColour", m_accordion->GetExpandedStyle().GetTextColour());
    }

    if( temp->GetExpandedStyle().GetFont() != m_accordion->GetExpandedStyle().GetFont() )
    {
        addFontString(s,"GetExpandedStyle().SetFont",m_accordion->GetExpandedStyle().GetFont());
    }



    if( temp->GetExpandedHLStyle().GetColour1() != m_accordion->GetExpandedHLStyle().GetColour1() )
    {
        addColourString(s, "GetExpandedHLStyle().SetColour1", m_accordion->GetExpandedHLStyle().GetColour1());
    }

    if( temp->GetExpandedHLStyle().GetColour2() != m_accordion->GetExpandedHLStyle().GetColour2() )
    {
        addColourString(s, "GetExpandedHLStyle().SetColour2", m_accordion->GetExpandedHLStyle().GetColour2());
    }

    stopProp = m_pgExpHLCapBarGradStops;
    if( stopProp->GetChildCount() != 0 )
    {
        for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
        {
            addGradientStopString( s, "GetExpandedHLStyle().AddGradientStop", stopProp->Item(i) );
        }
    }

    if( m_pgExpHLCapBarUseImage->GetValue().GetBool() )
    {
        addBitmapString(s, "GetExpandedHLStyle().SetBGBitmap", m_pgExpHLCapBarImage );
    }

    if( m_pgExpHLCapBarBrdrTrans->GetValue().GetBool() )
    {
        s << "m_accordion->GetExpandedHLStyle().SetBorderColour(wxTransparentColour);\n";
    }
    else
    {
        if( temp->GetExpandedHLStyle().GetBorderColour() != m_accordion->GetExpandedHLStyle().GetBorderColour() )
        {
            addColourString(s, "GetExpandedHLStyle().SetBorderColour", m_accordion->GetExpandedHLStyle().GetBorderColour());
        }
    }

    if( temp->GetExpandedHLStyle().GetTextColour() != m_accordion->GetExpandedHLStyle().GetTextColour() )
    {
        addColourString(s, "GetExpandedHLStyle().SetTextColour", m_accordion->GetExpandedHLStyle().GetTextColour());
    }

    if( temp->GetExpandedHLStyle().GetFont() != m_accordion->GetExpandedHLStyle().GetFont() )
    {
        addFontString(s,"GetExpandedHLStyle().SetFont",m_accordion->GetExpandedHLStyle().GetFont());
    }



    if( temp->GetDisabledStyle().GetColour1() != m_accordion->GetDisabledStyle().GetColour1() )
    {
        addColourString(s, "GetDisabledStyle().SetColour1", m_accordion->GetDisabledStyle().GetColour1());
    }

    if( temp->GetDisabledStyle().GetColour2() != m_accordion->GetDisabledStyle().GetColour2() )
    {
        addColourString(s, "GetDisabledStyle().SetColour2", m_accordion->GetDisabledStyle().GetColour2());
    }

    stopProp = m_pgDisCapBarGradStops;
    if( stopProp->GetChildCount() != 0 )
    {
        for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
        {
            addGradientStopString( s, "GetDisabledStyle().AddGradientStop", stopProp->Item(i) );
        }
    }

    if( m_pgDisCapBarUseImage->GetValue().GetBool() )
    {
        addBitmapString(s, "GetDisabledStyle().SetBGBitmap", m_pgDisCapBarImage );
    }

    if( m_pgDisCapBarBrdrTrans->GetValue().GetBool() )
    {
        s << "m_accordion->GetDisabledStyle().SetBorderColour(wxTransparentColour);\n";
    }
    else
    {
        if( temp->GetDisabledStyle().GetBorderColour() != m_accordion->GetDisabledStyle().GetBorderColour() )
        {
            addColourString(s, "GetDisabledStyle().SetBorderColour", m_accordion->GetDisabledStyle().GetBorderColour());
        }
    }

    if( temp->GetDisabledStyle().GetTextColour() != m_accordion->GetDisabledStyle().GetTextColour() )
    {
        addColourString(s, "GetDisabledStyle().SetTextColour", m_accordion->GetDisabledStyle().GetTextColour());
    }

    if( temp->GetDisabledStyle().GetFont() != m_accordion->GetDisabledStyle().GetFont() )
    {
        addFontString(s,"GetDisabledStyle().SetFont",m_accordion->GetDisabledStyle().GetFont());
    }




    if( temp->GetPagePadding() != m_accordion->GetPagePadding() )
    {
        addIntString( s, "SetPagePadding", m_accordion->GetPagePadding() );
    }

    if( temp->GetPageRadius() != m_accordion->GetPageRadius() )
    {
        addIntString( s, "SetPageRadius", m_accordion->GetPageRadius() );
    }

    if( temp->GetPageStyle().GetColour1() != m_accordion->GetPageStyle().GetColour1() )
    {
        addColourString(s, "GetPageStyle().SetColour1", m_accordion->GetPageStyle().GetColour1());
    }

    if( temp->GetPageStyle().GetColour2() != m_accordion->GetPageStyle().GetColour2() )
    {
        addColourString(s, "GetPageStyle().SetColour2", m_accordion->GetPageStyle().GetColour2());
    }

    stopProp = m_pgPagesGradStops;
    if( stopProp->GetChildCount() != 0 )
    {
        for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
        {
            addGradientStopString( s, "GetPageStyle().AddGradientStop", stopProp->Item(i) );
        }
    }

    if( m_pgPagesUseImage->GetValue().GetBool() )
    {
        addBitmapString(s, "GetPageStyle().SetBGBitmap", m_pgPagesImage );
    }

    if( m_pgPagesBrdrTrans->GetValue().GetBool() )
    {
        s << "m_accordion->GetPageStyle().SetBorderColour(wxTransparentColour);\n";
    }
    else
    {
        if( temp->GetPageStyle().GetBorderColour() != m_accordion->GetPageStyle().GetBorderColour() )
        {
            addColourString(s, "GetPageStyle().SetBorderColour", m_accordion->GetPageStyle().GetBorderColour());
        }
    }

    temp->Destroy();
    m_textCtrl->SetValue(s);
}

void BuilderFrame::onPropertyGridChanged( wxPropertyGridEvent& event )
{
    wxPGProperty* p = event.GetProperty();

    if( p == m_pgAccordionStyle )
    {
        m_accordion->SetWindowStyle( p->GetValue().GetLong() );
        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgAccordionPadding )
    {
        m_accordion->SetAccordionPadding( p->GetValue().GetLong() );
        m_accordion->Layout();
    }
    else if( p == m_pgAccordionOnlyToggleWithButton )
    {
        m_accordion->SetToggleStyle( p->GetValue().GetBool()?wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS:wxAC_TOGGLE_ON_ANY_CLICK );
    }
    else if( p == m_pgAccordionBGProp )
    {
        m_accordion->SetBackgroundColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgAccordionUseImage )
    {
        bool b = p->GetValue().GetBool();
        m_pgAccordionImage->Enable(b);

        if(b)
        {
            if(!m_pgAccordionImage->GetValue().GetString().IsEmpty())
            {
                m_accordion->SetAccordionBGBitmap(wxBitmap(m_pgAccordionImage->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
            }
        }
        else
        {
            m_accordion->SetAccordionBGBitmap(wxNullBitmap);
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgAccordionImage )
    {
        m_accordion->SetAccordionBGBitmap(wxBitmap(p->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
        m_accordion->Refresh();
    }
    else if( p == m_pgCaptionMinHt )
    {
        m_accordion->SetMinCaptionHeight(p->GetValue().GetLong());
        m_accordion->Layout();
    }
    else if( p == m_pgCaptionRadius)
    {
        m_accordion->SetCaptionRadius(p->GetValue().GetLong());
        m_accordion->Refresh();
    }
    else if( p == m_pgCaptionGradientAngle )
    {
        m_accordion->SetCaptionGradientAngle(p->GetValue().GetLong());
        m_accordion->Refresh();
    }
    else if( p == m_pgCaptionExpandedBtmBrdr )
    {
        m_accordion->SetCaptionBorderStyle( p->GetValue().GetBool()?wxAC_BORDERS_FULL:wxAC_BORDERS_NO_BOTTOM_WHEN_EXPANDED );
        m_accordion->Refresh();
    }
    else if( p == m_pgCaptionTextPadding )
    {
        m_accordion->SetTextMargin( wxSizeRefFromVariant(p->GetValue()) );
        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgCaptionUseHighlighting )
    {
        bool b = p->GetValue().GetBool();

        m_pgColHLCapBarCat->Hide(!b);
        m_pgExpHLCapBarCat->Hide(!b);

        m_accordion->SetHighlighting(b?wxAC_HIGHLIGHTING_MOUSEOVER:wxAC_HIGHLIGHTING_NONE);
        m_accordion->Refresh();
    }
    else if( p == m_pgCaptionCustomDisabled )
    {
        bool b = p->GetValue().GetBool();

        m_pgDisCapBarCat->Hide(!b);
        m_pgCaptionTDisabledBrightness->Enable(b);

        m_accordion->SetDisabledState(b?wxAC_DISABLED_STATE_CUSTOM:wxAC_DISABLED_STATE_DEFAULT);
        m_accordion->Refresh();
    }
    else if( p == m_pgCaptionTDisabledBrightness )
    {
        long b = p->GetValue().GetLong();

        m_accordion->SetDisabledBrightness(b);
        m_accordion->Refresh();
    }
    else if(p == m_pgUseButtons )
    {
        bool b = p->GetValue().GetBool();
        m_pgExpButton->Enable(b);
        m_pgColButton->Enable(b);
        m_pgButtonPadding->Enable(b);

        if(b)
        {
            if(!m_pgExpButton->GetValue().GetString().IsEmpty())
            {
                m_accordion->SetExpandButton(wxBitmap(m_pgExpButton->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
            }

            if(!m_pgColButton->GetValue().GetString().IsEmpty())
            {
                m_accordion->SetCollapseButton(wxBitmap(m_pgColButton->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
            }
        }
        else
        {
            m_accordion->SetCollapseButton(wxNullBitmap);
            m_accordion->SetExpandButton(wxNullBitmap);
        }

        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgExpButton )
    {
        m_accordion->SetExpandButton( wxBitmap(p->GetValue().GetString(),wxBITMAP_TYPE_ANY ) );
        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgColButton )
    {
        m_accordion->SetCollapseButton( wxBitmap(p->GetValue().GetString(),wxBITMAP_TYPE_ANY ) );
        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgButtonPadding)
    {
        m_accordion->SetButtonMargin( wxSizeRefFromVariant(p->GetValue()) );
        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgShowIcons )
    {
        bool b = p->GetValue().GetBool();
        m_pgIconColour->Enable(b);
        m_pgIconSize->Enable(b);
        m_pgIconPadding->Enable(b);

        if(b)
        {
            setImageList(m_accordion,wxSizeRefFromVariant(m_pgIconSize->GetValue()),m_pgIconColour->GetValue().GetAny().As<wxColour>());
        }
        else
        {
            m_accordion->AssignImageList(NULL);

            for(size_t i=0;i<m_accordion->GetPageCount();i++)
            {
                m_accordion->SetPageImage(i,wxAccordion::NO_IMAGE);
            }
        }

        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgIconColour )
    {
        setImageList(m_accordion,wxSizeRefFromVariant(p->GetParent()->Item(2)->GetValue()),p->GetValue().GetAny().As<wxColour>());
        m_accordion->Refresh();
    }
    else if( p == m_pgIconSize )
    {
        setImageList(m_accordion,wxSizeRefFromVariant(p->GetValue()),p->GetParent()->Item(1)->GetValue().GetAny().As<wxColour>());
        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgIconPadding )
    {
        m_accordion->SetIconMargin( wxSizeRefFromVariant(p->GetValue()) );
        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgColCapBarCol1 )
    {
        m_accordion->GetCollapsedStyle().SetColour1( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgColCapBarCol2 )
    {
        m_accordion->GetCollapsedStyle().SetColour2( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgColCapBarUseImage )
    {
        bool b = p->GetValue().GetBool();
        m_pgColCapBarImage->Enable(b);

        if(b)
        {
            if(!m_pgColCapBarImage->GetValue().GetString().IsEmpty())
            {
                m_accordion->GetCollapsedStyle().SetBGBitmap(wxBitmap(m_pgColCapBarImage->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
            }
        }
        else
        {
            m_accordion->GetCollapsedStyle().SetBGBitmap(wxNullBitmap);
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgColCapBarImage )
    {
        m_accordion->GetCollapsedStyle().SetBGBitmap(wxBitmap(p->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
        m_accordion->Refresh();
    }
    else if( p == m_pgColCapBarBrdrTrans )
    {
        bool b = p->GetValue().GetBool();

        m_pgColCapBarBrdrCol->Enable(!b);
        if(b)
        {
            m_accordion->GetCollapsedStyle().SetBorderColour(wxTransparentColour);
        }
        else
        {
            m_accordion->GetCollapsedStyle().SetBorderColour(m_pgColCapBarBrdrCol->GetValue().GetAny().As<wxColour>());
        }
        m_accordion->Refresh();
    }
    else if( p == m_pgColCapBarBrdrCol )
    {
        m_accordion->GetCollapsedStyle().SetBorderColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgColCapBarTextCol )
    {
        m_accordion->GetCollapsedStyle().SetTextColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgColCapBarFont )
    {
        m_accordion->GetCollapsedStyle().SetFont( p->GetValue().GetAny().As<wxFont>() );
        m_accordion->Layout();
        m_accordion->Refresh();
    }

    else if( p == m_pgExpCapBarCol1 )
    {
        m_accordion->GetExpandedStyle().SetColour1( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgExpCapBarCol2 )
    {
        m_accordion->GetExpandedStyle().SetColour2( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgExpCapBarUseImage )
    {
        bool b = p->GetValue().GetBool();
        m_pgExpCapBarImage->Enable(b);

        if(b)
        {
            if(!m_pgExpCapBarImage->GetValue().GetString().IsEmpty())
            {
                m_accordion->GetExpandedStyle().SetBGBitmap(wxBitmap(m_pgExpCapBarImage->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
            }
        }
        else
        {
            m_accordion->GetExpandedStyle().SetBGBitmap(wxNullBitmap);
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgExpCapBarImage )
    {
        m_accordion->GetExpandedStyle().SetBGBitmap(wxBitmap(p->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
        m_accordion->Refresh();
    }
    else if( p == m_pgExpCapBarBrdrTrans )
    {
        bool b = p->GetValue().GetBool();

        m_pgExpCapBarBrdrCol->Enable(!b);
        if(b)
        {
            m_accordion->GetExpandedStyle().SetBorderColour(wxTransparentColour);
        }
        else
        {
            m_accordion->GetExpandedStyle().SetBorderColour(m_pgExpCapBarBrdrCol->GetValue().GetAny().As<wxColour>());
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgExpCapBarBrdrCol )
    {
        m_accordion->GetExpandedStyle().SetBorderColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgExpCapBarTextCol )
    {
        m_accordion->GetExpandedStyle().SetTextColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgExpCapBarFont )
    {
        m_accordion->GetExpandedStyle().SetFont( p->GetValue().GetAny().As<wxFont>() );
        m_accordion->Layout();
        m_accordion->Refresh();
    }

    else if( p == m_pgColHLCapBarCol1 )
    {
        m_accordion->GetCollapsedHLStyle().SetColour1( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgColHLCapBarCol2 )
    {
        m_accordion->GetCollapsedHLStyle().SetColour2( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgColHLCapBarUseImage )
    {
        bool b = p->GetValue().GetBool();
        m_pgColHLCapBarImage->Enable(b);

        if(b)
        {
            if(!m_pgColHLCapBarImage->GetValue().GetString().IsEmpty())
            {
                m_accordion->GetCollapsedHLStyle().SetBGBitmap(wxBitmap(m_pgColHLCapBarImage->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
            }
        }
        else
        {
            m_accordion->GetCollapsedHLStyle().SetBGBitmap(wxNullBitmap);
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgColHLCapBarImage )
    {
        m_accordion->GetCollapsedHLStyle().SetBGBitmap(wxBitmap(p->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
        m_accordion->Refresh();
    }
    else if( p == m_pgColHLCapBarBrdrTrans )
    {
        bool b = p->GetValue().GetBool();

        m_pgColHLCapBarBrdrCol->Enable(!b);
        if(b)
        {
            m_accordion->GetCollapsedHLStyle().SetBorderColour(wxTransparentColour);
        }
        else
        {
            m_accordion->GetCollapsedHLStyle().SetBorderColour(m_pgColHLCapBarBrdrCol->GetValue().GetAny().As<wxColour>());
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgColHLCapBarBrdrCol )
    {
        m_accordion->GetCollapsedHLStyle().SetBorderColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgColHLCapBarTextCol )
    {
        m_accordion->GetCollapsedHLStyle().SetTextColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgColHLCapBarFont )
    {
        m_accordion->GetCollapsedHLStyle().SetFont( p->GetValue().GetAny().As<wxFont>() );
        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgExpHLCapBarCol1 )
    {
        m_accordion->GetExpandedHLStyle().SetColour1( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgExpHLCapBarCol2 )
    {
        m_accordion->GetExpandedHLStyle().SetColour2( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgExpHLCapBarUseImage )
    {
        bool b = p->GetValue().GetBool();
        m_pgExpHLCapBarImage->Enable(b);

        if(b)
        {
            if(!m_pgExpHLCapBarImage->GetValue().GetString().IsEmpty())
            {
                m_accordion->GetExpandedHLStyle().SetBGBitmap(wxBitmap(m_pgExpHLCapBarImage->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
            }
        }
        else
        {
            m_accordion->GetExpandedHLStyle().SetBGBitmap(wxNullBitmap);
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgExpHLCapBarImage )
    {
        m_accordion->GetExpandedHLStyle().SetBGBitmap(wxBitmap(p->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
        m_accordion->Refresh();
    }
    else if( p == m_pgExpHLCapBarBrdrTrans )
    {
        bool b = p->GetValue().GetBool();

        m_pgExpHLCapBarBrdrCol->Enable(!b);
        if(b)
        {
            m_accordion->GetExpandedHLStyle().SetBorderColour(wxTransparentColour);
        }
        else
        {
            m_accordion->GetExpandedHLStyle().SetBorderColour(m_pgExpHLCapBarBrdrCol->GetValue().GetAny().As<wxColour>());
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgExpHLCapBarBrdrCol )
    {
        m_accordion->GetExpandedHLStyle().SetBorderColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgExpHLCapBarTextCol )
    {
        m_accordion->GetExpandedHLStyle().SetTextColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgExpHLCapBarFont )
    {
        m_accordion->GetExpandedHLStyle().SetFont( p->GetValue().GetAny().As<wxFont>() );
        m_accordion->Layout();
        m_accordion->Refresh();
    }

    else if( p == m_pgDisCapBarCol1 )
    {
        m_accordion->GetDisabledStyle().SetColour1( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgDisCapBarCol2 )
    {
        m_accordion->GetDisabledStyle().SetColour2( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgDisCapBarUseImage )
    {
        bool b = p->GetValue().GetBool();
        m_pgDisCapBarImage->Enable(b);

        if(b)
        {
            if(!m_pgDisCapBarImage->GetValue().GetString().IsEmpty())
            {
                m_accordion->GetDisabledStyle().SetBGBitmap(wxBitmap(m_pgDisCapBarImage->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
            }
        }
        else
        {
            m_accordion->GetDisabledStyle().SetBGBitmap(wxNullBitmap);
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgDisCapBarImage )
    {
        m_accordion->GetDisabledStyle().SetBGBitmap(wxBitmap(p->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
        m_accordion->Refresh();
    }
    else if( p == m_pgDisCapBarBrdrTrans )
    {
        bool b = p->GetValue().GetBool();

        m_pgDisCapBarBrdrCol->Enable(!b);
        if(b)
        {
            m_accordion->GetDisabledStyle().SetBorderColour(wxTransparentColour);
        }
        else
        {
            m_accordion->GetDisabledStyle().SetBorderColour(m_pgDisCapBarBrdrCol->GetValue().GetAny().As<wxColour>());
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgDisCapBarBrdrCol )
    {
        m_accordion->GetDisabledStyle().SetBorderColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgDisCapBarTextCol )
    {
        m_accordion->GetDisabledStyle().SetTextColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgDisCapBarFont )
    {
        m_accordion->GetDisabledStyle().SetFont( p->GetValue().GetAny().As<wxFont>() );
        m_accordion->Layout();
        m_accordion->Refresh();
    }

    else if( p == m_pgPagesPadding )
    {
        m_accordion->SetPagePadding(p->GetValue().GetLong());
        m_accordion->Layout();
    }
    else if( p == m_pgPagesRadius )
    {
        m_accordion->SetPageRadius(p->GetValue().GetLong());
        m_accordion->Refresh();
    }
    else if( p == m_pgPagesCol1)
    {
        m_accordion->GetPageStyle().SetColour1( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgPagesCol2)
    {
        m_accordion->GetPageStyle().SetColour2( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Refresh();
    }
    else if( p == m_pgPagesUseImage)
    {
        bool b = p->GetValue().GetBool();
        m_pgPagesImage->Enable(b);

        if(b)
        {
            if(!m_pgPagesImage->GetValue().GetString().IsEmpty())
            {
                m_accordion->GetPageStyle().SetBGBitmap(wxBitmap(m_pgPagesImage->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
            }
        }
        else
        {
            m_accordion->GetPageStyle().SetBGBitmap(wxNullBitmap);
        }

        m_accordion->Refresh();
    }
    else if( p == m_pgPagesImage)
    {
        m_accordion->GetPageStyle().SetBGBitmap(wxBitmap(p->GetValue().GetString(),wxBITMAP_TYPE_ANY ));
        m_accordion->Refresh();
    }
    else if( p == m_pgPagesBrdrTrans )
    {
        bool b = p->GetValue().GetBool();

        m_pgPagesBrdrCol->Enable(!b);
        if(b)
        {
            m_accordion->GetPageStyle().SetBorderColour(wxTransparentColour);
        }
        else
        {
            m_accordion->GetPageStyle().SetBorderColour(m_pgPagesBrdrCol->GetValue().GetAny().As<wxColour>());
        }

        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if( p == m_pgPagesBrdrCol )
    {
        m_accordion->GetPageStyle().SetBorderColour( p->GetValue().GetAny().As<wxColour>() );
        m_accordion->Layout();
        m_accordion->Refresh();
    }
    else if(p->GetLabel().Left(4)=="Stop")
    {
        //There isn't really any way to update a stop from outside
        //the accordion.  The best that can be done is to clear the stops, and
        //then add the stops back 1 by 1.

        wxPGProperty* stopProp = p->GetParent();
        wxColour c;
        float pos;

        if( stopProp == m_pgColCapBarGradStops )
        {
            m_accordion->GetCollapsedStyle().ClearGradientStops();

            for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
            {
                c = stopProp->Item(i)->Item(0)->GetValue().GetAny().As<wxColour>();
                pos = static_cast<float>(stopProp->Item(i)->Item(1)->GetValue().GetLong())/100.0;
                m_accordion->GetCollapsedStyle().AddGradientStop(c,pos);
            }
        }
        else if( stopProp == m_pgColHLCapBarGradStops )
        {
            m_accordion->GetCollapsedHLStyle().ClearGradientStops();

            for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
            {
                c = stopProp->Item(i)->Item(0)->GetValue().GetAny().As<wxColour>();
                pos = static_cast<float>(stopProp->Item(i)->Item(1)->GetValue().GetLong())/100.0;
                m_accordion->GetCollapsedHLStyle().AddGradientStop(c,pos);
            }
        }
        else if( stopProp == m_pgExpCapBarGradStops )
        {
            m_accordion->GetExpandedStyle().ClearGradientStops();

            for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
            {
                c = stopProp->Item(i)->Item(0)->GetValue().GetAny().As<wxColour>();
                pos = static_cast<float>(stopProp->Item(i)->Item(1)->GetValue().GetLong())/100.0;
                m_accordion->GetExpandedStyle().AddGradientStop(c,pos);
            }
        }
        else if( stopProp == m_pgExpHLCapBarGradStops )
        {
            m_accordion->GetExpandedHLStyle().ClearGradientStops();

            for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
            {
                c = stopProp->Item(i)->Item(0)->GetValue().GetAny().As<wxColour>();
                pos = static_cast<float>(stopProp->Item(i)->Item(1)->GetValue().GetLong())/100.0;
                m_accordion->GetExpandedHLStyle().AddGradientStop(c,pos);
            }
        }
        else if( stopProp == m_pgDisCapBarGradStops )
        {
            m_accordion->GetDisabledStyle().ClearGradientStops();

            for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
            {
                c = stopProp->Item(i)->Item(0)->GetValue().GetAny().As<wxColour>();
                pos = static_cast<float>(stopProp->Item(i)->Item(1)->GetValue().GetLong())/100.0;
                m_accordion->GetDisabledStyle().AddGradientStop(c,pos);
            }
        }
        else if( stopProp == m_pgPagesGradStops )
        {
            m_accordion->GetPageStyle().ClearGradientStops();

            for(unsigned int i=0; i< stopProp->GetChildCount(); i++)
            {
                c = stopProp->Item(i)->Item(0)->GetValue().GetAny().As<wxColour>();
                pos = static_cast<float>(stopProp->Item(i)->Item(1)->GetValue().GetLong())/100.0;
                m_accordion->GetPageStyle().AddGradientStop(c,pos);
            }
        }

        m_accordion->Refresh();
        adjustStopBounds(stopProp);
    }
}


void BuilderFrame::buildGrid()
{
    wxArrayString cases;
    wxArrayInt cases_values;
    cases.Add("BUTTON_LEFT");
    cases.Add("SINGLE_FOLD");
    cases.Add("COLLAPSE_TO_BOTTOM");
    cases.Add("FLOAT_TO_TOP");

    cases_values.Add(wxAC_BUTTON_LEFT);
    cases_values.Add(wxAC_SINGLE_FOLD);
    cases_values.Add(wxAC_COLLAPSE_TO_BOTTOM);
    cases_values.Add(wxAC_FLOAT_TO_TOP);

    //m_pgAccordionCat
	m_pgAccordionCat = m_propertyGrid->Append( new wxPropertyCategory( wxT("Accordion"), wxT("Accordion") ) );

    m_pgAccordionStyle=m_propertyGrid->Append( new wxFlagsProperty( wxT("Style"), wxPG_LABEL,cases,cases_values,m_accordion->GetWindowStyleFlag()) );
    m_pgAccordionStyle->SetAttribute( wxPG_BOOL_USE_CHECKBOX,wxPG_RECURSE );

	m_pgAccordionPadding = m_propertyGrid->Append( new wxIntProperty( wxT("Padding"), wxPG_LABEL,m_accordion->GetAccordionPadding() ) );
    m_pgAccordionPadding->SetEditor( "SpinCtrl" );
    m_pgAccordionPadding->SetAttribute(  wxT("Step"), (long)1 );
	m_pgAccordionPadding->SetAttribute(  wxT("MotionSpin"), true );
	m_pgAccordionPadding->SetAttribute(  wxPG_ATTR_MIN, (long)0);

	//I'm using a bool property instead of an enum because if you use an enum, the
	//program will crash when the grid is reset.
	m_pgAccordionOnlyToggleWithButton = m_propertyGrid->Append( new wxBoolProperty( wxT("Only Toggle With Button"), wxPG_LABEL,m_accordion->GetToggleStyle()==wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS ) );
	m_pgAccordionOnlyToggleWithButton->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgAccordionBGProp = m_propertyGrid->Append( new wxColourProperty( wxT("Background"), wxPG_LABEL ,m_accordion->GetBackgroundColour() ) );

	m_pgAccordionUseImage = m_propertyGrid->Append( new wxBoolProperty( wxT("Use Image"), wxPG_LABEL,m_accordion->GetAccordionBGBitmap().IsOk() ) );
	m_pgAccordionUseImage->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgAccordionImage = m_propertyGrid->Append( new wxImageFileProperty( wxT("Image"), wxPG_LABEL ) );
	m_pgAccordionImage->Enable(false);

    //m_pgCaptionBarCat
    m_pgCaptionBarCat = m_propertyGrid->Append( new wxPropertyCategory( wxT("Caption Bar"), wxPG_LABEL ) );

	m_pgCaptionMinHt = m_propertyGrid->Append( new wxIntProperty( wxT("Minimum Height"), wxPG_LABEL,m_accordion->GetMinCaptionHeight() ) );
    m_pgCaptionMinHt->SetEditor( "SpinCtrl" );
    m_pgCaptionMinHt->SetAttribute(  wxT("Step"), (long)1 );
	m_pgCaptionMinHt->SetAttribute(  wxT("MotionSpin"), true );
	m_pgCaptionMinHt->SetAttribute(  wxPG_ATTR_MIN, (long)0);

	m_pgCaptionRadius = m_propertyGrid->Append( new wxIntProperty( wxT("Radius"), wxPG_LABEL, static_cast<int>(m_accordion->GetCaptionRadius()) ) );
    m_pgCaptionRadius->SetEditor( "SpinCtrl" );
    m_pgCaptionRadius->SetAttribute(  wxT("Step"), (long)1 );
	m_pgCaptionRadius->SetAttribute(  wxT("MotionSpin"), true );
	m_pgCaptionRadius->SetAttribute(  wxPG_ATTR_MIN, (long)0);

    m_pgCaptionGradientAngle = m_propertyGrid->Append( new wxIntProperty( wxT("Gradient Angle"), wxPG_LABEL ,static_cast<int>(m_accordion->GetCaptionGradientAngle()) ) );
    m_pgCaptionGradientAngle->SetEditor( "SpinCtrl" );
    m_pgCaptionGradientAngle->SetAttribute(  wxT("Step"), (long)1 );
	m_pgCaptionGradientAngle->SetAttribute(  wxT("MotionSpin"), true );

    m_pgCaptionExpandedBtmBrdr = m_propertyGrid->Append( new wxBoolProperty( wxT("Expanded Bottom Border"), wxPG_LABEL, m_accordion->GetCaptionBorderStyle()==wxAC_BORDERS_FULL ) );
    m_pgCaptionExpandedBtmBrdr->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

    m_pgCaptionTextPadding = m_propertyGrid->Append( new wxSizeProperty("Text Margin",wxPG_LABEL,m_accordion->GetTextMargin()) );

    m_pgCaptionUseHighlighting = m_propertyGrid->Append( new wxBoolProperty("Use Highlighting",wxPG_LABEL,m_accordion->GetHighlighting()==wxAC_HIGHLIGHTING_MOUSEOVER) );
    m_pgCaptionUseHighlighting->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

    m_pgCaptionCustomDisabled = m_propertyGrid->Append( new wxBoolProperty("Custom Disabled State",wxPG_LABEL,m_accordion->GetDisabledState()==wxAC_DISABLED_STATE_CUSTOM) );
    m_pgCaptionCustomDisabled->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

    m_pgCaptionTDisabledBrightness = m_propertyGrid->Append( new wxIntProperty( wxT("Disabled Brightness"), wxPG_LABEL ,static_cast<int>(m_accordion->GetDisabledBrightness()) ) );
    m_pgCaptionTDisabledBrightness->SetEditor( "SpinCtrl" );
    m_pgCaptionTDisabledBrightness->SetAttribute(  wxT("Step"), (long)1 );
	m_pgCaptionTDisabledBrightness->SetAttribute(  wxT("MotionSpin"), true );
	m_pgCaptionTDisabledBrightness->SetAttribute(  wxPG_ATTR_MIN, (long)0);
    m_pgCaptionTDisabledBrightness->SetAttribute(  wxPG_ATTR_MAX, (long)255);
    m_pgCaptionTDisabledBrightness->Enable(m_accordion->GetDisabledState()!=wxAC_DISABLED_STATE_CUSTOM);

    //m_pgButtonCat
	m_pgButtonCat = m_propertyGrid->AppendIn( m_pgCaptionBarCat,new wxPropertyCategory( wxT("Buttons"), wxPG_LABEL ) );

	m_pgUseButtons = m_propertyGrid->Append( new wxBoolProperty( wxT("Use Buttons"), wxPG_LABEL ,false) );
    m_pgUseButtons->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );
    m_pgUseButtons->SetValue(false);

	m_pgExpButton = m_propertyGrid->Append( new wxImageFileProperty( wxT("Expand Button"), wxPG_LABEL ) );
    m_pgExpButton->SetAttribute(wxPG_FILE_DIALOG_TITLE,wxT("Expand Button Image"));
    m_pgExpButton->Enable(false);

	m_pgColButton = m_propertyGrid->Append( new wxImageFileProperty( wxT("Collapse Button"), wxPG_LABEL ) );
    m_pgColButton->SetAttribute(wxPG_FILE_DIALOG_TITLE,wxT("Collapse Button Image"));
    m_pgColButton->Enable(false);

    m_pgButtonPadding=m_propertyGrid->Append( new wxSizeProperty("Button Margin", wxPG_LABEL, m_accordion->GetButtonMargin()) );
    m_pgButtonPadding->Enable(false);

    //m_pgIconCat
	m_pgIconCat = m_propertyGrid->AppendIn( m_pgCaptionBarCat,new wxPropertyCategory( wxT("Icons"), wxPG_LABEL ) );

	m_pgShowIcons = m_propertyGrid->Append( new wxBoolProperty( wxT("Show Icon Area"), wxPG_LABEL, false ) );
	m_pgShowIcons->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgIconColour = m_propertyGrid->Append( new wxColourProperty( wxT("Icon Area Colour"), wxPG_LABEL,*wxBLACK ) );
    m_pgIconColour->Enable(false);

    m_pgIconSize = m_propertyGrid->Append( new wxSizeProperty("Icon Size",wxPG_LABEL,wxSize(16,16)));
    m_pgIconSize->Enable(false);

    m_pgIconPadding = m_propertyGrid->Append( new wxSizeProperty("Icon Margin",wxPG_LABEL,m_accordion->GetIconMargin()) );
    m_pgIconPadding->Enable(false);

    //m_pgColCapBarCat
	m_pgColCapBarCat = m_propertyGrid->AppendIn( m_pgCaptionBarCat, new wxPropertyCategory( wxT("Collapsed State"), wxPG_LABEL ) );

	m_pgColCapBarCol1 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 1"), wxPG_LABEL, m_accordion->GetCollapsedStyle().GetColour1() ) );

	m_pgColCapBarGradStops = m_propertyGrid->Append( new wxStringProperty( wxT("Gradient Stops"), wxPG_LABEL , "None"  ) );
    m_propertyGrid->SetPropertyReadOnly(m_pgColCapBarGradStops,true,wxPG_DONT_RECURSE);

	m_pgColCapBarCol2 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 2"), wxPG_LABEL, m_accordion->GetCollapsedStyle().GetColour2() ) );

	m_pgColCapBarUseImage = m_propertyGrid->Append( new wxBoolProperty( wxT("Use Image"), wxPG_LABEL,m_accordion->GetCollapsedStyle().GetBGBitmap().IsOk() ) );
	m_pgColCapBarUseImage->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgColCapBarImage = m_propertyGrid->Append( new wxImageFileProperty( wxT("Image"), wxPG_LABEL ) );
	m_pgColCapBarImage->Enable(false);

	m_pgColCapBarBrdrTrans = m_propertyGrid->Append( new wxBoolProperty( wxT("Transparent Border"), wxPG_LABEL,false ) );
	m_pgColCapBarBrdrTrans->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgColCapBarBrdrCol = m_propertyGrid->Append( new wxColourProperty( wxT("Border Colour"), wxPG_LABEL, m_accordion->GetCollapsedStyle().GetBorderColour() ) );

	m_pgColCapBarTextCol = m_propertyGrid->Append( new wxColourProperty( wxT("Text Colour"), wxPG_LABEL, m_accordion->GetCollapsedStyle().GetTextColour() ) );

	m_pgColCapBarFont = m_propertyGrid->Append( new wxFontProperty( wxT("Font"), wxPG_LABEL, m_accordion->GetCollapsedStyle().GetFont() ) );
    m_pgColCapBarFont->Item(0)->SetEditor( "SpinCtrl" );
    m_pgColCapBarFont->Item(0)->SetAttribute(  wxT("Step"), (long)1 );
    m_pgColCapBarFont->Item(0)->SetAttribute(  wxT("MotionSpin"), true );
    m_pgColCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MIN, (long)4);
    m_pgColCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MAX, (long)72);
    m_pgColCapBarFont->Item(4)->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

    //m_pgColHLCapBarCat
	m_pgColHLCapBarCat = m_propertyGrid->AppendIn( m_pgCaptionBarCat, new wxPropertyCategory( wxT("Collapsed Highlighted State"), wxPG_LABEL ) );

	m_pgColHLCapBarCol1 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 1"), wxPG_LABEL, m_accordion->GetCollapsedHLStyle().GetColour1() ) );

	m_pgColHLCapBarGradStops = m_propertyGrid->Append( new wxStringProperty( wxT("Gradient Stops"), wxPG_LABEL , "None"  ) );
    m_propertyGrid->SetPropertyReadOnly(m_pgColHLCapBarGradStops,true,wxPG_DONT_RECURSE);

	m_pgColHLCapBarCol2 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 2"), wxPG_LABEL, m_accordion->GetCollapsedHLStyle().GetColour2() ) );

	m_pgColHLCapBarUseImage = m_propertyGrid->Append( new wxBoolProperty( wxT("Use Image"), wxPG_LABEL,m_accordion->GetCollapsedHLStyle().GetBGBitmap().IsOk() ) );
	m_pgColHLCapBarUseImage->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgColHLCapBarImage = m_propertyGrid->Append( new wxImageFileProperty( wxT("Image"), wxPG_LABEL ) );
	m_pgColHLCapBarImage->Enable(false);

	m_pgColHLCapBarBrdrTrans = m_propertyGrid->Append( new wxBoolProperty( wxT("Transparent Border"), wxPG_LABEL,false ) );
	m_pgColHLCapBarBrdrTrans->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgColHLCapBarBrdrCol = m_propertyGrid->Append( new wxColourProperty( wxT("Border Colour"), wxPG_LABEL, m_accordion->GetCollapsedHLStyle().GetBorderColour() ) );

	m_pgColHLCapBarTextCol = m_propertyGrid->Append( new wxColourProperty( wxT("Text Colour"), wxPG_LABEL, m_accordion->GetCollapsedHLStyle().GetTextColour() ) );

	m_pgColHLCapBarFont = m_propertyGrid->Append( new wxFontProperty( wxT("Font"), wxPG_LABEL, m_accordion->GetCollapsedHLStyle().GetFont() ) );
    m_pgColHLCapBarFont->Item(0)->SetEditor( "SpinCtrl" );
    m_pgColHLCapBarFont->Item(0)->SetAttribute(  wxT("Step"), (long)1 );
    m_pgColHLCapBarFont->Item(0)->SetAttribute(  wxT("MotionSpin"), true );
    m_pgColHLCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MIN, (long)4);
    m_pgColHLCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MAX, (long)72);
    m_pgColHLCapBarFont->Item(4)->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

    if(m_accordion->GetHighlighting()!=wxAC_HIGHLIGHTING_MOUSEOVER)
    {
        m_pgColHLCapBarCat->Hide(true);
    }


    //m_pgExpCapBarCat
	m_pgExpCapBarCat = m_propertyGrid->AppendIn( m_pgCaptionBarCat, new wxPropertyCategory( wxT("Expanded State"), wxPG_LABEL ) );

	m_pgExpCapBarCol1 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 1"), wxPG_LABEL, m_accordion->GetExpandedStyle().GetColour1() ) );

	m_pgExpCapBarGradStops = m_propertyGrid->Append( new wxStringProperty( wxT("Gradient Stops"), wxPG_LABEL, "None" ) );
	m_propertyGrid->SetPropertyReadOnly(m_pgExpCapBarGradStops,true,wxPG_DONT_RECURSE);

	m_pgExpCapBarCol2 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 2"), wxPG_LABEL, m_accordion->GetExpandedStyle().GetColour2() ) );

	m_pgExpCapBarUseImage = m_propertyGrid->Append( new wxBoolProperty( wxT("Use Image"), wxPG_LABEL,m_accordion->GetExpandedStyle().GetBGBitmap().IsOk() ) );
	m_pgExpCapBarUseImage->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgExpCapBarImage = m_propertyGrid->Append( new wxImageFileProperty( wxT("Image"), wxPG_LABEL ) );
	m_pgExpCapBarImage->Enable(false);

	m_pgExpCapBarBrdrTrans = m_propertyGrid->Append( new wxBoolProperty( wxT("Transparent Border"), wxPG_LABEL ) );
	m_pgExpCapBarBrdrTrans->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgExpCapBarBrdrCol = m_propertyGrid->Append( new wxColourProperty( wxT("Border Colour"), wxPG_LABEL, m_accordion->GetExpandedStyle().GetBorderColour() ) );

	m_pgExpCapBarTextCol = m_propertyGrid->Append( new wxColourProperty( wxT("Text Colour"), wxPG_LABEL, m_accordion->GetExpandedStyle().GetTextColour() ) );

	m_pgExpCapBarFont = m_propertyGrid->Append( new wxFontProperty( wxT("Font"), wxPG_LABEL, m_accordion->GetExpandedStyle().GetFont() ) );
    m_pgExpCapBarFont->Item(0)->SetEditor( "SpinCtrl" );
    m_pgExpCapBarFont->Item(0)->SetAttribute(  wxT("Step"), (long)1 );
    m_pgExpCapBarFont->Item(0)->SetAttribute(  wxT("MotionSpin"), true );
    m_pgExpCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MIN, (long)4);
    m_pgExpCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MAX, (long)72);
    m_pgExpCapBarFont->Item(4)->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );


    //m_pgExpHLCapBarCat
	m_pgExpHLCapBarCat = m_propertyGrid->AppendIn( m_pgCaptionBarCat, new wxPropertyCategory( wxT("Expanded Highlighted State"), wxPG_LABEL ) );

	m_pgExpHLCapBarCol1 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 1"), wxPG_LABEL, m_accordion->GetExpandedHLStyle().GetColour1() ) );

	m_pgExpHLCapBarGradStops = m_propertyGrid->Append( new wxStringProperty( wxT("Gradient Stops"), wxPG_LABEL, "None" ) );
	m_propertyGrid->SetPropertyReadOnly(m_pgExpHLCapBarGradStops,true,wxPG_DONT_RECURSE);

	m_pgExpHLCapBarCol2 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 2"), wxPG_LABEL, m_accordion->GetExpandedHLStyle().GetColour2() ) );

	m_pgExpHLCapBarUseImage = m_propertyGrid->Append( new wxBoolProperty( wxT("Use Image"), wxPG_LABEL,m_accordion->GetExpandedHLStyle().GetBGBitmap().IsOk() ) );
	m_pgExpHLCapBarUseImage->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgExpHLCapBarImage = m_propertyGrid->Append( new wxImageFileProperty( wxT("Image"), wxPG_LABEL ) );
	m_pgExpHLCapBarImage->Enable(false);

	m_pgExpHLCapBarBrdrTrans = m_propertyGrid->Append( new wxBoolProperty( wxT("Transparent Border"), wxPG_LABEL ) );
	m_pgExpHLCapBarBrdrTrans->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgExpHLCapBarBrdrCol = m_propertyGrid->Append( new wxColourProperty( wxT("Border Colour"), wxPG_LABEL, m_accordion->GetExpandedHLStyle().GetBorderColour() ) );

	m_pgExpHLCapBarTextCol = m_propertyGrid->Append( new wxColourProperty( wxT("Text Colour"), wxPG_LABEL, m_accordion->GetExpandedHLStyle().GetTextColour() ) );

	m_pgExpHLCapBarFont = m_propertyGrid->Append( new wxFontProperty( wxT("Font"), wxPG_LABEL, m_accordion->GetExpandedHLStyle().GetFont() ) );
    m_pgExpHLCapBarFont->Item(0)->SetEditor( "SpinCtrl" );
    m_pgExpHLCapBarFont->Item(0)->SetAttribute(  wxT("Step"), (long)1 );
    m_pgExpHLCapBarFont->Item(0)->SetAttribute(  wxT("MotionSpin"), true );
    m_pgExpHLCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MIN, (long)4);
    m_pgExpHLCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MAX, (long)72);
    m_pgExpHLCapBarFont->Item(4)->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

    if(m_accordion->GetHighlighting()!=wxAC_HIGHLIGHTING_MOUSEOVER)
    {
        m_pgExpHLCapBarCat->Hide(true);
    }


    //m_pgDisCapBarCat
	m_pgDisCapBarCat = m_propertyGrid->AppendIn( m_pgCaptionBarCat,new wxPropertyCategory( wxT("Disabled State"), wxPG_LABEL ) );

	m_pgDisCapBarCol1 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 1"), wxPG_LABEL, m_accordion->GetDisabledStyle().GetColour1() ) );

	m_pgDisCapBarGradStops = m_propertyGrid->Append( new wxStringProperty( wxT("Gradient Stops"), wxPG_LABEL, "None" ) );
	m_propertyGrid->SetPropertyReadOnly(m_pgDisCapBarGradStops,true,wxPG_DONT_RECURSE);

	m_pgDisCapBarCol2 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 2"), wxPG_LABEL, m_accordion->GetDisabledStyle().GetColour2() ) );

	m_pgDisCapBarUseImage = m_propertyGrid->Append( new wxBoolProperty( wxT("Use Image"), wxPG_LABEL,m_accordion->GetDisabledStyle().GetBGBitmap().IsOk() ) );
	m_pgDisCapBarUseImage->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgDisCapBarImage = m_propertyGrid->Append( new wxImageFileProperty( wxT("Image"), wxPG_LABEL ) );
	m_pgDisCapBarImage->Enable(false);

	m_pgDisCapBarBrdrTrans = m_propertyGrid->Append( new wxBoolProperty( wxT("Transparent Border"), wxPG_LABEL ) );
	m_pgDisCapBarBrdrTrans->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgDisCapBarBrdrCol = m_propertyGrid->Append( new wxColourProperty( wxT("Border Colour"), wxPG_LABEL, m_accordion->GetDisabledStyle().GetBorderColour() ) );

	m_pgDisCapBarTextCol = m_propertyGrid->Append( new wxColourProperty( wxT("Text Colour"), wxPG_LABEL, m_accordion->GetDisabledStyle().GetTextColour() ) );

	m_pgDisCapBarFont = m_propertyGrid->Append( new wxFontProperty( wxT("Font"), wxPG_LABEL, m_accordion->GetDisabledStyle().GetFont() ) );
    m_pgDisCapBarFont->Item(0)->SetEditor( "SpinCtrl" );
    m_pgDisCapBarFont->Item(0)->SetAttribute(  wxT("Step"), (long)1 );
    m_pgDisCapBarFont->Item(0)->SetAttribute(  wxT("MotionSpin"), true );
    m_pgDisCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MIN, (long)4);
    m_pgDisCapBarFont->Item(0)->SetAttribute(  wxPG_ATTR_MAX, (long)72);
    m_pgDisCapBarFont->Item(4)->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

    if(m_accordion->GetDisabledState()!=wxAC_DISABLED_STATE_CUSTOM)
    {
        m_pgDisCapBarCat->Hide(true);
    }

    //m_pgPagesCat
	m_pgPagesCat = m_propertyGrid->Append( new wxPropertyCategory( wxT("Pages"), wxPG_LABEL ) );

	m_pgPagesPadding = m_propertyGrid->Append( new wxIntProperty( wxT("Padding"), wxPG_LABEL ) );
    m_pgPagesPadding->SetEditor( "SpinCtrl" );
    m_pgPagesPadding->SetAttribute(  wxT("Step"), (long)1 );
	m_pgPagesPadding->SetAttribute(  wxT("MotionSpin"), true );
	m_pgPagesPadding->SetAttribute(  wxPG_ATTR_MIN, (long)0);
    m_pgPagesPadding->SetValue( m_accordion->GetPagePadding() );

	m_pgPagesRadius = m_propertyGrid->Append( new wxIntProperty( wxT("Radius"), wxPG_LABEL ) );
    m_pgPagesRadius->SetEditor( "SpinCtrl" );
    m_pgPagesRadius->SetAttribute(  wxT("Step"), (long)1 );
	m_pgPagesRadius->SetAttribute(  wxT("MotionSpin"), true );
	m_pgPagesRadius->SetAttribute(  wxPG_ATTR_MIN, (long)0);
    m_pgPagesRadius->SetValue( static_cast<int>(m_accordion->GetPageRadius()) );

	m_pgPagesCol1 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 1"), wxPG_LABEL ,m_accordion->GetPageStyle().GetColour1()) );

	m_pgPagesGradStops = m_propertyGrid->Append( new wxStringProperty( wxT("Gradient Stops"), wxPG_LABEL,"None" ) );
    m_propertyGrid->SetPropertyReadOnly(m_pgPagesGradStops,true,wxPG_DONT_RECURSE);

	m_pgPagesCol2 = m_propertyGrid->Append( new wxColourProperty( wxT("Colour 2"), wxPG_LABEL,m_accordion->GetPageStyle().GetColour2() ) );

	m_pgPagesUseImage = m_propertyGrid->Append( new wxBoolProperty( wxT("Use Image"), wxPG_LABEL,m_accordion->GetPageStyle().GetBGBitmap().IsOk() ) );
	m_pgPagesUseImage->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgPagesImage = m_propertyGrid->Append( new wxImageFileProperty( wxT("Image"), wxPG_LABEL ) );
	m_pgPagesImage->Enable(false);

	m_pgPagesBrdrTrans = m_propertyGrid->Append( new wxBoolProperty( wxT("Transparent Border"), wxPG_LABEL,false ) );
    m_pgPagesBrdrTrans->SetAttribute (wxPG_BOOL_USE_CHECKBOX,true );

	m_pgPagesBrdrCol = m_propertyGrid->Append( new wxColourProperty( wxT("Border Colour"), wxPG_LABEL ,m_accordion->GetPageStyle().GetBorderColour()) );
}
