/////////////////////////////////////////////////////////////////////////////
// Name:        accordion.cpp
// Purpose:     Implementation of wxAccordion
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
#include <wx/panel.h>
#include <wx/sizer.h>
#endif

#include <wx/accordion/accordion.h>
#include <wx/dcbuffer.h>

#include <math.h>
#include <vector>

#define PI 3.14159265


#ifndef _DOXYGEN_
class wxAccordionFold:public wxPanel
{
    wxDECLARE_ABSTRACT_CLASS(wxAccordionFold);
    friend class wxAccordion;

    public:
        wxAccordionFold(wxAccordion *parent, const wxString & ,int,bool  );
        void Expand();
        void Collapse();
        int GetProportion();
        wxWindow* GetPage();
        void SetFixedHeight();
        void SetFixedWidth();
        void SetCaptionHeight(int);
        void SetBorder(int b);
        void SetWindowMargin(int);
        void SetWindowSpacer(int);
        void SetHighlighting(bool);

    private:
		void onMouse( wxMouseEvent& event );
		void onMouseMotion( wxMouseEvent& event );
		void onMouseLeave( wxMouseEvent& event );
		void onPaint( wxPaintEvent& event );

        wxAccordion* m_accordion;
        wxRect m_curTextRect;
        wxString m_caption;
        int m_image;

        bool m_isCollapsed;
        bool m_isFixedWidth;
        bool m_isFixedHeight;
        bool m_isEnabled;
        bool m_isHighlighted;
        bool m_useHighlighting;
};

wxIMPLEMENT_ABSTRACT_CLASS(wxAccordionFold, wxPanel)

wxAccordionFold::wxAccordionFold(wxAccordion *parent, const wxString & s,int im,bool  b)
    :wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE),
    m_accordion(parent),m_caption(s),m_image(im),m_isCollapsed(b),m_curTextRect(),
    m_isFixedWidth(false),m_isFixedHeight(false),m_isEnabled(true),
    m_isHighlighted(false),m_useHighlighting(false)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	Bind( wxEVT_PAINT,   &wxAccordionFold::onPaint  ,this );
	//Forward most of the mouse events to be handled by the accordion
    Bind( wxEVT_LEFT_DCLICK,   &wxAccordionFold::onMouse, this );
    Bind( wxEVT_LEFT_DOWN,     &wxAccordionFold::onMouse, this );
    Bind( wxEVT_LEFT_UP,       &wxAccordionFold::onMouse, this );
    Bind( wxEVT_MIDDLE_DCLICK, &wxAccordionFold::onMouse, this );
    Bind( wxEVT_MIDDLE_DOWN,   &wxAccordionFold::onMouse, this );
    Bind( wxEVT_MIDDLE_UP,     &wxAccordionFold::onMouse, this );
    Bind( wxEVT_RIGHT_DCLICK,  &wxAccordionFold::onMouse, this );
    Bind( wxEVT_RIGHT_DOWN,    &wxAccordionFold::onMouse, this );
    Bind( wxEVT_RIGHT_UP,      &wxAccordionFold::onMouse, this );
    Bind( wxEVT_AUX1_DCLICK,   &wxAccordionFold::onMouse, this );
    Bind( wxEVT_AUX1_DOWN,     &wxAccordionFold::onMouse, this );
    Bind( wxEVT_AUX1_UP,       &wxAccordionFold::onMouse, this );
    Bind( wxEVT_AUX2_DCLICK,   &wxAccordionFold::onMouse, this );
    Bind( wxEVT_AUX2_DOWN,     &wxAccordionFold::onMouse, this );
    Bind( wxEVT_AUX2_UP,       &wxAccordionFold::onMouse, this );
    Bind( wxEVT_MOTION,        &wxAccordionFold::onMouseMotion, this );
    Bind( wxEVT_LEAVE_WINDOW,  &wxAccordionFold::onMouseLeave, this );
}

void wxAccordionFold::Expand()
{
    m_isCollapsed=false;
    GetSizer()->GetItem(1)->Show(true);
    GetSizer()->GetItem(2)->Show(true);
    GetContainingSizer()->GetItem(this)->SetProportion(m_isFixedHeight?0:1);
}

void wxAccordionFold::Collapse()
{
    m_isCollapsed=true;
    GetSizer()->GetItem(1)->Show(false);
    GetSizer()->GetItem(2)->Show(false);
    GetContainingSizer()->GetItem(this)->SetProportion(0);
}

void wxAccordionFold::SetFixedWidth()
{
    m_isFixedWidth=true;
    GetSizer()->GetItem(2)->SetFlag(GetSizer()->GetItem(2)->GetFlag()&~wxEXPAND);
}

void wxAccordionFold::SetFixedHeight()
{
    GetContainingSizer()->GetItem(this)->SetProportion(0);
    m_isFixedHeight=true;
    GetSizer()->GetItem(2)->SetProportion(0);
}

int wxAccordionFold::GetProportion()
{
    return GetContainingSizer()->GetItem(this)->GetProportion();
}

void wxAccordionFold::SetCaptionHeight(int captionBarHeight)
{
    GetSizer()->GetItem(size_t(0))->AssignSpacer(0,captionBarHeight);
}

wxWindow* wxAccordionFold::GetPage()
{
    return GetSizer()->GetItem(2)->GetWindow();
}

void wxAccordionFold::SetBorder(int b)
{
    GetContainingSizer()->GetItem(this)->SetBorder(b);
}

void wxAccordionFold::SetWindowMargin(int windowMargin)
{
    GetSizer()->GetItem(2)->SetBorder(windowMargin);
}

void wxAccordionFold::SetWindowSpacer(int newheight)
{
    bool b=GetSizer()->GetItem(1)->IsShown();
    GetSizer()->GetItem(1)->AssignSpacer(0, newheight);
    GetSizer()->GetItem(1)->Show(b);
}

void wxAccordionFold::SetHighlighting(bool b)
{
    m_useHighlighting=b;
}

void wxAccordionFold::onMouse( wxMouseEvent& event )
{
    //repackage the event and forward it to the accordion
    event.SetX(event.GetX()+GetRect().x);
    event.SetY(event.GetY()+GetRect().y);
    event.SetEventObject(m_accordion);
    event.SetId(m_accordion->GetId());
    m_accordion->ProcessWindowEvent(event);
}

void wxAccordionFold::onMouseLeave( wxMouseEvent& WXUNUSED(event) )
{
    if(m_useHighlighting)
    {
        bool b = m_isHighlighted;
        m_isHighlighted=false;

        if(b)
        {
            Refresh();
        }
    }
}

void wxAccordionFold::onMouseMotion( wxMouseEvent& event )
{
    if(m_useHighlighting)
    {
        bool b = m_isHighlighted;

        if(event.GetY() < m_accordion->m_captionBarHeight)
        {
            m_isHighlighted=true;

            if(!b)
            {
                Refresh();
            }
        }
        else
        {
            m_isHighlighted=false;

            if(b)
            {
                Refresh();
            }
        }
    }

    //repackage the event and forward it to the accordion
    event.SetX(event.GetX()+GetRect().x);
    event.SetY(event.GetY()+GetRect().y);
    event.SetEventObject(m_accordion);
    event.SetId(m_accordion->GetId());
    m_accordion->ProcessWindowEvent(event);
}

void wxAccordionFold::onPaint( wxPaintEvent& WXUNUSED(event) )
{
    if( m_accordion->changesWereMade() )
    {
        m_accordion->computeCaptionInfo();
    }

    wxAutoBufferedPaintDC myDC(this);
    myDC.Clear();
    wxPoint textPoint;
    wxColour borderColour;

    if(m_isCollapsed)
    {
        if(m_isEnabled)
        {
            if(m_isHighlighted)
            {
                myDC.DrawBitmap(m_accordion->m_captionBarCollapsedHL,0,0 ,true);
                myDC.SetFont(m_accordion->m_collapsedHLStyle.m_font);
                myDC.SetTextForeground(m_accordion->m_collapsedHLStyle.m_textColour);

                borderColour = m_accordion->m_collapsedHLStyle.m_borderColour;
                textPoint = m_accordion->m_collapsedHLTextPoint;
            }
            else
            {
                myDC.DrawBitmap(m_accordion->m_captionBarCollapsed,0,0 ,true);
                myDC.SetFont( m_accordion->m_collapsedStyle.m_font );
                myDC.SetTextForeground( m_accordion->m_collapsedStyle.m_textColour );

                borderColour = m_accordion->m_collapsedStyle.m_borderColour;
                textPoint = m_accordion->m_collapsedTextPoint;
            }
        }
        else
        {
            myDC.DrawBitmap(m_accordion->m_captionBarDisabled,0,0 ,true);
            myDC.SetFont(m_accordion->m_disabledStyle.m_font);
            myDC.SetTextForeground(m_accordion->m_disabledStyle.m_textColour);

            if(m_accordion->m_disabledState==wxAC_DISABLED_STATE_CUSTOM)
            {
                borderColour = m_accordion->m_disabledStyle.m_borderColour;
            }
            else
            {
                borderColour = m_accordion->m_collapsedStyle.m_borderColour;
                borderColour.MakeDisabled(m_accordion->m_disabledBrightness);
            }

            textPoint = m_accordion->m_disabledTextPoint;
        }

        //the collapsed versions don't have a bottom border since
        //if the page margin is zero,
        //we would get 2 borders drawn right next to each other.
        //In all other situations, we want the bottom border, so
        //we need to add it back:

        if( m_accordion->m_accordionPadding!=0 && borderColour != wxTransparentColour )
        {
            int width = m_accordion->m_captionBarCollapsed.GetWidth();
            int height  = m_accordion->m_captionBarCollapsed.GetHeight();
            myDC.SetPen(borderColour);

            myDC.DrawLine(m_accordion->m_captionBarRadius+1,height-1,width-(m_accordion->m_captionBarRadius)-1,height-1);
        }
    }
    else
    {
        if(m_isHighlighted)
        {
            myDC.DrawBitmap(m_accordion->m_captionBarExpandedHL,0,0 ,true);
            myDC.SetFont(m_accordion->m_expandedHLStyle.m_font);
            myDC.SetTextForeground(m_accordion->m_expandedHLStyle.m_textColour);

            borderColour = m_accordion->m_expandedHLStyle.m_borderColour;
            textPoint = m_accordion->m_expandedHLTextPoint;
        }
        else
        {
            myDC.DrawBitmap(m_accordion->m_captionBarExpanded,0,0 ,true);
            myDC.SetFont(m_accordion->m_expandedStyle.m_font);
            myDC.SetTextForeground(m_accordion->m_expandedStyle.m_textColour);

            borderColour = m_accordion->m_expandedStyle.m_borderColour;
            textPoint = m_accordion->m_expandedTextPoint;
        }
    }

    if(m_image!=wxAccordion::NO_IMAGE)
    {
        if( wxImageList* il = m_accordion->GetImageList() )
        {
            il->Draw(m_image, myDC, m_accordion->m_iconRect.x, m_accordion->m_iconRect.y, wxIMAGELIST_DRAW_TRANSPARENT);
        }
    }

    wxString text=wxControl::Ellipsize(m_caption,myDC,wxELLIPSIZE_MIDDLE,m_accordion->m_textWidth,wxELLIPSIZE_FLAGS_NONE );
    m_curTextRect = wxRect( textPoint, myDC.GetTextExtent(text) );
    myDC.DrawText(text, textPoint);

    if(m_isCollapsed)
    {
        myDC.DrawBitmap(m_accordion->m_expandButton,m_accordion->m_buttonRect.GetTopLeft() ,true);
    }
    else
    {
        myDC.DrawBitmap(m_accordion->m_collapseButton,m_accordion->m_buttonRect.GetTopLeft() ,true);

        //Now we need to draw the page's background.
        //Unfortunately, we can't cache a bitmap like we did for the caption bar and so
        //we have to build the background here because there's no way of knowing the
        //height ahead of time.

        wxBitmap temp;
        int width(GetRect().GetWidth()),height(GetRect().GetHeight()-m_accordion->m_captionBarHeight);
        m_accordion->buildBitmap(temp, true, 2,  width, height,  0, 0, 0, height,m_accordion->m_pageRadius,m_accordion->m_pageStyle);
        myDC.DrawBitmap(temp,0,m_accordion->m_captionBarHeight,true);
    }
}
#endif


/////////////////////////////////////////////////
//// wxAccordionStyle
//////////////////////////////////////////////////

/// \brief Default constructor.
///
///
wxAccordionStyle::wxAccordionStyle()
:m_stops(*wxWHITE,*wxWHITE),m_borderColour(*wxBLACK),m_background(wxNullBitmap),
 m_textColour(*wxBLACK),m_font(*wxNORMAL_FONT),m_accordion(NULL),m_isPageStyle(false)
{
}

/// \brief Returns the first (top) colour.
///
///
wxColour wxAccordionStyle::GetColour1() const{return m_stops.GetStartColour();}

/// \brief 	Returns a copy of the gradient stops used for this style.
///
///
wxGraphicsGradientStops wxAccordionStyle::GetGradientStops() const{return m_stops;}

/// \brief Returns the second (bottom) colour.
///
///
wxColour wxAccordionStyle::GetColour2() const{return m_stops.GetEndColour();}

/// \brief Returns a copy of the background bitmap used for this style.
///
///
wxBitmap wxAccordionStyle::GetBGBitmap() const{return m_background;}

/// \brief Returns the border colour.
///
///
wxColour wxAccordionStyle::GetBorderColour() const{return m_borderColour;}

/// \brief Returns the text colour.
///
///
wxColour wxAccordionStyle::GetTextColour() const{return m_textColour;}

/// \brief Returns a copy of the font used for this style.
///
///
wxFont   wxAccordionStyle::GetFont() const{return m_font;}

/// \brief Sets the first (top) colour.
///
///
void wxAccordionStyle::SetColour1(const wxColour& col)
{
    if( m_stops.GetStartColour() != col )
    {
        m_stops.SetStartColour(col);
        if(m_accordion!=NULL) m_accordion->m_changesMade=true;
    }
}

/// \brief Clears any gradient stops that have been set.
///
///
void wxAccordionStyle::ClearGradientStops()
{
    m_stops=wxGraphicsGradientStops(m_stops.GetStartColour(), m_stops.GetEndColour());
    if(m_accordion!=NULL) m_accordion->m_changesMade=true;
}

/// \brief Adds a gradient stop to the set of colours used for this style.
///
///
void wxAccordionStyle::AddGradientStop(const wxColour& col,float pos)
{
    m_stops.Add( col, pos);
    if(m_accordion!=NULL) m_accordion->m_changesMade=true;
}

/// \brief Sets the second (bottom) colour.
///
///
void wxAccordionStyle::SetColour2(const wxColour& col)
{
    if( m_stops.GetEndColour() != col )
    {
        m_stops.SetEndColour(col);
        if(m_accordion!=NULL) m_accordion->m_changesMade=true;
    }

}

/// \brief Sets the background bitmap used for this style.
///
///
void wxAccordionStyle::SetBGBitmap(const wxBitmap& b)
{
    m_background=b;
    if(m_accordion!=NULL) m_accordion->m_changesMade=true;
}

/// \brief Sets the border colour.
///
/// wxTransparentColour can be used to make the border invisible.
void wxAccordionStyle::SetBorderColour(const wxColour& col)
{
    if(m_isPageStyle)
    {
        if(m_accordion!=NULL) m_accordion->setPageBorderColourHelper(col);
    }
    else if( m_borderColour != col )
    {
        m_borderColour=col;
        if(m_accordion!=NULL) m_accordion->m_changesMade=true;
    }
}

/// \brief Sets the text colour.
///
///
void wxAccordionStyle::SetTextColour(const wxColour& col)
{
    if( m_textColour != col )
    {
        m_textColour=col;
        if(m_accordion!=NULL) m_accordion->m_changesMade=true;
    }
}

/// \brief Sets the font used for this style.
///
///
void wxAccordionStyle::SetFont(const wxFont& f)
{
    if( m_font != f )
    {
        m_font=f;
        if(m_accordion!=NULL) m_accordion->m_changesMade=true;
    }

    m_font=f;
}

/////////////////////////////////////////////////
//// wxAccordion
//////////////////////////////////////////////////



wxIMPLEMENT_CLASS(wxAccordion, wxBookCtrlBase)
wxDEFINE_EVENT(wxEVT_ACCORDION_COLLAPSED, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_ACCORDION_COLLAPSING, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_ACCORDION_EXPANDED, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_ACCORDION_EXPANDING, wxBookCtrlEvent);

/// \brief Default constructor.
///
///
wxAccordion::wxAccordion()
{
    Init();
}

/// \brief Constructor.
///
/// \param parent	The parent window.
/// \param id	    An identifier for the accordion. wxID_ANY is taken to mean a default.
/// \param pos	    The accordion position. The value wxDefaultPosition indicates a default position, chosen by either the windowing system or wxWidgets, depending on platform.
/// \param size	    The accordion size. The value wxDefaultSize indicates a default size, chosen by either the windowing system or wxWidgets, depending on platform.
/// \param style	The accordion style.
/// \param name	    The window name.
/// \sa Create
///
wxAccordion::wxAccordion( wxWindow* parent, wxWindowID id , const wxPoint& pos , const wxSize& size , long style, const wxString &name )
{
    Init();
    Create(parent, id, pos, size, style, name);
}

/// \brief Destructor.
///
/// Deletes any child windows before deleting the physical window.
///
wxAccordion::~wxAccordion()
{
    //?
}

/// \brief Used for two-step accordion construction.
///
/// See wxAccordion() for details.
///
bool wxAccordion::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString &name)
{
    if (!wxBookCtrlBase::Create(parent, id, pos, size, style,name))
        return false;

    InitAccordion(style);

    return true;
}

void wxAccordion::Init()
{
    m_selection=wxNOT_FOUND;
    m_trackedImageList=NULL;

    m_accordionPadding=5;
    m_toggleStyle=wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS;
    m_pagePadding=5;
    m_minCaptionHeight=20;
    m_iconMargin=wxSize(5,5);
    m_textMargin=wxSize(5,5);
    m_buttonMargin=wxSize(5,5);
    m_disabledState=wxAC_DISABLED_STATE_DEFAULT;
    m_highlighting=wxAC_HIGHLIGHTING_NONE;
    m_disabledBrightness=60;
    m_collapseButton=wxNullBitmap;
    m_expandButton=wxNullBitmap;
    m_accordionBG=wxNullBitmap;
    m_captionBarRadius=0.0;
    m_gradientAngle=0.0;
    m_captionBorderStyle=wxAC_BORDERS_FULL;
    m_pageRadius=0.0;

    m_collapsedStyle.m_accordion    = this;
    m_collapsedStyle.m_stops        = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTION ),wxSystemSettings::GetColour( wxSYS_COLOUR_GRADIENTINACTIVECAPTION  ));
    m_collapsedStyle.m_borderColour = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME );
    m_collapsedStyle.m_background   = wxNullBitmap;
    m_collapsedStyle.m_textColour   = wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTIONTEXT );
    m_collapsedStyle.m_font         = GetFont();

    m_expandedStyle.m_accordion    = this;
    m_expandedStyle.m_stops        = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ),wxSystemSettings::GetColour( wxSYS_COLOUR_GRADIENTACTIVECAPTION  ));
    m_expandedStyle.m_borderColour = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME );
    m_expandedStyle.m_background   = wxNullBitmap;
    m_expandedStyle.m_textColour   = wxSystemSettings::GetColour( wxSYS_COLOUR_CAPTIONTEXT );
    m_expandedStyle.m_font         = GetFont();

    m_collapsedHLStyle.m_accordion    = this;
    m_collapsedHLStyle.m_stops        = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTION ),wxSystemSettings::GetColour( wxSYS_COLOUR_GRADIENTINACTIVECAPTION  ));
    m_collapsedHLStyle.m_borderColour = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME );
    m_collapsedHLStyle.m_background   = wxNullBitmap;
    m_collapsedHLStyle.m_textColour   = wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTIONTEXT );
    m_collapsedHLStyle.m_font         = GetFont();

    m_disabledStyle.m_accordion    = this;
    m_disabledStyle.m_stops        = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_FRAMEBK  ),wxSystemSettings::GetColour( wxSYS_COLOUR_FRAMEBK   ));
    m_disabledStyle.m_borderColour = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME );
    m_disabledStyle.m_background   = wxNullBitmap;
    m_disabledStyle.m_textColour   = wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT );
    m_disabledStyle.m_font         = GetFont();

    m_pageStyle.m_accordion    = this;
    m_pageStyle.m_stops        = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_FRAMEBK ),wxSystemSettings::GetColour( wxSYS_COLOUR_FRAMEBK ));
    m_pageStyle.m_borderColour = wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER );
    m_pageStyle.m_background   = wxNullBitmap;
    m_pageStyle.m_isPageStyle  = true;

    computeCaptionInfo();
}

void wxAccordion::InitAccordion(long style)
{
    SetBackgroundColour( GetParent()->GetBackgroundColour() );

    //3 things are added at the beginning
    //  1) a sizer of fixed proportion that serves as the top margin for the
    //     first page (the panels are all added with flags LEFT|BOTTOM|RIGHT.
    //     we could add the first one with flags WXALL, but then if a new
    //     panel moves to the first position, we would need to adjust its flags
    //     and the old first panels flags.  It's earsier to use the same flags
    //     for all panels and just have a spacer at the top)
    //  2) a sizer of variable proportion that serves to push items to the
    //     bottom when the wxACCORDION_COLLAPSE_TO_BOTTOM style is used
    //     (99% of the time, we could just use 1 spacer for both purposes, the
    //     only time doing so would be bad is if we were using the wxACCORDION_COLLAPSE_TO_BOTTOM
    //     style and the heights of the panesl was just a little taller than the height
    //     available, if we only had 1 variable height spacer, we would lose the top margin.
    //     the first fixed height spacer keeps this from happening.  I think I'll keep
    //     the 2 spacers even though both are rarely needed).
    //  3) a panel on which the last page's bottom border can
    //     be drawn the accordion margin is 0 and the last page is collapsed.

    wxBoxSizer* b_sizer = new wxBoxSizer( wxVERTICAL );

    b_sizer->Add( 0, m_accordionPadding, 0, 0, 0 );

    if(style&wxAC_COLLAPSE_TO_BOTTOM)
    {
        b_sizer->Add( 0, 0, 1, 0, 0 );
    }
    else
    {
        b_sizer->Add( 0, 0, 0, 0, 0 );
    }

    m_lastBorder = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize(-1,1), wxFULL_REPAINT_ON_RESIZE );
    m_lastBorder->SetBackgroundStyle(wxBG_STYLE_PAINT);
    m_lastBorder->SetForegroundColour( m_collapsedStyle.m_borderColour );
    m_lastBorder->Hide();
    b_sizer->Add( m_lastBorder,0, wxEXPAND, 0 );

    SetSizer( b_sizer );

    m_lastBorder->Bind( wxEVT_PAINT,   &wxAccordion::onPaintBorder  ,this );
    Bind( wxEVT_SIZE,        &wxAccordion::onSize,       this );
    Bind( wxEVT_LEFT_UP,     &wxAccordion::onLeftUp,     this );
    Bind( wxEVT_LEFT_DCLICK, &wxAccordion::onLeftDClick, this );
}

void wxAccordion::onPaintBorder( wxPaintEvent& WXUNUSED(event) )
{
    wxAutoBufferedPaintDC myDC(m_lastBorder);
    myDC.Clear();

    int width = m_captionBarCollapsed.GetWidth();
    myDC.SetPen( m_lastBorder->GetForegroundColour() );
    myDC.DrawLine(m_captionBarRadius,0,width-m_captionBarRadius,0);
}

void wxAccordion::onPaint( wxPaintEvent& WXUNUSED(event) )
{
    wxAutoBufferedPaintDC myDC(this);

    myDC.Clear();

    int width = GetSize().GetWidth();
    int height = GetSize().GetHeight();
    int bmpwidth = m_accordionBG.GetWidth();
    int bmpheight = m_accordionBG.GetHeight();

    for(int i=0;i*bmpheight<height;i++)
    {
        for(int j=0;j*bmpwidth<width;j++)
        {
            myDC.DrawBitmap(m_accordionBG,j*bmpwidth,i*bmpheight,true);
        }
    }
}

void wxAccordion::onLeftDClick( wxMouseEvent& event )
{
    long flags;
    int fold = HitTest(event.GetPosition(),&flags);

    //In order to work well with onLeftUp, the behavior we want is as follows:

    //if( IsDisabled(fold) )
    //{
    //    return;
    //}
    //else if( IsCollapsed(fold) )
    //{
    //    if(expandButton.IsOk())
    //    {
    //        if(!onlyToggleWithButton ||  (flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON) )
    //        {
    //            //this will be handled by the second call to onLeftUp, so do nothing here
    //        }
    //        else
    //        {
    //            SetSelection(fold);
    //        }
    //    }
    //    else
    //    {
    //        //do nothing here
    //    }
    //}
    //else if( flags&wxACCORDION_HITTEST_ONCAPTION )
    //{
    //    if(collapseButton.IsOk())
    //    {
    //        if( !onlyToggleWithButton ||  (flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON ) )
    //        {
    //            //do nothing here
    //        }
    //        else
    //        {
    //            SetSelection(fold);
    //        }
    //    }
    //    else
    //    {
    //        //do nothing here
    //    }
    //}

    //I think this is a shorter way of doing the above:
    if( IsDisabled(fold) )
    {
        return;
    }
    else if( IsCollapsed(fold) && m_expandButton.IsOk() && !( !(m_toggleStyle==wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS) ||  (flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON)) )
    {
        SetSelection(fold);
    }
    else if(flags&wxACCORDION_HITTEST_ONCAPTION && m_collapseButton.IsOk() && !( !(m_toggleStyle==wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS) ||  (flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON)) )
    {
        SetSelection(fold);
    }
}

void wxAccordion::onLeftUp( wxMouseEvent& event )
{
    //The fold behavior I want is as follows:
    //-If there's no button or onlyToggleWithButton is set false,
    // any click on the caption bar will toggle the fold
    //-Otherwise, only double clicks or
    // clicks in the button area will toggle the fold.

    long flags;
    int fold = HitTest(event.GetPosition(),&flags);

    //This is oftly convoluted, but it gives the fold behavior described above
    if( IsDisabled(fold) )
    {
        return;
    }
    else if( IsCollapsed(fold) )
    {
        if(m_expandButton.IsOk())
        {
            if(!(m_toggleStyle==wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS) ||  (flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON) )
            {
                SetSelection(fold);
            }
        }
        else
        {
            SetSelection(fold);
        }
    }
    else if( flags&wxACCORDION_HITTEST_ONCAPTION )
    {
        if(m_collapseButton.IsOk())
        {
            if( !(m_toggleStyle==wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS) || flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON )
            {
                SetSelection(fold);
            }
        }
        else
        {
            SetSelection(fold);
        }
    }
}

bool wxAccordion::SetBackgroundColour(const wxColour &colour)
{
    for(size_t i=0;i<GetPageCount();i++)
    {
        getFold(i)->SetBackgroundColour(colour);
    }

    return wxBookCtrlBase::SetBackgroundColour(colour);
}

void wxAccordion::SetWindowStyleFlag(long style)
{
    //should layout be done here?
    if( style != GetWindowStyleFlag())
    {
        m_changesMade=true;
    }

    adjustForStyle( style );

    //if wxAC_FLOAT_TO_TOP or wxAC_COLLAPSE_TO_BOTTOM
    //are being removed, we need to take some extra steps:
    if( (GetWindowStyleFlag()&wxAC_FLOAT_TO_TOP) && !(style & wxAC_FLOAT_TO_TOP) )
    {
        //We need to sink the floated items, so first make a vector
        //copy of the folds:
        wxSizer* szr = GetSizer();
        std::vector<wxAccordionFold*> folds;

        for(size_t i=0;i<GetPageCount();i++)
        {
            folds.push_back(getFold(i));
        }

        //now remove all the folds from the sizer:
        for(std::vector<wxAccordionFold*>::iterator it = folds.begin();it!=folds.end();it++)
        {
            szr->Detach(*it);
        }
        szr->Detach(m_lastBorder);


        //add back all folds:
        int proportion;
        wxAccordionFold* fold;
        for(std::vector<wxAccordionFold*>::iterator it = folds.begin();it!=folds.end();it++)
        {
            fold = *it;

            if( fold->m_isCollapsed || fold->m_isFixedHeight )
            {
                proportion=0;
            }
            else
            {
                proportion=1;
            }

            szr->Add( fold, proportion, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, m_accordionPadding );
            //lastFold=fold;
        }
        szr->Add( m_lastBorder, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 0 );

    }

    if( !(style & wxAC_COLLAPSE_TO_BOTTOM) )
    {
        setSecondSpacerProp(0);
    }

    wxBookCtrlBase::SetWindowStyleFlag(style);
}

bool wxAccordion::changesWereMade()
{
    //this is an ugly hackaround intended to help with the fact that
    //AssignImageList isn't virtual, so I can't just set m_changesMade=true
    //there.  Without this extra check, refresh and layout won't detect
    //changes made to the imagelist, and so won't call computeCaptionInfo()
    //when the imagelist was changed.

    if( m_trackedImageList != GetImageList() )
    {
        m_trackedImageList=GetImageList();
        return true;
    }
    else if(m_changesMade)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool wxAccordion::Layout()
{
    if( changesWereMade() ) computeCaptionInfo();
    return wxBookCtrlBase::Layout();
}

void wxAccordion::Refresh( bool eraseBackground, const wxRect *rect)
{
    if( changesWereMade() ) computeCaptionInfo();
    wxBookCtrlBase::Refresh(eraseBackground,rect);
}

void wxAccordion::onSize( wxSizeEvent& event )
{
    m_changesMade=true;
    event.Skip();
}

/// \brief Calculates which (if any) item is under the given point, returning the tree item id at this point plus extra information flags.
///
/// \param pt The point to check.
/// \param flags A long pointer that will be set as described below.
/// \return The (zero based) index of the page under pt or wxNOT_FOUND if none.  Upon return, flags is set as a bitlist of the following:
/// \li wxACCORDION_HITTEST_NOWHERE: In the client area but above the first item or below the last item.</li>
/// \li wxACCORDION_HITTEST_ONCAPTION: On the caption bar.</li>
/// \li wxACCORDION_HITTEST_ONCAPTIONBORDER: On the border of the caption bar.</li>
/// \li wxACCORDION_HITTEST_ONCAPTIONLABEL: On the label (string) associated with an item.</li>
/// \li wxACCORDION_HITTEST_ONCAPTIONICON: On the bitmap associated with an item.</li>
/// \li wxACCORDION_HITTEST_ONCAPTIONBUTTON: On the button associated with an item.</li>
/// \li wxACCORDION_HITTEST_ONPAGE: On the page contained by the caption bar.</li>
/// \li wxACCORDION_HITTEST_ONPAGEBORDER: On the border of the page contained by the caption bar.</li>
/// \li wxACCORDION_HITTEST_INPAGEPADDING: In the padding  of the page contained by the caption bar.</li>
int wxAccordion::HitTest( const wxPoint& pt, long* flags ) const
{
    if(flags) *flags = 0;
    int found(wxNOT_FOUND);

    for(size_t i=0;i<GetPageCount();i++)
    {
        wxAccordionFold* fold = getFold(i);
        wxRect r = fold->GetRect();

        if( !r.Contains(pt) )
        {
            continue;
        }

        //Yay! We found the point.
        found=i;

        if(flags==NULL)
        {
            break;
        }

        //Is it on the caption bar?
        if(pt.y-r.y<m_captionBarHeight)
        {
            *flags = *flags | wxACCORDION_HITTEST_ONCAPTION;

            if(IsExpanded(i))
            {
                if(m_expandedStyle.m_borderColour!=wxTransparentColour)
                {
                    if(pt.y==r.y || pt.x ==r.x || pt.y==r.y +m_captionBarHeight-1 || pt.x ==r.GetRight()  )
                    {
                        *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONBORDER;
                    }
                }
            }
            else
            {
                if(m_collapsedStyle.m_borderColour!=wxTransparentColour)
                {
                    if(pt.y==r.y || pt.x ==r.x || pt.y==r.y +m_captionBarHeight-1 || pt.x ==r.GetRight()  )
                    {
                        *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONBORDER;
                    }
                }
            }

            if(  m_buttonRect.Contains(wxPoint(pt.x-r.x,pt.y-r.y)))
            {
                *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONBUTTON;
            }

            if(  m_iconRect.Contains(wxPoint(pt.x-r.x,pt.y-r.y)))
            {
                *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONICON;
            }

            if(  fold->m_curTextRect.Contains(wxPoint(pt.x-r.x,pt.y-r.y)))
            {
                *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONLABEL;
            }
        }
        else //if it's not on the caption bar, it has to be on the window
        {
            *flags = *flags | wxACCORDION_HITTEST_ONPAGE;

            if(m_pagePadding>0)
            {
                if( pt.x==r.x || pt.y==r.GetBottom() ||pt.x==r.GetRight()   )
                {

                    if(m_pageStyle.m_borderColour!=wxTransparentColour)
                    {
                        *flags = *flags | wxACCORDION_HITTEST_ONPAGEBORDER;
                    }
                    else
                    {
                        *flags = *flags | wxACCORDION_HITTEST_INPAGEPADDING;
                    }
                }
                else
                {
                    if
                    (
                        //if the border is transparent, we can use assume there
                        //are pagePadding pixels on the top.
                        //Otherwise there are pagePadding-1 pixels.
                        pt.x-r.x<m_pagePadding ||
                        pt.y-(r.y+m_captionBarHeight)< (m_pageStyle.m_borderColour==wxTransparentColour?m_pagePadding:m_pagePadding-1) ||
                        r.GetBottom()-pt.y<m_pagePadding ||
                        r.GetRight()-pt.x<m_pagePadding
                    )
                    {
                        *flags = *flags | wxACCORDION_HITTEST_INPAGEPADDING;
                    }
                }
            }
        }
        break;
    }

    if(found == wxNOT_FOUND  && flags != NULL)
    {
        *flags = *flags | wxACCORDION_HITTEST_NOWHERE;
    }

    return found;
}

void wxAccordion::computeCaptionInfo()
{
    //In this method, we need to do 3 things
    //1) compute the current captionBarHeight.
    //2) if that value is different from the old value,
    //   we need to adjust all the caption bars
    //3) build bitmaps that can be used for caption bar backgrounds
    //   when they need to be drawn.

    m_changesMade=false;
    int oldHeight = m_captionBarHeight;

    //First find the needed captionBarHeight and the
    //rectangles for the button, icon, and text.  Because there are
    //so many possibilities, this is a slightly complicated process.

    wxMemoryDC memDC;
    memDC.SetFont(m_expandedStyle.m_font);
    int expandedTextHeight = memDC.GetFontMetrics().height;
    memDC.SetFont(m_collapsedStyle.m_font);
    int collapsedTextHeight = memDC.GetFontMetrics().height;
    memDC.SetFont(m_collapsedHLStyle.m_font);
    int collapsedHLTextHeight = memDC.GetFontMetrics().height;
    memDC.SetFont(m_expandedHLStyle.m_font);
    int expandedHLTextHeight = memDC.GetFontMetrics().height;
    memDC.SetFont(m_disabledStyle.m_font);
    int disabledTextHeight = memDC.GetFontMetrics().height;

    int textHeight = expandedTextHeight;
    if(collapsedTextHeight>textHeight)
    {
        textHeight = collapsedTextHeight;
    }
    if(collapsedHLTextHeight>textHeight)
    {
        textHeight = collapsedHLTextHeight;
    }
    if(expandedHLTextHeight>textHeight)
    {
        textHeight = expandedHLTextHeight;
    }
    if(disabledTextHeight>textHeight)
    {
        textHeight = disabledTextHeight;
    }

    int totalTextHeight = textHeight + 2*m_textMargin.GetHeight();

    int iconWidth(0),iconHeight(0),totalIconHeight(0);
    if(wxImageList* il=	GetImageList())
    {
        il->GetSize(0, iconWidth, iconHeight);
        totalIconHeight = iconHeight + 2*m_iconMargin.GetHeight();
    }

    int collapseButtonHeight = m_collapseButton.GetHeight();
    int expandButtonHeight = m_expandButton.GetHeight();
    int buttonHeight = collapseButtonHeight>expandButtonHeight?collapseButtonHeight:expandButtonHeight;

    int collapseButtonWidth = m_collapseButton.GetWidth();
    int expandButtonWidth = m_expandButton.GetWidth();
    int buttonWidth  = collapseButtonWidth>expandButtonWidth?collapseButtonWidth:expandButtonWidth;

    int totalButtonHeight = buttonHeight+ 2*m_buttonMargin.GetHeight();

    m_captionBarHeight=m_minCaptionHeight;

    if(totalTextHeight>m_captionBarHeight)
    {
        m_captionBarHeight=totalTextHeight;
    }
    if(totalIconHeight>m_captionBarHeight)
    {
        m_captionBarHeight=totalIconHeight;
    }
    if(totalButtonHeight>m_captionBarHeight)
    {
        m_captionBarHeight=totalButtonHeight;
    }

    wxRect r=GetRect();

    int butnLeftPos(0),iconLeftPos(0),textLeftPos(0);
    m_textWidth = r.GetWidth()
                  - 2*m_textMargin.GetWidth()
                  - 2*m_accordionPadding
                  - buttonWidth
                  - iconWidth
                  - (buttonWidth!=0?m_buttonMargin.GetWidth():0)
                  - (iconWidth!=0?m_iconMargin.GetWidth():0)
                  ;

    if(GetWindowStyle() & wxAC_BUTTON_LEFT)
    {
        butnLeftPos = m_buttonMargin.GetWidth();
        iconLeftPos = buttonWidth+(buttonWidth!=0?m_buttonMargin.GetWidth():0)
                        + m_iconMargin.GetWidth();
        textLeftPos = buttonWidth+(buttonWidth!=0?m_buttonMargin.GetWidth():0)
                        + iconWidth  +(iconWidth!=0?m_iconMargin.GetWidth():0)
                        + m_textMargin.GetWidth();
    }
    else
    {
        butnLeftPos = r.GetWidth()-2*m_accordionPadding-buttonWidth-m_buttonMargin.GetWidth();
        iconLeftPos = m_iconMargin.GetWidth();
        textLeftPos = iconWidth+(iconWidth!=0?m_iconMargin.GetWidth():0) + m_textMargin.GetWidth();
    }

    m_buttonRect           = wxRect(butnLeftPos, (m_captionBarHeight-buttonHeight)/2,        buttonWidth, buttonHeight);
    m_iconRect             = wxRect(iconLeftPos, (m_captionBarHeight-iconHeight)/2,          iconWidth,   iconHeight);
    m_collapsedTextPoint   = wxPoint(textLeftPos, (m_captionBarHeight-collapsedTextHeight)/2);
    m_expandedTextPoint    = wxPoint(textLeftPos, (m_captionBarHeight-expandedTextHeight)/2);
    m_collapsedHLTextPoint = wxPoint(textLeftPos, (m_captionBarHeight-collapsedHLTextHeight)/2);
    m_expandedHLTextPoint  = wxPoint(textLeftPos, (m_captionBarHeight-expandedHLTextHeight)/2);
    m_disabledTextPoint    = wxPoint(textLeftPos, (m_captionBarHeight-disabledTextHeight)/2);

    //Part2: if the height has changed, we need to adjust the height allocated
    //for any pages already in the accordion.
    if(m_captionBarHeight!=oldHeight)
    {
        for(size_t i=0;i<GetPageCount();i++)
        {
            getFold(i)->SetCaptionHeight(m_captionBarHeight);
        }
    }

    //Part3: build bitmaps for the caption bar backgrounnds
    int width(GetRect().GetWidth()-2*m_accordionPadding);
    int height = m_captionBarHeight;

    double alpha=m_gradientAngle;

    //a bunch of rigamarole to make sure -180<alpha<=180
    if(alpha>0)
    {
        alpha =fmod(alpha, 360.0);
    }
    else if(alpha<0)
    {
        alpha= -1*fmod(-1*alpha ,360) + 360;
    }

    alpha=alpha>180?alpha-360:alpha;

    alpha=alpha==-180?180:alpha;

    //Now we need to know how to draw the gradient depending on alpha
    double x1,x2,y1,y2;

    if(-180 < alpha && alpha < -90)
    {
        double beta = -1*alpha-90;
        double radbeta=beta* PI / 180.0;

        double ab=(double) width;
        double bg=(double) height;

        double bd=tan(radbeta)*bg;
        double ad = ab+bd;
        double af = cos(radbeta)*ad;
        double ac = cos(radbeta)*af;
        double cf = sin(radbeta)*af;

        x1=ac;
        y1=cf;
        x2=0;
        y2=0;
    }
    else if( alpha ==-90 )
    {
        x1=width;
        y1=0;
        x2=0;
        y2=0;
    }
    else if( -90 < alpha && alpha < 0)
    {
        double beta = -1*alpha;
        double radbeta=beta* PI / 180.0;

        double ab=(double) height;
        double bg=(double) width;

        double hg = tan(radbeta)*bg;
        double bd=hg;
        double ad=ab+bd;
        double af=cos(radbeta)*ad;
        double ac=cos(radbeta)*af;
        double cf=sin(radbeta)*af;

        x1=cf;
        y1=ab-ac;
        x2=0;
        y2=ab;
    }
    else if( alpha == 0 )
    {
        x1=0;
        y1=0;
        x2=0;
        y2=height;
    }
    else if(0 < alpha && alpha < 90)
    {
        double radalpha=alpha* PI / 180.0;


        double ab=(double) height;
        double bg=(double) width;

        double hg = tan(radalpha)*bg;
        double bd=hg;
        double ad=ab+bd;
        double af=cos(radalpha)*ad;
        double ac=cos(radalpha)*af;
        double cf=sin(radalpha)*af;

        x1=0;
        y1=0;
        x2=cf;
        y2=ac;
    }
    else if( alpha == 90)
    {
        x1=0;
        y1=0;
        x2=width;
        y2=0;
    }
    else if(90 < alpha && alpha < 180)
    {
        double beta = alpha-90;
        double radbeta=beta* PI / 180.0;

        double ab=(double) width;
        double bg=(double) height;

        double hg = tan(radbeta)*bg;
        double bd=hg;
        double ad=ab+bd;
        double af=cos(radbeta)*ad;
        double ac=cos(radbeta)*af;
        double cf=sin(radbeta)*af;

        x1=0;
        y1=bg;
        x2=ac;
        y2=bg-cf;
    }
    else //alpha == 180 (up to floating point error)
    {
        x1=0;
        y1=height;
        x2=0;
        y2=0;
    }

    //We're finally ready to build the bitmaps
    //bitmap for a collapsed caption bar:

    buildBitmap(   m_captionBarCollapsed, false, 0, width, height, x1, y1, x2, y2,m_captionBarRadius,m_collapsedStyle);
    buildBitmap(    m_captionBarExpanded,  (m_captionBorderStyle==wxAC_BORDERS_FULL), 1, width, height, x1, y1, x2, y2,m_captionBarRadius,m_expandedStyle);
    if(m_highlighting==wxAC_HIGHLIGHTING_MOUSEOVER)
    {
        buildBitmap( m_captionBarCollapsedHL, false, 0, width, height, x1, y1, x2, y2,m_captionBarRadius,m_collapsedHLStyle);
        buildBitmap(  m_captionBarExpandedHL,  (m_captionBorderStyle==wxAC_BORDERS_FULL), 1, width, height, x1, y1, x2, y2,m_captionBarRadius,m_expandedHLStyle);
    }
    else
    {
        m_captionBarCollapsedHL=m_captionBarCollapsed;
        m_captionBarExpandedHL=m_captionBarExpanded;
    }

    if(m_disabledState==wxAC_DISABLED_STATE_CUSTOM)
    {
        buildBitmap(    m_captionBarDisabled, false, 0, width, height, x1, y1, x2, y2,m_captionBarRadius,m_disabledStyle);
    }
    else
    {
        //The obvious thing to do here is:
        //m_captionBarDisabled=m_captionBarDisabled.ConvertToDisabled(disabledBrightness);
        //but that seems to mess up the mask used when the corners are rounded.  So,
        //I'll just convert all the colors to disabled and build a new bitmap with
        //those colors.

        wxColour c1 = m_collapsedStyle.m_stops.GetStartColour().MakeDisabled(m_disabledBrightness);
        wxColour c2 = m_collapsedStyle.m_stops.GetEndColour().MakeDisabled(m_disabledBrightness);

        wxAccordionStyle adhocStyle;
        adhocStyle.m_stops = wxGraphicsGradientStops(c1, c2);

        wxColor bc;
        for(size_t i=1;i<m_collapsedStyle.m_stops.GetCount()-1;i++)
        {
            bc=m_collapsedStyle.m_stops.Item(i).GetColour();
            bc.MakeDisabled(m_disabledBrightness);
            adhocStyle.m_stops.Add( bc, m_collapsedStyle.m_stops.Item(i).GetPosition() );
        }

        adhocStyle.m_borderColour = m_collapsedStyle.m_borderColour;
        adhocStyle.m_borderColour.MakeDisabled(m_disabledBrightness);
        adhocStyle.m_background = m_collapsedStyle.m_background.ConvertToDisabled(m_disabledBrightness);

        buildBitmap(    m_captionBarDisabled, false, 0, width, height, x1, y1, x2, y2,m_captionBarRadius,adhocStyle);
    }
}

void wxAccordion::buildBitmap(wxBitmap& out, bool bottomBorder, int drawtype, int width,int height, double x1,double y1,double x2,double y2, double radius, const wxAccordionStyle& style)
{
    //buildBitmap(out, bottomBorder, drawtype, width ,height, x1, y1, x2, y2,  radius, style.m_borderColour,wxNullBitmap,style.m_stops );

    //todo: in certain situations, we can, take some steps to
    //speed things up:
    //  -if stops only has 2 entries and they're both transparent, do nothing
    //  -if stops only has 2 entries, but their the same, just fill the window
    //  -if stops only has 2 different entries - use wxDC::GradientFillLinear instead
    //  -in all cases, if radius==0, we can just use
    //     a regular rectangle instead of a rounded rectangle.
    //  -only if all of those fail should we go through the whole process.

    const wxColour& borderColour = style.m_borderColour;
    const wxBitmap& bgBitmap = style.m_background;
    const wxGraphicsGradientStops& stops = style.m_stops;



    bool roundTopOnly(drawtype==1),roundBottomOnly(drawtype==2);

    wxMemoryDC memDC;

    wxBitmap b1(width, height,1);
    memDC.SelectObject(b1);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.SetPen(*wxBLACK_PEN);
    memDC.SetBrush(*wxBLACK_BRUSH);
    memDC.Clear();
    memDC.DrawRoundedRectangle(wxRect(0,0,width,height),radius);
    if(roundTopOnly)
    {
        memDC.DrawRectangle(wxRect(0,height/2,width,height));
    }
    if(roundBottomOnly)
    {
        memDC.DrawRectangle(wxRect(0,0,width,height/2));
    }

    wxBitmap b3(width, height,1);
    wxBitmap b4(width, height);
    if(borderColour!=wxTransparentColour)
    {
        memDC.SelectObject(b3);
        memDC.SetBrush(*wxWHITE_BRUSH);
        memDC.Clear();
        memDC.DrawRoundedRectangle(wxRect(0,0,width,height),radius);

        if(roundTopOnly)
        {
            memDC.SetPen(*wxWHITE_PEN);
            memDC.DrawRectangle(wxRect(0,height/2,width,height));
            memDC.SetPen(*wxBLACK_PEN);
            memDC.DrawLine(0,height/2,0,height);
            memDC.DrawLine(width-1,height/2,width-1,height);

            if(bottomBorder)
            {
                memDC.DrawLine(0,height-1,width-1,height-1);
            }
        }

        if(roundBottomOnly)
        {
            memDC.SetPen(*wxWHITE_PEN);
            memDC.DrawRectangle(wxRect(0,0,width,height/2));
            memDC.SetPen(*wxBLACK_PEN);
            memDC.DrawLine(0,0,0,height/2);
            memDC.DrawLine(width-1,0,width-1,height/2);
        }

        //In most situations we want the bottom border, but there
        //are a few situations where we wont want it.
        //In situations where we want the border (which is actually most
        //of the time), it's just easier to draw it then than it would be do
        //draw the bottom border here and then remove it when we don't want it.

        if(!bottomBorder)
        {
            memDC.SetPen(*wxWHITE_PEN);
            int cbr = static_cast<int>(radius+.5);
            memDC.DrawLine(cbr+1,height-1,width-cbr-1,height-1);
        }

        memDC.SelectObject(b4);
        memDC.SetBackground(wxBrush(borderColour));
        memDC.Clear();
        b4.SetMask( new wxMask(b3,wxColour(255,255,255)) );
    }

    out=wxBitmap(width, height);
    memDC.SelectObject(out);

    if(wxGraphicsContext *gc = wxGraphicsContext::Create( memDC ))
    {
        gc->SetBrush(gc->CreateLinearGradientBrush(x1, y1,x2, y2,stops));

        wxRect gfr(0,0,width,height);
        wxGraphicsPath pth = gc->CreatePath();
        pth.MoveToPoint(gfr.x,gfr.y);
        pth.AddLineToPoint(gfr.x + gfr.width,gfr.y);
        pth.AddLineToPoint(gfr.x + gfr.width,gfr.y+gfr.height);
        pth.AddLineToPoint(gfr.x,gfr.y+gfr.height);
        pth.CloseSubpath();
        gc->FillPath(pth);

        delete gc;
    }

    if(bgBitmap.IsOk())
    {
        int bmpwidth = bgBitmap.GetWidth();
        int bmpheight = bgBitmap.GetHeight();

        for(int i=0;i*bmpheight<height;i++)
        {
            for(int j=0;j*bmpwidth<width;j++)
            {
                memDC.DrawBitmap(bgBitmap,j*bmpwidth,i*bmpheight,true);
            }
        }
    }

    if(borderColour!=wxTransparentColour)
    {
        memDC.DrawBitmap(b4,0,0,true);
    }
    out.SetMask(new wxMask(b1,wxColour(255,255,255)));
}

/// \brief Sets the accordion page with index n to have the enabled state.
///
/// \sa Disable, IsEnabled
void wxAccordion::Enable(size_t n)
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        fold->m_isEnabled=true;
        fold->Refresh();

        m_lastBorder->SetForegroundColour(m_collapsedStyle.m_borderColour);
        m_lastBorder->Refresh();
    }
}


/// \brief Sets the accordion page with index n to have the disabled state.
///
/// The caption bar will be drawn as a grayed version of the collapsed state
/// by default or using the disabled style properties if SetDisabledState(wxAC_DISABLED_STATE_CUSTOM)
/// been called.  If the page was expanded, it will be collapsed.
/// \sa Enable, IsDisabled, SetDisabledBrightness, SetDisabledState
void wxAccordion::Disable(size_t n)
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        fold->m_isEnabled=false;
        fold->Refresh();

        if(m_disabledState==wxAC_DISABLED_STATE_CUSTOM)
        {
            m_lastBorder->SetForegroundColour(m_disabledStyle.m_borderColour);
        }
        else
        {
            wxColour c = m_collapsedStyle.m_borderColour;
            m_lastBorder->SetForegroundColour( c.MakeDisabled(m_disabledBrightness) );
        }
        m_lastBorder->Refresh();
    }
    Collapse(n);

}

/// \brief Checks if the accordion page with index n is in the enabled state.
///
/// Note that this will return false if n is not a valid page index.
/// \sa IsDisabled
bool wxAccordion::IsEnabled(size_t n) const
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        return fold->m_isEnabled;
    }
    else
    {
        return false;
    }
}

/// \brief Checks if the accordion page with index n is in the disabled state.
///
/// Note that this will return false if n is not a valid page index.
/// \sa IsEnabled
bool wxAccordion::IsDisabled(size_t n) const
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        return !fold->m_isEnabled;
    }
    else
    {
        return false;
    }
}


/// \brief Checks if the accordion page with index n is expanded.
///
/// Note that this will return false if n is not a valid page index.
/// \sa IsCollapsed
bool wxAccordion::IsExpanded(size_t n) const
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        return !(fold->m_isCollapsed);
    }
    else
    {
        return false;
    }
}


/// \brief Checks if the accordion page with index n is collapsed.
///
/// Note that this will return false if n is not a valid page index.
/// \sa IsExpanded
bool wxAccordion::IsCollapsed(size_t n) const
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        return fold->m_isCollapsed;
    }
    else
    {
        return false;
    }
}

/// \brief Expands the accordion page with index n to show its contents.
///
/// If the page is already expanded or n is not a valid page index, this has no effect.
/// \sa IsExpanded, Collapse, Toggle
void wxAccordion::Expand(size_t n)
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        if( fold->m_isCollapsed )
        {
            toggle(fold,n,false);
        }
    }
}

/// \brief Collapses the accordion page with index n to hide its contents.
///
/// If the page is already collapsed or n is not a valid page index, this has no effect.
/// \sa IsCollapsed, Expand, Toggle
void wxAccordion::Collapse(size_t n)
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        if( !fold->m_isCollapsed )
        {
            toggle( fold, n,false);
        }
    }
}

/// \brief Toggles the accordion page with index n between the expanded and collapsed states.
///
/// If n is not a valid page index, this has no effect.
/// \sa Collapse, Expand
void wxAccordion::Toggle(size_t n)
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        toggle(fold,n,false);
    }
}

/// \brief Sets accordion page with index n to have a fixed width.
///
/// By default, the window added to the accordion page will be stretched to
/// be as wide as the accordion (minus any padding).  If the window is meant
/// to have a fixed width (for example, a listbox), this function can be used.
///
/// Note that there isn't any way to undo this other than removing the page and
/// then adding it back.
/// \sa SetFixedHeight, SetFixedSize
void wxAccordion::SetFixedWidth(size_t n)
{
    if(wxAccordionFold* fold = getFold(n))
    {
        fold->SetFixedWidth();
    }
}

/// \brief Sets accordion page with index n to have a fixed height.
///
/// By default, the window added to the accordion page will be stretched to
/// use as much vertical space as the accordion has available.  If the window is meant
/// to have a fixed height (for example, a single line text control), this function can be used.
///
/// Note that there isn't any way to undo this other than removing the page and
/// then adding it back.
/// \sa SetFixedWidth, SetFixedSize
void wxAccordion::SetFixedHeight(size_t n)
{
    if(wxAccordionFold* fold = getFold(n))
    {
        fold->SetFixedHeight();
    }
}

/// \brief Sets accordion page with index n to have both a fixed width and a fixed height.
///
/// By default, the window added to the accordion page will be stretched to
/// be stretched both vertically and horizontally.  If the window is meant
/// to have both a fixed width and a fixed height (for example, a button), this function can be used.
///
/// Note that there isn't any way to undo this other than removing the page and
/// then adding it back.
/// \sa SetFixedHeight, SetFixedWidth
void wxAccordion::SetFixedSize(size_t n)
{
    if(wxAccordionFold* fold = getFold(n))
    {
        fold->SetFixedHeight();
        fold->SetFixedWidth();
    }
}

/// \brief Returns the current height of the caption bars in the accordion.
///
/// The caption bar is set to have the smallest height needed for the text,
/// the icons (if used), and the buttons (if used) plus any margins for those items.
int wxAccordion::GetCaptionBarHeight() const
{
    return m_captionBarHeight;
}

void wxAccordion::toggle(wxAccordionFold* fold,size_t selection,bool useEvents)
{
    if(useEvents)
    {
        wxBookCtrlEvent event2(fold->m_isCollapsed?wxEVT_ACCORDION_EXPANDING:wxEVT_ACCORDION_COLLAPSING, GetId(), selection, m_selection);
        event2.SetEventObject(this);
        ProcessWindowEvent(event2);

        if(!event2.IsAllowed())
        {
            return;
        }
    }

    Freeze();

    if(fold->m_isCollapsed)
    {
        fold->Expand();
    }
    else
    {
        fold->Collapse();
    }

    int oldSelection = m_selection;
    m_selection=selection;

    adjustForStyle( GetWindowStyleFlag() );

    if( m_accordionPadding==0 )
    {
        if( fold == getFold(m_pages.size()-1) )
        {
            if(fold->m_isCollapsed && m_lastBorder->GetForegroundColour()!=wxTransparentColor)
            {
                m_lastBorder->Show();
            }
            else
            {
                m_lastBorder->Hide();
            }
        }
    }

    Layout();
    Thaw();

    if(useEvents)
    {
        wxBookCtrlEvent event3(fold->m_isCollapsed?wxEVT_ACCORDION_COLLAPSED:wxEVT_ACCORDION_EXPANDED, GetId(), selection, oldSelection);
        event3.SetEventObject(this);
        ProcessWindowEvent(event3);
    }
}

void wxAccordion::adjustForStyle( long style )
{
    if( style & wxAC_SINGLE_FOLD )
    {
        for(size_t i=0;i<GetPageCount();i++)
        {
            if( static_cast<int>(i)!=m_selection )
            {
                getFold(i)->Collapse();
            }
        }
    }

    if( style & wxAC_FLOAT_TO_TOP )
    {
        rebuildFloats();
    }

    if( style & wxAC_COLLAPSE_TO_BOTTOM )
    {
        //if everything has proportion 0, we want to set the spacer at item 0 to have
        //proportion 1 to push everything to the bottom.

        int newProportion(1);

        for(size_t i=0;i<GetPageCount();i++)
        {
            if(getFold(i)->GetProportion()!=0)
            {
                newProportion=0;
                break;
            }
        }

        setSecondSpacerProp(newProportion);
    }
}

wxAccordionFold* wxAccordion::getFold(size_t n) const
{
    wxSizer* szr = GetSizer();

    if( n >= m_pages.GetCount() )
    {
        return NULL;
    }

    if( GetWindowStyleFlag() & wxAC_FLOAT_TO_TOP )
    {
        //If wxAC_FLOAT_TO_TOP is active, the pages may not be in order
        //in the sizer since expanded pages are moved to the top.  We'll just have
        //to look at all the sizer items and find one whose window is m_pages[n]:

        wxSizer* szr = GetSizer();
        wxAccordionFold* found = NULL;

        for( size_t i = 2; i<szr->GetItemCount(); i++)
        {
            if( wxAccordionFold* fold = wxDynamicCast(szr->GetItem(i)->GetWindow(),wxAccordionFold))
            {
                if( fold->GetPage()==m_pages[n] )
                {
                    found = fold;
                    break;
                }
            }
        }

        return found;
    }
    else
    {
        //item 1 is a spacer used with the collapse to bottom style
        //item 0 is a spacer that servers as the top page margin
        //for the first item.  So the n-th fold, is the n+2th sizer item

        if( wxAccordionFold* fold = wxDynamicCast(szr->GetItem(n+2)->GetWindow(),wxAccordionFold) )
        {
            return fold;
        }
        else
        {
            return NULL;
        }

        //Theoretically it's possible for a user to add things to the accordion's sizer
        //on their own so that the sizer's items and m_pages don't match.
        //What I should do is iterate over the sizer's items find the one
        //that can be casted to a wxAccordionFold and whose data is m_pages[page]
        //as above.  However, I'll just do this for now.
    }
}

bool wxAccordion::SetPageText(size_t n, const wxString& strText)
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        fold->m_caption = strText;
        fold->Refresh();
        return true;
    }
    else
    {
        return false;
    }
}

wxString wxAccordion::GetPageText(size_t n) const
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        return fold->m_caption;
    }
    else
    {
        return wxEmptyString;
    }
}

int wxAccordion::GetPageImage(size_t n) const
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        return fold->m_image;
    }
    else
    {
        return NO_IMAGE;
    }
}

bool wxAccordion::SetPageImage(size_t n, int imageId)
{
    if( wxAccordionFold* fold = getFold(n) )
    {
        fold->m_image=imageId;
        fold->Refresh();
        return true;
    }
    else
    {
        return false;
    }
}

int wxAccordion::SetSelection(size_t n)
{
    int old = m_selection;

    if( wxAccordionFold* fold = getFold(n) )
    {
        toggle(fold,n,true);
    }

    return old;
}

int wxAccordion::ChangeSelection(size_t n)
{
    int old = m_selection;
    Toggle(n);
    return old;
}

wxWindow* wxAccordion::DoRemovePage(size_t page)
{
    wxWindow* win(NULL);

    if(wxAccordionFold* fold = getFold(page) )
    {
        Freeze();
        GetSizer()->Detach(fold);
        wxWindow* win = fold->GetPage();
        //somewhere we should check that m_pages[page]==fold->GetPage()
        //since if not, something has gone very wrong.
        //But then again, I'm not sure what could be done if they don't match.
        win->Reparent(this);
        fold->Destroy();
        if( static_cast<int>(page)==m_selection )
        {
            m_selection=wxNOT_FOUND;
        }
        Layout();
        Thaw();
    }

    if( page < m_pages.GetCount() )
    {
        m_pages.RemoveAt(page);
    }

    return win;
}

void wxAccordion::setSecondSpacerProp(int p)
{
    GetSizer()->GetItem(1)->SetProportion(p);
}

void wxAccordion::setFirstSpacerHeight(int h)
{
    GetSizer()->GetItem((size_t)0)->AssignSpacer(0,h);
}

void wxAccordion::rebuildFloats()
{
    //first make a vector copy of the folds:
    wxSizer* szr = GetSizer();
    std::vector<wxAccordionFold*> folds;

    for(size_t i=0;i<GetPageCount();i++)
    {
        folds.push_back(getFold(i));
    }

    //now remove all the folds from the sizer:
    for(std::vector<wxAccordionFold*>::iterator it = folds.begin();it!=folds.end();it++)
    {
        szr->Detach(*it);
    }
    szr->Detach(m_lastBorder);

    //add back all expanded folds:
    long flags( wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT );
    int proportion(0);
    wxAccordionFold* fold;

    for(std::vector<wxAccordionFold*>::iterator it = folds.begin();it!=folds.end();it++)
    {
        fold = *it;

        if(!fold->m_isCollapsed)
        {
            proportion = fold->m_isFixedHeight?0:1;

            szr->Add( fold, proportion, flags, m_accordionPadding );
        }
    }

    //add add all the collapsed items back:
    for(std::vector<wxAccordionFold*>::iterator it = folds.begin();it!=folds.end();it++)
    {
        fold = *it;

        if(fold->m_isCollapsed)
        {
            szr->Add( fold, 0, flags, m_accordionPadding );
        }
    }

    //add the last border back
    szr->Add( m_lastBorder, 0, flags, 0 );
}

void wxAccordion::setPageBorderColourHelper(const wxColour& col)
{
    //Since the top border for the window isn't drawn, there is a spacer 1 pixel
    //less than the margin when the border colour is not transparent and
    //equal to the margin when the border colour is transparent.
    //So if we're changing to or from a transparent colour, all of those spacers need
    //to be adjusted.

    if(col==m_pageStyle.m_borderColour)
    {
        return;
    }

    wxColour old = m_pageStyle.m_borderColour;
    bool adjust(false);
    int newheight(0);

    m_pageStyle.m_borderColour=col;
    m_changesMade=true;

    if(m_pagePadding!=0)
    {
        if(old == wxTransparentColour && col != wxTransparentColour)
        {
            adjust=true;
            newheight=m_pagePadding-1;
        }
        else if(old != wxTransparentColour && col == wxTransparentColour)
        {
            adjust=true;
            newheight=m_pagePadding;
        }
    }

    if(adjust)
    {
        for(size_t i=0;i<GetPageCount();i++)
        {
            getFold(i)->SetWindowSpacer(newheight);
        }
    }
}

bool wxAccordion::InsertPage(size_t n, wxWindow *page, const wxString& text, bool bSelect, int imageId )
{
    if(wxSizer* szr = GetSizer())
    {
        Freeze();

        wxAccordionFold* fold = new wxAccordionFold( this , text,imageId,!bSelect);
        wxBoxSizer* bSizer3 = new wxBoxSizer( wxVERTICAL );
        long flags( wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT );

        int proportion(0);

        if (n >= m_pages.GetCount())
        {
            m_pages.Add(page);
        }
        else
        {
            m_pages.Insert(page, n);
        }

        page->Reparent(fold);

        fold->SetBackgroundColour( GetBackgroundColour() );
        fold->m_useHighlighting=(m_highlighting==wxAC_HIGHLIGHTING_MOUSEOVER);

        bSizer3->Add( 0, m_captionBarHeight);
        //Since there is no drawn top border for the window, if
        //we just add page with flags wxALL, there will be an
        //extra pixel on the top.  So I'll add a spacer of size
        //pagePadding-1 and use flags wxBOTTOM|wxLEFT|wxRIGHT

        if(m_pagePadding>0)
        {
            bSizer3->Add( 0, (m_pageStyle.m_borderColour==wxTransparentColour?m_pagePadding:m_pagePadding-1));
        }
        else
        {
            bSizer3->Add( 0, 0);
        }

        bSizer3->Add( page, 1, wxEXPAND|wxBOTTOM|wxLEFT|wxRIGHT, m_pagePadding );

        if(bSelect)
        {
            proportion=1;
            m_selection=n;

            setSecondSpacerProp(0);
        }
        else
        {
            bSizer3->GetItem(1)->Show(false);
            bSizer3->GetItem(2)->Show(false);
        }
        fold->SetSizer( bSizer3 );

        if( GetWindowStyleFlag() & wxAC_FLOAT_TO_TOP )
        {
            //rather than trying to figure out where to put it, just
            //dump it on the end and then call the rebuildFloats function
            //to place everything correctly.
            szr->Add( fold, proportion, flags, m_accordionPadding );
            rebuildFloats();
        }
        else
        {
            szr->Detach(m_lastBorder);

            if( n >= m_pages.GetCount()-1 )
            {
                szr->Add( fold, proportion, flags, m_accordionPadding );
            }
            else
            {
                //item 0 is a spacer used with the collapse to bottom style
                //item 1 is a spacer that servers as the top page margin
                //for the first item.  So to add the n-th fold, we have to
                //add it at sizer position n+2
                szr->Insert( n+2, fold, proportion, flags, m_accordionPadding );
            }

            szr->Add( m_lastBorder, 0, flags, 0 );
        }

        Layout();
        Thaw();

        return true;
    }
    else
    {
        return false;
    }
}



/////////////////////////////////////////////////
////  Accordion style get/set functions
//////////////////////////////////////////////////

/// \brief Returns the accordion padding size (in pixels).
///
/// The accordion padding is the gap between the edge of the accordion and
/// the pages.  It's also the gap between pages.
/// \sa SetAccordionPadding
int      wxAccordion::GetAccordionPadding()        const {return m_accordionPadding;}


/// \brief Returns a copy of the bitmap being used for the accordion background.
///
/// \sa SetAccordionBGBitmap
wxBitmap wxAccordion::GetAccordionBGBitmap()       const {return m_accordionBG;}

/// \brief Returns the current toggle style of the accordion.
///
/// The return value will be either wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS or
/// wxTOGGLE_ON_ANY_CLICK
/// \sa SetToggleStyle
wxAccordionToggleStyle wxAccordion::GetToggleStyle() const{return m_toggleStyle;}

/// \brief Sets the accordion padding size (in pixels).
///
/// The accordion padding is the gap between the edge of the accordion and
/// the pages.  It's also the gap between pages.
/// \sa GetAccordionPadding
void wxAccordion::SetAccordionPadding(int m)
{
    int old = m_accordionPadding;
    m_accordionPadding=m>=0?m:0;

    if(m_accordionPadding!=old)
    {
        if( m_accordionPadding==0 && m_pages.size()>0 && m_collapsedStyle.m_borderColour!=wxTransparentColor )
        {
            m_lastBorder->Show();
        }
        else
        {
            m_lastBorder->Hide();
        }

        m_changesMade = true;

        setFirstSpacerHeight(m);

        for(size_t i=0;i<GetPageCount();i++)
        {
            getFold(i)->SetBorder(m);
        }
    }
}

/// \brief Sets the bitmap to be used for the accordion background.
///
/// To clear the bitmap, call this function with wxNullBitmap as the parameter.
/// \sa GetAccordionBGBitmap
void wxAccordion::SetAccordionBGBitmap(const wxBitmap& b)
{
    //I'm just going to assume that this is a different bitmap than
    //what was already set.
    m_changesMade=true;
    m_accordionBG = b;

    if(b.IsOk())
    {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT,&wxAccordion::onPaint,this);
    }
    else
    {
        SetBackgroundStyle(wxBG_STYLE_SYSTEM);
        Unbind(wxEVT_PAINT,&wxAccordion::onPaint,this);
    }
}

/// \brief Sets the toggle style of the accordion.
///
/// The parameter toggleStyle can be either wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS or wxTOGGLE_ON_ANY_CLICK.
///
/// This setting will only be used if expand and collapse button have been set for the accordion.
/// If buttons have not been set, the accordion will always toggle in response to a click on the caption bar.
/// \sa GetToggleStyle
void wxAccordion::SetToggleStyle(wxAccordionToggleStyle toggleStyle){m_toggleStyle=toggleStyle;}

/// \brief Returns the radius of the accordion's pages (in pixels).
///
/// \sa SetPageRadius
double   wxAccordion::GetPageRadius()              const {return m_pageRadius;}

/// \brief Returns the padding (in pixels) of the accordion's pages.
///
/// The page padding is the space between the edge of the page and the edge of the window it contains.
/// \sa SetPagePadding
int      wxAccordion::GetPagePadding()             const {return m_pagePadding;}

/// \brief Sets the radius of the accordion's pages (in pixels).
///
/// This determines the radius used for the bottom corners of a page when it is expanded.  The radius of the top corners are set with SetCaptionRadius().
/// \sa SetCaptionRadius, GetPageRadius
void     wxAccordion::SetPageRadius(double d){m_pageRadius=d>=0?d:0;m_changesMade=true;}

/// \brief Sets the padding (in pixels) of the accordion's pages.
///
/// The page padding is the space between the edge of the page and the edge of the window it contains.
/// \sa GetPagePadding
void wxAccordion::SetPagePadding(int m)
{
    int old = m_pagePadding;
    m_pagePadding=m>=0?m:0;

    if(m_pagePadding==old)
    {
        return;
    }

    m_changesMade = true;

    //See the notes for SetWindowBorderColour for an explanation of
    //why we're adjusting the spacer here:

    int newheight(0);

    if(m_pagePadding!=0)
    {
        newheight=m_pageStyle.m_borderColour!=wxTransparentColour?m_pagePadding-1:m_pagePadding;
    }

    for(size_t i=0;i<GetPageCount();i++)
    {
        wxAccordionFold* m_fold=getFold(i);
        m_fold->SetWindowSpacer(newheight);
        m_fold->SetWindowMargin(m_pagePadding);
    }
}

/// \brief Returns the radius of the accordion's caption bars corners (in pixels).
///
/// \sa SetCaptionRadius
double   wxAccordion::GetCaptionRadius()           const {return m_captionBarRadius;}

/// \brief Returns the angle (in degrees) that the transition lines will take in the accordion's caption bars.
///
/// \sa SetCaptionGradientAngle
double   wxAccordion::GetCaptionGradientAngle()    const {return m_gradientAngle;}

/// \brief Returns the border style for the accordion's caption bars.
///
/// The return value will be either wxAC_BORDERS_FULL or wxAC_BORDERS_NO_BOTTOM_WHEN_EXPANDED.
/// \sa SetCaptionBorderStyle
wxAccordionBorderStyle wxAccordion::GetCaptionBorderStyle() const{return m_captionBorderStyle;}

/// \brief Returns the minimum height of the accordion's caption bars.
///
/// \sa SetMinCaptionHeight
int      wxAccordion::GetMinCaptionHeight()        const {return m_minCaptionHeight;}

/// \brief Returns the size of the margins (in pixels) used for the icons in the accordion's caption bars.
///
/// \sa SetIconMargin
wxSize   wxAccordion::GetIconMargin()              const {return m_iconMargin;}

/// \brief Returns the size of the margins (in pixels) used for the label on the accordion's caption bars.
///
/// \sa SetTextMargin
wxSize   wxAccordion::GetTextMargin()              const {return m_textMargin;}

/// \brief Returns the size of the margins (in pixels) used for the expand and collapse in the accordion's caption bars.
///
/// \sa SetButtonMargin
wxSize   wxAccordion::GetButtonMargin()            const {return m_buttonMargin;}

/// \brief Returns a copy of the bitmap used for the collapse button of the accordion's caption bars.
///
/// \sa SetCollapseButton
wxBitmap wxAccordion::GetCollapseButton()          const {return m_collapseButton;}

/// \brief Returns a copy of the bitmap used for the expand button of the accordion's caption bars.
///
/// \sa SetExpandButton
wxBitmap wxAccordion::GetExpandButton()            const {return m_expandButton;}

/// \brief Returns the current highlighting style for the accordion.
///
/// The return value will be either wxAC_HIGHLIGHTING_NONE or wxAC_HIGHLIGHTING_MOUSEOVER.
wxAccordionHighlighting wxAccordion::GetHighlighting() const{return m_highlighting;}

/// \brief Returns the the disabled state used for the accordion's caption bars.
///
/// The return value will be either wxAC_DISABLED_STATE_DEFAULT or wxAC_DISABLED_STATE_CUSTOM.
/// \sa SetDisabledState
wxAccordionDisabledState wxAccordion::GetDisabledState() const{return m_disabledState;}


/// \brief Returns the disabled brightness of the accordion's caption bars.
///
/// This setting will not be used if SetDisabledState(wxAC_DISABLED_STATE_CUSTOM) has been called.
/// \sa SetDisabledBrightness
int      wxAccordion::GetDisabledBrightness()      const {return m_disabledBrightness;}

/// \brief Sets the radius of the accordion's caption bars corners (in pixels).
///
/// \sa GetCaptionRadius
void wxAccordion::SetCaptionRadius(double d){m_captionBarRadius=d>=0?d:0;m_changesMade=true;}

/// \brief Sets the angle (in degrees) that the transition lines will take in the accordion's caption bars.
///
/// With an angle of zero, the gradient will run from the top to the bottom.  With an angle of 45, the gradient will run from the top left to the bottom right.  With an angle of 90, the gradient will run from left to right.  And so on.
/// \sa GetCaptionGradientAngle
void wxAccordion::SetCaptionGradientAngle(double a){m_gradientAngle=a;m_changesMade=true;}

/// \brief Sets the border style for the accordion's caption bars.
///
/// The parameter borderStyle can be either wxAC_BORDERS_FULL or wxAC_BORDERS_NO_BOTTOM_WHEN_EXPANDED.
/// \sa SetCaptionBorderStyle
void wxAccordion::SetCaptionBorderStyle(wxAccordionBorderStyle borderStyle){m_captionBorderStyle=borderStyle;m_changesMade=true;}

/// \brief Sets the minimum height of the accordion's caption bars.
///
/// \sa GetMinCaptionHeight
void wxAccordion::SetMinCaptionHeight(int m){m_minCaptionHeight=m>=0?m:0;m_changesMade=true;}

/// \brief Sets the size of the margins (in pixels) used for the icons in the accordion's caption bars.
///
/// \sa GetIconMargin
void wxAccordion::SetIconMargin(const wxSize& p){m_iconMargin=p;m_changesMade=true;}

/// \brief Sets the size of the margins (in pixels) used for the label on the accordion's caption bars.
///
/// \sa GetTextMargin
void wxAccordion::SetTextMargin(const wxSize& p){m_textMargin=p;m_changesMade=true;}

/// \brief Sets the size of the margins (in pixels) used for the expand and collapse in the accordion's caption bars.
///
/// \sa GetButtonMargin
void wxAccordion::SetButtonMargin(const wxSize& p){m_buttonMargin=p;m_changesMade=true;}

/// \brief Sets the bitmap used for the collapse button of the accordion's caption bars.
///
/// \sa GetCollapseButton
void wxAccordion::SetCollapseButton(const wxBitmap& b){m_collapseButton=b;m_changesMade=true;}

/// \brief Sets the bitmap used for the expand button of the accordion's caption bars.
///
/// \sa GetExpandButton
void wxAccordion::SetExpandButton(const wxBitmap& b){m_expandButton=b;m_changesMade=true;}

/// \brief Sets the highlighting style for the accordion.
///
/// The parameter highlighting can be eithe wxAC_HIGHLIGHTING_NONE or wxAC_HIGHLIGHTING_MOUSEOVER.
/// \sa GetHighlighting
void wxAccordion::SetHighlighting(wxAccordionHighlighting highlighting)
{
    if(m_highlighting!=highlighting)
    {
        m_highlighting=highlighting;
        m_changesMade=true;

        for(size_t i=0;i<GetPageCount();i++)
        {
            getFold(i)->SetHighlighting(highlighting==wxAC_HIGHLIGHTING_MOUSEOVER);
        }
    }
}

/// \brief Sets the the disabled state for the accordion's caption bars.
///
/// The parameter state can be either wxAC_DISABLED_STATE_DEFAULT or wxAC_DISABLED_STATE_CUSTOM.
/// \sa GetDisabledState
void wxAccordion::SetDisabledState(wxAccordionDisabledState state){m_disabledState=state;m_changesMade=true;}

/// \brief Sets the disabled brightness of the accordion's caption bars.
///
/// By default, the disabled state of a caption bar is a grayed version of the collapsed caption bar style.  Values close to 0 will produce a very dark disabled caption bar.  Values close to 255 will produce a very light disabled caption bar.
///
/// This setting will not be used if SetDisabledState(wxAC_DISABLED_STATE_CUSTOM) has been called.
/// \sa GetDisabledBrightness
void wxAccordion::SetDisabledBrightness(int b)
{
    if(b<0) b=0;
    else if(b>255) b=255;

    if(m_disabledBrightness!=b)
    {
        m_disabledBrightness=b;
        m_changesMade=true;
    }
}

/// \brief Returns the style used for drawing the caption bar in the collapsed state.
///
/// \sa SetCollapsedStyle
wxAccordionStyle& wxAccordion::GetCollapsedStyle(){return m_collapsedStyle;}

/// \brief Returns the style used for drawing the caption bar in the expanded state.
///
/// \sa SetExpandedStyle
wxAccordionStyle& wxAccordion::GetExpandedStyle(){return m_expandedStyle;}

/// \brief Returns the style used for drawing the caption bar in the collapsed hightlighted state.
///
/// Note: these settings are not used unless SetHighlighting(wxAC_HIGHLIGHTING_MOUSEOVER) has been called.
/// \sa SetCollapsedHLStyle, SetHighlighting
wxAccordionStyle& wxAccordion::GetCollapsedHLStyle(){return m_collapsedHLStyle;}

/// \brief Returns the style used for drawing the caption bar in the expanded highlighted state.
///
/// Note: these settings are not used unless SetHighlighting(wxAC_HIGHLIGHTING_MOUSEOVER) has been called.
/// \sa SetExpandedHLStyle, SetHighlighting
wxAccordionStyle& wxAccordion::GetExpandedHLStyle(){return m_expandedHLStyle;}

/// \brief Returns the style used for drawing the caption bar in the disabled state.
///
/// Note: These settings are not used unless SetDisabledState(wxAC_DISABLED_STATE_CUSTOM) has been called.
/// \sa SetDisabledStyle, SetDisabledState
wxAccordionStyle& wxAccordion::GetDisabledStyle(){return m_disabledStyle;}

/// \brief Returns the style used for drawing the accordion's pages.
///
/// \sa SetPageStyle
wxAccordionStyle& wxAccordion::GetPageStyle(){return m_pageStyle;}

/// \brief Sets the style used for drawing the caption bar in the collapsed state.
///
/// \sa GetCollapsedStyle
void wxAccordion::SetCollapsedStyle(const wxAccordionStyle& style)
{
    m_collapsedStyle=style;
    m_collapsedStyle.m_accordion=this;
    m_collapsedStyle.m_isPageStyle=false;
}

/// \brief Sets the style used for drawing the caption bar in the expanded state.
///
/// \sa GetExpandedStyle
void wxAccordion::SetExpandedStyle(const wxAccordionStyle& style)
{
    m_expandedStyle=style;
    m_expandedStyle.m_accordion=this;
    m_expandedStyle.m_isPageStyle=false;
}

/// \brief Sets the style used for drawing the caption bar in the collapsed hightlighted state.
///
/// Note: these settings are not used unless SetHighlighting(wxAC_HIGHLIGHTING_MOUSEOVER) has been called.
/// \sa GetCollapsedHLStyle, SetHighlighting
void wxAccordion::SetCollapsedHLStyle(const wxAccordionStyle& style)
{
    m_collapsedHLStyle=style;
    m_collapsedHLStyle.m_accordion=this;
    m_collapsedHLStyle.m_isPageStyle=false;
}

/// \brief Sets the style used for drawing the caption bar in the expanded highlighted state.
///
/// Note: these settings are not used unless SetHighlighting(wxAC_HIGHLIGHTING_MOUSEOVER) has been called.
/// \sa GetExpandedHLStyle, SetHighlighting
void wxAccordion::SetExpandedHLStyle(const wxAccordionStyle& style)
{
    m_expandedHLStyle=style;
    m_expandedHLStyle.m_accordion=this;
    m_expandedHLStyle.m_isPageStyle=false;
}

/// \brief Sets the style used for drawing the caption bar in the disabled state.
///
/// Note: These settings are not used unless SetDisabledState(wxAC_DISABLED_STATE_CUSTOM) has been called.
/// \sa GetDisabledStyle, SetDisabledState
void wxAccordion::SetDisabledStyle(const wxAccordionStyle& style)
{
    m_disabledStyle=style;
    m_disabledStyle.m_accordion=this;
    m_disabledStyle.m_isPageStyle=false;
}

/// \brief Sets the style used for drawing the background of the accordion's pages.
///
/// \sa GetPageStyle
void wxAccordion::SetPageStyle(const wxAccordionStyle& style)
{
    m_pageStyle=style;
    m_pageStyle.m_accordion=this;
    m_pageStyle.m_isPageStyle=true;
}
