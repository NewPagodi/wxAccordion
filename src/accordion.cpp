/***************************************************************
 * Name:      accordion.cpp
 * Purpose:   wxAccordion
 * Author:    ()
 * Created:   2015-10-10
 * Copyright: ()
 * License:   wxWindows licence
 **************************************************************/

#include <wx/accordion/accordion.h>
#include <wx/dcbuffer.h>
#include <wx/panel.h>
#include <wx/sizer.h>

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

        wxAccordion* accordion;
        wxRect curTextRect;
        wxString caption;
        int image;

        bool isCollapsed;
        bool isFixedWidth;
        bool isFixedHeight;
        bool isEnabled;
        bool isHighlighted;
        bool useHighlighting;
};

wxIMPLEMENT_ABSTRACT_CLASS(wxAccordionFold, wxPanel)

wxAccordionFold::wxAccordionFold(wxAccordion *parent, const wxString & s,int im,bool  b)
    :wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE),
    accordion(parent),caption(s),image(im),isCollapsed(b),curTextRect(),
    isFixedWidth(false),isFixedHeight(false),isEnabled(true),//isFirstFold(false),isLastFold(false),
    isHighlighted(false),useHighlighting(false)
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
    isCollapsed=false;
    GetSizer()->GetItem(1)->Show(true);
    GetSizer()->GetItem(2)->Show(true);
    GetContainingSizer()->GetItem(this)->SetProportion(isFixedHeight?0:1);
}

void wxAccordionFold::Collapse()
{
    isCollapsed=true;
    GetSizer()->GetItem(1)->Show(false);
    GetSizer()->GetItem(2)->Show(false);
    GetContainingSizer()->GetItem(this)->SetProportion(0);
}

void wxAccordionFold::SetFixedWidth()
{
    isFixedWidth=true;
    GetSizer()->GetItem(2)->SetFlag(GetSizer()->GetItem(2)->GetFlag()&~wxEXPAND);
}

void wxAccordionFold::SetFixedHeight()
{
    GetContainingSizer()->GetItem(this)->SetProportion(0);
    isFixedHeight=true;
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
    useHighlighting=b;
}

void wxAccordionFold::onMouse( wxMouseEvent& event )
{
    //repackage the event and forward it to the accordion
    event.SetX(event.GetX()+GetRect().x);
    event.SetY(event.GetY()+GetRect().y);
    event.SetEventObject(accordion);
    event.SetId(accordion->GetId());
    accordion->ProcessWindowEvent(event);
}

void wxAccordionFold::onMouseLeave( wxMouseEvent& event )
{
    if(useHighlighting)
    {
        bool b = isHighlighted;
        isHighlighted=false;

        if(b)
        {
            Refresh();
        }
    }
}

void wxAccordionFold::onMouseMotion( wxMouseEvent& event )
{
    if(useHighlighting)
    {
        bool b = isHighlighted;

        if(event.GetY() < accordion->captionBarHeight)
        {
            isHighlighted=true;

            if(!b)
            {
                Refresh();
            }
        }
        else
        {
            isHighlighted=false;

            if(b)
            {
                Refresh();
            }
        }
    }

    //repackage the event and forward it to the accordion
    event.SetX(event.GetX()+GetRect().x);
    event.SetY(event.GetY()+GetRect().y);
    event.SetEventObject(accordion);
    event.SetId(accordion->GetId());
    accordion->ProcessWindowEvent(event);
}

void wxAccordionFold::onPaint( wxPaintEvent& event )
{
    if( accordion->changesWereMade() )
    {
        accordion->computeCaptionInfo();
    }

    wxAutoBufferedPaintDC myDC(this);
    myDC.Clear();
    wxPoint textPoint;
    wxColour borderColour;

    if(isCollapsed)
    {
        if(isEnabled)
        {
            if(isHighlighted)
            {
                myDC.DrawBitmap(accordion->captionBarCollapsedHL,0,0 ,true);
                myDC.SetFont(accordion->collapsedHLFont);
                myDC.SetTextForeground(accordion->collapsedHLTextColour);

                borderColour = accordion->collapsedHLBorderColour;
                textPoint = accordion->collapsedHLTextPoint;
            }
            else
            {
                myDC.DrawBitmap(accordion->captionBarCollapsed,0,0 ,true);
                myDC.SetFont(accordion->collapsedFont);
                myDC.SetTextForeground(accordion->collapsedTextColour);

                borderColour = accordion->collapsedBorderColour;
                textPoint = accordion->collapsedTextPoint;
            }
        }
        else
        {
            myDC.DrawBitmap(accordion->captionBarDisabled,0,0 ,true);
            myDC.SetFont(accordion->disabledFont);
            myDC.SetTextForeground(accordion->disabledTextColour);

            if(accordion->customDisabledState)
            {
                borderColour = accordion->disabledBorderColour;
            }
            else
            {
                borderColour = accordion->collapsedBorderColour;
                borderColour.MakeDisabled(accordion->disabledBrightness);
            }

            textPoint = accordion->disabledTextPoint;
        }

        //the collapsed versions don't have a bottom border since
        //if the page margin is zero,
        //we would get 2 borders drawn right next to each other.
        //In all other situations, we want the bottom border, so
        //we need to add it back:

        if( accordion->accordionPadding!=0 && borderColour != wxTransparentColour )
        {
            int width = accordion->captionBarCollapsed.GetWidth();
            int height  = accordion->captionBarCollapsed.GetHeight();
            myDC.SetPen(borderColour);

            myDC.DrawLine(accordion->captionBarRadius+1,height-1,width-(accordion->captionBarRadius)-1,height-1);
        }
    }
    else
    {
        if(isHighlighted)
        {
            myDC.DrawBitmap(accordion->captionBarExpandedHL,0,0 ,true);
            myDC.SetFont(accordion->expandedHLFont);
            myDC.SetTextForeground(accordion->expandedHLTextColour);

            borderColour = accordion->expandedHLBorderColour;
            textPoint = accordion->expandedHLTextPoint;
        }
        else
        {
            myDC.DrawBitmap(accordion->captionBarExpanded,0,0 ,true);
            myDC.SetFont(accordion->expandedFont);
            myDC.SetTextForeground(accordion->expandedTextColour);

            borderColour = accordion->expandedBorderColour;
            textPoint = accordion->expandedTextPoint;
        }
    }

    if(image!=wxAccordion::NO_IMAGE)
    {
        if( wxImageList* il = accordion->GetImageList() )
        {
            il->Draw(image, myDC, accordion->iconRect.x, accordion->iconRect.y, wxIMAGELIST_DRAW_TRANSPARENT);
        }
    }

    wxString text=wxControl::Ellipsize(caption,myDC,wxELLIPSIZE_MIDDLE,accordion->textWidth,wxELLIPSIZE_FLAGS_NONE );
    curTextRect = wxRect( textPoint, myDC.GetTextExtent(text) );
    myDC.DrawText(text, textPoint);

    if(isCollapsed)
    {
        myDC.DrawBitmap(accordion->expandButton,accordion->buttonRect.GetTopLeft() ,true);
    }
    else
    {
        myDC.DrawBitmap(accordion->collapseButton,accordion->buttonRect.GetTopLeft() ,true);

        //Now we need to draw the page's background.
        //Unfortunately, we can't cache a bitmap like we did for the caption bar and so
        //we have to build the background here because there's no way of knowing the
        //height ahead of time.

        wxBitmap temp;
        int width(GetRect().GetWidth()),height(GetRect().GetHeight()-accordion->captionBarHeight);
        accordion->buildBitmap(temp, true, 2,  width, height,  0, 0, 0, height,accordion->pageRadius,accordion->pageBorderColour, accordion->pageBG, accordion->pageStops);
        myDC.DrawBitmap(temp,0,accordion->captionBarHeight,true);
    }
}
#endif

/////////////////////////////////////////////////
//// wxAccordion
//////////////////////////////////////////////////

wxIMPLEMENT_CLASS(wxAccordion, wxBookCtrlBase)
wxDEFINE_EVENT(wxEVT_ACCORDION_COLLAPSED, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_ACCORDION_COLLAPSING, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_ACCORDION_EXPANDED, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_ACCORDION_EXPANDING, wxBookCtrlEvent);

/*! \brief Default constructor.
*
*/
wxAccordion::wxAccordion()
{
    Init();
}

/*! \brief Constructor.
 *
 * \param parent	The parent window.
 * \param id	    An identifier for the panel. wxID_ANY is taken to mean a default.
 * \param pos	The panel position. The value wxDefaultPosition indicates a default position, chosen by either the windowing system or wxWidgets, depending on platform.
 * \param size	The panel size. The value wxDefaultSize indicates a default size, chosen by either the windowing system or wxWidgets, depending on platform.
 * \param style	The window style. See wxPanel.
 * \param name	Window name.
 * \sa Create
 */
wxAccordion::wxAccordion( wxWindow* parent, wxWindowID id , const wxPoint& pos , const wxSize& size , long style, const wxString &name )
{
    Init();
    Create(parent, id, pos, size, style);
}

/*! \brief Destructor.
*
* Deletes any child windows before deleting the physical window.
*/
wxAccordion::~wxAccordion()
{
    //?
}

/*! \brief Used for two-step accordion construction.
*
* See wxAccordion() for details.
*/
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

    trackedImageList=NULL;
    //lastFold=NULL;

    accordionPadding=5;
    onlyToggleWithButton=true;
    pagePadding=5;
    minCaptionHeight=20;
    iconMargin=wxSize(5,5);
    textMargin=wxSize(5,5);
    buttonMargin=wxSize(5,5);
    customDisabledState=false;
    useHighlighting=false;
    disabledBrightness=60;
    collapseButton=wxNullBitmap;
    expandButton=wxNullBitmap;

    accordionBG=wxNullBitmap;
    collapsedBG=wxNullBitmap;
    expandedBG=wxNullBitmap;
    pageBG=wxNullBitmap;
    collapsedHLBG=wxNullBitmap;
    expandedHLBG=wxNullBitmap;
    disabledBG=wxNullBitmap;

    captionBarRadius=0.0;
    gradientAngle=0.0;
    hasBottomBorderWhenExpanded=true;

    expandedStops = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ),wxSystemSettings::GetColour( wxSYS_COLOUR_GRADIENTACTIVECAPTION ));
    expandedBorderColour=wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME );
    expandedTextColour=wxSystemSettings::GetColour( wxSYS_COLOUR_CAPTIONTEXT );
    expandedFont = GetFont();

    collapsedStops = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTION ),wxSystemSettings::GetColour( wxSYS_COLOUR_GRADIENTINACTIVECAPTION  ));
    collapsedBorderColour=wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME );
    collapsedTextColour=wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTIONTEXT );
    collapsedFont = GetFont();

    expandedHLStops = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ),wxSystemSettings::GetColour( wxSYS_COLOUR_GRADIENTACTIVECAPTION  ));
    expandedHLBorderColour=wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME );
    expandedHLTextColour=wxSystemSettings::GetColour( wxSYS_COLOUR_CAPTIONTEXT );
    expandedHLFont = GetFont();

    collapsedHLStops = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTION ),wxSystemSettings::GetColour( wxSYS_COLOUR_GRADIENTINACTIVECAPTION  ));
    collapsedHLBorderColour=wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME );
    collapsedHLTextColour=wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTIONTEXT );
    collapsedHLFont = GetFont();

    disabledStops = wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_FRAMEBK  ),wxSystemSettings::GetColour( wxSYS_COLOUR_FRAMEBK   ));
    disabledBorderColour=wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME );
    disabledTextColour=wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT );
    disabledFont = GetFont();

    pageRadius=0.0;
    pageStops=wxGraphicsGradientStops(wxSystemSettings::GetColour( wxSYS_COLOUR_FRAMEBK ),wxSystemSettings::GetColour( wxSYS_COLOUR_FRAMEBK ));
    pageBorderColour=wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER );

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

    b_sizer->Add( 0, accordionPadding, 0, 0, 0 );

    if(style&wxACCORDION_COLLAPSE_TO_BOTTOM)
    {
        b_sizer->Add( 0, 0, 1, 0, 0 );
    }
    else
    {
        b_sizer->Add( 0, 0, 0, 0, 0 );
    }

    lastBorder = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize(-1,1), wxFULL_REPAINT_ON_RESIZE );
    lastBorder->SetBackgroundStyle(wxBG_STYLE_PAINT);
    wxColour c = collapsedBorderColour;
    lastBorder->SetForegroundColour(c.MakeDisabled(disabledBrightness));

    lastBorder->Hide();
    b_sizer->Add( lastBorder,0, wxEXPAND, 0 );

    SetSizer( b_sizer );

    lastBorder->Bind( wxEVT_PAINT,   &wxAccordion::onPaintBorder  ,this );
    Bind( wxEVT_SIZE,        &wxAccordion::onSize,       this );
    Bind( wxEVT_LEFT_UP,     &wxAccordion::onLeftUp,     this );
    Bind( wxEVT_LEFT_DCLICK, &wxAccordion::onLeftDClick, this );
}

void wxAccordion::onPaintBorder( wxPaintEvent& event )
{
    wxAutoBufferedPaintDC myDC(lastBorder);
    myDC.Clear();

    int width = captionBarCollapsed.GetWidth();
    myDC.SetPen( lastBorder->GetForegroundColour() );
    myDC.DrawLine(captionBarRadius,0,width-captionBarRadius,0);
}

void wxAccordion::onPaint( wxPaintEvent& event )
{
    wxAutoBufferedPaintDC myDC(this);

    myDC.Clear();

    int width = GetSize().GetWidth();
    int height = GetSize().GetHeight();
    int bmpwidth = accordionBG.GetWidth();
    int bmpheight = accordionBG.GetHeight();

    for(int i=0;i*bmpheight<height;i++)
    {
        for(int j=0;j*bmpwidth<width;j++)
        {
            myDC.DrawBitmap(accordionBG,j*bmpwidth,i*bmpheight,true);
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
    else if( IsCollapsed(fold) && expandButton.IsOk() && !( !onlyToggleWithButton ||  (flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON)) )
    {
        SetSelection(fold);
    }
    else if(flags&wxACCORDION_HITTEST_ONCAPTION && collapseButton.IsOk() && !( !onlyToggleWithButton ||  (flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON)) )
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
        if(expandButton.IsOk())
        {
            if(!onlyToggleWithButton ||  (flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON) )
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
        if(collapseButton.IsOk())
        {
            if( !onlyToggleWithButton || flags&wxACCORDION_HITTEST_ONCAPTIONBUTTON )
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
        changesMade=true;
    }

    adjustForStyle( style );

    //if wxACCORDION_FLOAT_TO_TOP or wxACCORDION_COLLAPSE_TO_BOTTOM
    //are being removed, we need to take some extra steps:
    if( (GetWindowStyleFlag()&wxACCORDION_FLOAT_TO_TOP) && !(style & wxACCORDION_FLOAT_TO_TOP) )
    {
        //We need to sink the floated items, so first make a vector
        //copy of the folds:
        wxSizer* szr = GetSizer();
        std::vector<wxAccordionFold*> m_folds;

        for(size_t i=0;i<GetPageCount();i++)
        {
            m_folds.push_back(getFold(i));
        }

        //now remove all the folds from the sizer:
        for(std::vector<wxAccordionFold*>::iterator it = m_folds.begin();it!=m_folds.end();it++)
        {
            szr->Detach(*it);
        }
        szr->Detach(lastBorder);


        //add back all folds:
        int proportion;
        wxAccordionFold* m_fold;
        for(std::vector<wxAccordionFold*>::iterator it = m_folds.begin();it!=m_folds.end();it++)
        {
            m_fold = *it;

            if( m_fold->isCollapsed || m_fold->isFixedHeight )
            {
                proportion=0;
            }
            else
            {
                proportion=1;
            }

            szr->Add( m_fold, proportion, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, accordionPadding );
            //lastFold=m_fold;
        }
        szr->Add( lastBorder, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 0 );

    }

    if( !(style & wxACCORDION_COLLAPSE_TO_BOTTOM) )
    {
        setSecondSpacerProp(0);
    }

    wxBookCtrlBase::SetWindowStyleFlag(style);
}

bool wxAccordion::changesWereMade()
{
    //this is an ugly hackaround intended to help with the fact that
    //AssignImageList isn't virtual, so I can't just set changesMade=true
    //there.  Without this extra check, refresh and layout won't detect
    //changes made to the imagelist, and so won't call computeCaptionInfo()
    //when the imagelist was changed.

    if( trackedImageList != GetImageList() )
    {
        trackedImageList=GetImageList();
        return true;
    }
    else if(changesMade)
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
    changesMade=true;
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
        wxAccordionFold* m_fold = getFold(i);
        wxRect r = m_fold->GetRect();

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
        if(pt.y-r.y<captionBarHeight)
        {
            *flags = *flags | wxACCORDION_HITTEST_ONCAPTION;

            if(IsExpanded(i))
            {
                if(expandedBorderColour!=wxTransparentColour)
                {
                    if(pt.y==r.y || pt.x ==r.x || pt.y==r.y +captionBarHeight-1 || pt.x ==r.GetRight()  )
                    {
                        *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONBORDER;
                    }
                }
            }
            else
            {
                if(collapsedBorderColour!=wxTransparentColour)
                {
                    if(pt.y==r.y || pt.x ==r.x || pt.y==r.y +captionBarHeight-1 || pt.x ==r.GetRight()  )
                    {
                        *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONBORDER;
                    }
                }
            }

            if(  buttonRect.Contains(wxPoint(pt.x-r.x,pt.y-r.y)))
            {
                *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONBUTTON;
            }

            if(  iconRect.Contains(wxPoint(pt.x-r.x,pt.y-r.y)))
            {
                *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONICON;
            }

            if(  m_fold->curTextRect.Contains(wxPoint(pt.x-r.x,pt.y-r.y)))
            {
                *flags = *flags | wxACCORDION_HITTEST_ONCAPTIONLABEL;
            }
        }
        else //if it's not on the caption bar, it has to be on the window
        {
            *flags = *flags | wxACCORDION_HITTEST_ONPAGE;

            if(pagePadding>0)
            {
                if( pt.x==r.x || pt.y==r.GetBottom() ||pt.x==r.GetRight()   )
                {

                    if(pageBorderColour!=wxTransparentColour)
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
                        pt.x-r.x<pagePadding ||
                        pt.y-(r.y+captionBarHeight)< (pageBorderColour==wxTransparentColour?pagePadding:pagePadding-1) ||
                        r.GetBottom()-pt.y<pagePadding ||
                        r.GetRight()-pt.x<pagePadding
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

    changesMade=false;
    int oldHeight = captionBarHeight;

    //First find the needed captionBarHeight and the
    //rectangles for the button, icon, and text.  Because there are
    //so many possibilities, this is a slightly complicated process.

    wxMemoryDC memDC;
    memDC.SetFont(expandedFont);
    int expandedTextHeight = memDC.GetFontMetrics().height;
    memDC.SetFont(collapsedFont);
    int collapsedTextHeight = memDC.GetFontMetrics().height;
    memDC.SetFont(collapsedHLFont);
    int collapsedHLTextHeight = memDC.GetFontMetrics().height;
    memDC.SetFont(expandedHLFont);
    int expandedHLTextHeight = memDC.GetFontMetrics().height;
    memDC.SetFont(disabledFont);
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

    int totalTextHeight = textHeight + 2*textMargin.GetHeight();

    int iconWidth(0),iconHeight(0),totalIconHeight(0);
    if(wxImageList* il=	GetImageList())
    {
        il->GetSize(0, iconWidth, iconHeight);
        totalIconHeight = iconHeight + 2*iconMargin.GetHeight();
    }

    int collapseButtonHeight = collapseButton.GetHeight();
    int expandButtonHeight = expandButton.GetHeight();
    int buttonHeight = collapseButtonHeight>expandButtonHeight?collapseButtonHeight:expandButtonHeight;

    int collapseButtonWidth = collapseButton.GetWidth();
    int expandButtonWidth = expandButton.GetWidth();
    int buttonWidth  = collapseButtonWidth>expandButtonWidth?collapseButtonWidth:expandButtonWidth;

    int totalButtonHeight = buttonHeight+ 2*buttonMargin.GetHeight();

    captionBarHeight=minCaptionHeight;

    if(totalTextHeight>captionBarHeight)
    {
        captionBarHeight=totalTextHeight;
    }
    if(totalIconHeight>captionBarHeight)
    {
        captionBarHeight=totalIconHeight;
    }
    if(totalButtonHeight>captionBarHeight)
    {
        captionBarHeight=totalButtonHeight;
    }

    wxRect r=GetRect();

    int butnLeftPos(0),iconLeftPos(0),textLeftPos(0);
    textWidth = r.GetWidth()
                  - 2*textMargin.GetWidth()
                  - 2*accordionPadding
                  - buttonWidth
                  - iconWidth
                  - (buttonWidth!=0?buttonMargin.GetWidth():0)
                  - (iconWidth!=0?iconMargin.GetWidth():0)
                  ;

    if(GetWindowStyle() & wxACCORDION_BUTTON_LEFT)
    {
        butnLeftPos = buttonMargin.GetWidth();
        iconLeftPos = buttonWidth+(buttonWidth!=0?buttonMargin.GetWidth():0)
                        + iconMargin.GetWidth();
        textLeftPos = buttonWidth+(buttonWidth!=0?buttonMargin.GetWidth():0)
                        + iconWidth  +(iconWidth!=0?iconMargin.GetWidth():0)
                        + textMargin.GetWidth();
    }
    else
    {
        butnLeftPos = r.GetWidth()-2*accordionPadding-buttonWidth-buttonMargin.GetWidth();
        iconLeftPos = iconMargin.GetWidth();
        textLeftPos = iconWidth+(iconWidth!=0?iconMargin.GetWidth():0) + textMargin.GetWidth();
    }

    buttonRect           = wxRect(butnLeftPos, (captionBarHeight-buttonHeight)/2,        buttonWidth, buttonHeight);
    iconRect             = wxRect(iconLeftPos, (captionBarHeight-iconHeight)/2,          iconWidth,   iconHeight);
    collapsedTextPoint   = wxPoint(textLeftPos, (captionBarHeight-collapsedTextHeight)/2);
    expandedTextPoint    = wxPoint(textLeftPos, (captionBarHeight-expandedTextHeight)/2);
    collapsedHLTextPoint = wxPoint(textLeftPos, (captionBarHeight-collapsedHLTextHeight)/2);
    expandedHLTextPoint  = wxPoint(textLeftPos, (captionBarHeight-expandedHLTextHeight)/2);
    disabledTextPoint    = wxPoint(textLeftPos, (captionBarHeight-disabledTextHeight)/2);

    //Part2: if the height has changed, we need to adjust the height allocated
    //for any pages already in the accordion.
    if(captionBarHeight!=oldHeight)
    {
        for(size_t i=0;i<GetPageCount();i++)
        {
            getFold(i)->SetCaptionHeight(captionBarHeight);
        }
    }

    //Part3: build bitmaps for the caption bar backgrounnds
    int width(GetRect().GetWidth()-2*accordionPadding);
    int height = captionBarHeight;

    double alpha=gradientAngle;

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
    else if( alpha == 180)
    {
        x1=0;
        y1=height;
        x2=0;
        y2=0;
    }

    //We're finally ready to build the bitmaps
    //bitmap for a collapsed caption bar:

    buildBitmap(   captionBarCollapsed, false, 0, width, height, x1, y1, x2, y2,captionBarRadius,collapsedBorderColour,   collapsedBG,   collapsedStops);
    buildBitmap(    captionBarExpanded,  hasBottomBorderWhenExpanded, 1, width, height, x1, y1, x2, y2,captionBarRadius,expandedBorderColour,    expandedBG,    expandedStops);

    if(useHighlighting)
    {
        buildBitmap( captionBarCollapsedHL, false, 0, width, height, x1, y1, x2, y2,captionBarRadius,collapsedHLBorderColour, collapsedHLBG, collapsedHLStops);
        buildBitmap(  captionBarExpandedHL,  hasBottomBorderWhenExpanded, 1, width, height, x1, y1, x2, y2,captionBarRadius,expandedHLBorderColour,  expandedHLBG,  expandedHLStops);
    }
    else
    {
        captionBarCollapsedHL=captionBarCollapsed;
        captionBarExpandedHL=captionBarExpanded;
    }

    if(customDisabledState)
    {
        buildBitmap(    captionBarDisabled, false, 0, width, height, x1, y1, x2, y2,captionBarRadius,disabledBorderColour,    disabledBG,    disabledStops);
    }
    else
    {
        //The obvious thing to do here is:
        //captionBarDisabled=captionBarDisabled.ConvertToDisabled(disabledBrightness);
        //but that seems to mess up the mask used when the corners are rounded.  So,
        //I'll just convert all the colors to disabled and build a new bitmap with
        //those colors.

        wxColour c1 = collapsedStops.GetStartColour().MakeDisabled(disabledBrightness);
        wxColour c2 = collapsedStops.GetEndColour().MakeDisabled(disabledBrightness);
        wxGraphicsGradientStops gs(c1, c2);

        wxColor bc;
        for(size_t i=1;i<collapsedStops.GetCount()-1;i++)
        {
            bc=collapsedStops.Item(i).GetColour();
            bc.MakeDisabled(disabledBrightness);
            gs.Add( bc, collapsedStops.Item(i).GetPosition() );
        }

        bc = collapsedBorderColour;
        bc.MakeDisabled(disabledBrightness);
        wxBitmap b = collapsedBG.ConvertToDisabled(disabledBrightness);

        buildBitmap(    captionBarDisabled, false, 0, width, height, x1, y1, x2, y2,captionBarRadius,bc,    b,    gs);
    }
}

void wxAccordion::buildBitmap(wxBitmap& out, bool bottomBorder, int drawtype, int width,int height, double x1,double y1,double x2,double y2, double radius, const wxColour& borderColour,const wxBitmap& bgBitmap,const wxGraphicsGradientStops& stops)
{
    //todo: in certain situations, we can, take some steps to
    //speed things up:
    //  -if stops only has 2 entries and they're both transparent, do nothing
    //  -if stops only has 2 entries, but their the same, just fill the window
    //  -if stops only has 2 different entries - use wxDC::GradientFillLinear instead
    //  -in all cases, if radius==0, we can just use
    //     a regular rectangle instead of a rounded rectangle.
    //  -only if all of those fail should we go through the whole process.

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
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        m_fold->isEnabled=true;
        m_fold->Refresh();

        lastBorder->SetForegroundColour(collapsedBorderColour);
        lastBorder->Refresh();
    }
}


/// \brief Sets the accordion page with index n to have the disabled state.
///
/// The caption bar will be drawn as a grayed version of the collapsed state
/// by default or using the disabled style properties if SetCustomDisabledState()
/// been called with true.  If the page was expanded, it will be collapsed.
/// \sa Enable, IsDisabled, SetDisabledBrightness
void wxAccordion::Disable(size_t n)
{
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        m_fold->isEnabled=false;
        m_fold->Refresh();

        if(customDisabledState)
        {
            lastBorder->SetForegroundColour(disabledBorderColour);
        }
        else
        {
            wxColour c = collapsedBorderColour;
            lastBorder->SetForegroundColour( c.MakeDisabled(disabledBrightness) );
        }
        lastBorder->Refresh();
    }
    Collapse(n);

}

/// \brief Checks if the accordion page with index n is in the enabled state.
///
/// Note that this will return false if n is not a valid page index.
/// \sa IsDisabled
bool wxAccordion::IsEnabled(size_t n) const
{
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        return m_fold->isEnabled;
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
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        return !m_fold->isEnabled;
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
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        return !(m_fold->isCollapsed);
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
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        return m_fold->isCollapsed;
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
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        if( m_fold->isCollapsed )
        {
            toggle(m_fold,n,false);
        }
    }
}

/// \brief Collapses the accordion page with index n to hide its contents.
///
/// If the page is already collapsed or n is not a valid page index, this has no effect.
/// \sa IsCollapsed, Expand, Toggle
void wxAccordion::Collapse(size_t n)
{
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        if( !m_fold->isCollapsed )
        {
            toggle( m_fold, n,false);
        }
    }
}

/// \brief Toggles the accordion page with index n between the expanded and collapsed states.
///
/// If n is not a valid page index, this has no effect.
/// \sa Collapse, Expand
void wxAccordion::Toggle(size_t n)
{
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        toggle(m_fold,n,false);
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
    if(wxAccordionFold* m_fold = getFold(n))
    {
        m_fold->SetFixedWidth();
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
    if(wxAccordionFold* m_fold = getFold(n))
    {
        m_fold->SetFixedHeight();
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
    if(wxAccordionFold* m_fold = getFold(n))
    {
        m_fold->SetFixedHeight();
        m_fold->SetFixedWidth();
    }
}

/// \brief Returns the current height of the caption bars in the accordion.
///
/// The caption bar is set to have the smallest height needed for the text,
/// the icons (if used), and the buttons (if used) plus any padding.  If the
/// icons, buttons, or font are changed, then the caption bar height may change
/// as well.  This function will return the current value.
int wxAccordion::GetCaptionBarHeight() const
{
    return captionBarHeight;
}

void wxAccordion::toggle(wxAccordionFold* m_fold,size_t selection,bool useEvents)
{
    if(useEvents)
    {
        wxBookCtrlEvent event2(m_fold->isCollapsed?wxEVT_ACCORDION_EXPANDING:wxEVT_ACCORDION_COLLAPSING, GetId(), selection, m_selection);
        event2.SetEventObject(this);
        ProcessWindowEvent(event2);

        if(!event2.IsAllowed())
        {
            return;
        }
    }

    Freeze();

    if(m_fold->isCollapsed)
    {
        m_fold->Expand();
    }
    else
    {
        m_fold->Collapse();
    }

    int oldSelection = m_selection;
    m_selection=selection;

    adjustForStyle( GetWindowStyleFlag() );

    if( accordionPadding==0 )
    {
        if( m_fold == getFold(m_pages.size()-1) )
        {
            if(m_fold->isCollapsed && lastBorder->GetForegroundColour()!=wxTransparentColor)
            {
                lastBorder->Show();
            }
            else
            {
                lastBorder->Hide();
            }
        }
    }

    Layout();
    Thaw();

    if(useEvents)
    {
        wxBookCtrlEvent event3(m_fold->isCollapsed?wxEVT_ACCORDION_COLLAPSED:wxEVT_ACCORDION_EXPANDED, GetId(), selection, oldSelection);
        event3.SetEventObject(this);
        ProcessWindowEvent(event3);
    }
}

void wxAccordion::adjustForStyle( long style )
{
    if( style & wxACCORDION_SINGLE_FOLD )
    {
        for(size_t i=0;i<GetPageCount();i++)
        {
            if(i!=m_selection)
            {
                getFold(i)->Collapse();
            }
        }
    }

    if( style & wxACCORDION_FLOAT_TO_TOP )
    {
        rebuildFloats();
    }

    if( style & wxACCORDION_COLLAPSE_TO_BOTTOM )
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

    if( GetWindowStyleFlag() & wxACCORDION_FLOAT_TO_TOP )
    {
        //If wxACCORDION_FLOAT_TO_TOP is active, the pages may not be in order
        //in the sizer since expanded pages are moved to the top.  We'll just have
        //to look at all the sizer items and find one whose window is m_pages[n]:

        wxSizer* szr = GetSizer();
        wxAccordionFold* found = NULL;

        for( size_t i = 2; i<szr->GetItemCount(); i++)
        {
            if( wxAccordionFold* m_fold = wxDynamicCast(szr->GetItem(i)->GetWindow(),wxAccordionFold))
            {
                if( m_fold->GetPage()==m_pages[n] )
                {
                    found = m_fold;
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

        if( wxAccordionFold* m_fold = wxDynamicCast(szr->GetItem(n+2)->GetWindow(),wxAccordionFold) )
        {
            return m_fold;
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
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        m_fold->caption = strText;
        m_fold->Refresh();
        return true;
    }
    else
    {
        return false;
    }
}

wxString wxAccordion::GetPageText(size_t n) const
{
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        return m_fold->caption;
    }
    else
    {
        return wxEmptyString;
    }
}

int wxAccordion::GetPageImage(size_t n) const
{
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        return m_fold->image;
    }
    else
    {
        return NO_IMAGE;
    }
}

bool wxAccordion::SetPageImage(size_t n, int imageId)
{
    if( wxAccordionFold* m_fold = getFold(n) )
    {
        m_fold->image=imageId;
        m_fold->Refresh();
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

    if( wxAccordionFold* m_fold = getFold(n) )
    {
        toggle(m_fold,n,true);
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

    if(wxAccordionFold* m_fold = getFold(page) )
    {
        Freeze();
        GetSizer()->Detach(m_fold);
        wxWindow* win = m_fold->GetPage();
        //somewhere we should check that m_pages[page]==m_fold->GetPage()
        //since if not, something has gone very wrong.
        //But then again, I'm not sure what could be done if they don't match.
        win->Reparent(this);
        m_fold->Destroy();
        if(page==m_selection)
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
    std::vector<wxAccordionFold*> m_folds;

    for(size_t i=0;i<GetPageCount();i++)
    {
        m_folds.push_back(getFold(i));
    }

    //now remove all the folds from the sizer:
    for(std::vector<wxAccordionFold*>::iterator it = m_folds.begin();it!=m_folds.end();it++)
    {
        szr->Detach(*it);
    }
    szr->Detach(lastBorder);

    //add back all expanded folds:
    long flags( wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT );
    int proportion(0);
    wxAccordionFold* m_fold;

    for(std::vector<wxAccordionFold*>::iterator it = m_folds.begin();it!=m_folds.end();it++)
    {
        m_fold = *it;

        if(!m_fold->isCollapsed)
        {
            proportion = m_fold->isFixedHeight?0:1;

            szr->Add( m_fold, proportion, flags, accordionPadding );
        }
    }

    //add add all the collapsed items back:
    for(std::vector<wxAccordionFold*>::iterator it = m_folds.begin();it!=m_folds.end();it++)
    {
        m_fold = *it;

        if(m_fold->isCollapsed)
        {
            szr->Add( m_fold, 0, flags, accordionPadding );
        }
    }

    //add the last border back
    szr->Add( lastBorder, 0, flags, 0 );
}

bool wxAccordion::InsertPage(size_t n, wxWindow *page, const wxString& text, bool bSelect, int imageId )
{
    if(wxSizer* szr = GetSizer())
    {
        Freeze();

        wxAccordionFold* m_fold = new wxAccordionFold( this , text,imageId,!bSelect);
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

        page->Reparent(m_fold);

        m_fold->SetBackgroundColour( GetBackgroundColour() );
        m_fold->useHighlighting=useHighlighting;

        bSizer3->Add( 0, captionBarHeight);
        //Since there is no drawn top border for the window, if
        //we just add page with flags wxALL, there will be an
        //extra pixel on the top.  So I'll add a spacer of size
        //pagePadding-1 and use flags wxBOTTOM|wxLEFT|wxRIGHT

        if(pagePadding>0)
        {
            bSizer3->Add( 0, (pageBorderColour==wxTransparentColour?pagePadding:pagePadding-1));
        }
        else
        {
            bSizer3->Add( 0, 0);
        }

        bSizer3->Add( page, 1, wxEXPAND|wxBOTTOM|wxLEFT|wxRIGHT, pagePadding );

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
        m_fold->SetSizer( bSizer3 );

        if( GetWindowStyleFlag() & wxACCORDION_FLOAT_TO_TOP )
        {
            //rather than trying to figure out where to put it, just
            //dump it on the end and then call the rebuildFloats function
            //to place everything correctly.
            szr->Add( m_fold, proportion, flags, accordionPadding );
            rebuildFloats();
        }
        else
        {
            szr->Detach(lastBorder);

            if( n >= m_pages.GetCount()-1 )
            {
                szr->Add( m_fold, proportion, flags, accordionPadding );
            }
            else
            {
                //item 0 is a spacer used with the collapse to bottom style
                //item 1 is a spacer that servers as the top page margin
                //for the first item.  So to add the n-th fold, we have to
                //add it at sizer position n+2
                szr->Insert( n+2, m_fold, proportion, flags, accordionPadding );
            }

            szr->Add( lastBorder, 0, flags, 0 );
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
/// the pages.  It's also the gap between 2 pages.
/// \sa SetAccordionPadding
int      wxAccordion::GetAccordionPadding()        const {return accordionPadding;}


/// \brief Returns a copy of the bitmap being used for the accordion background.
///
/// \sa SetAccordionBGBitmap
wxBitmap wxAccordion::GetAccordionBGBitmap()       const {return accordionBG;}


/// \brief Returns whether or not the accordion will toggle only in response to mouse clicks over the caption bar buttons.
///
/// \sa SetOnlyToggleWithButton
bool     wxAccordion::GetOnlyToggleWithButton()    const {return onlyToggleWithButton;}


/// \brief Sets the accordion padding size (in pixels).
///
/// The accordion padding is the gap between the edge of the accordion and
/// the pages.  It's also the gap between 2 pages.
/// \sa GetAccordionPadding
void wxAccordion::SetAccordionPadding(int m)
{
    int old = accordionPadding;
    accordionPadding=m>=0?m:0;

    if(accordionPadding!=old)
    {
        if( accordionPadding==0 && m_pages.size()>0 && collapsedBorderColour!=wxTransparentColor )
        {
            lastBorder->Show();
        }
        else
        {
            lastBorder->Hide();
        }

        changesMade = true;

        setFirstSpacerHeight(m);

        for(size_t i=0;i<GetPageCount();i++)
        {
            getFold(i)->SetBorder(m);
        }
    }
}


/// \brief Sets a bitmap to be used for the accordion background.
///
/// To clear the bitmap, call this function with wxNullBitmap as the parameter.
/// \sa GetAccordionBGBitmap
void wxAccordion::SetAccordionBGBitmap(const wxBitmap& b)
{
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

    accordionBG = b;
}


/// \brief Sets whether or not the accordion will toggle only in response to mouse clicks over the caption bar buttons.
///
/// \sa GetOnlyToggleWithButton
void  wxAccordion::SetOnlyToggleWithButton(bool b){onlyToggleWithButton=b;}




/////////////////////////////////////////////////
////  Page style get/set functions
//////////////////////////////////////////////////

/// \brief Returns the radius of the accordion's pages (in pixels).
///
/// \sa SetPageRadius
double   wxAccordion::GetPageRadius()              const {return pageRadius;}

/// \brief Returns the padding (in pixels) of the accordion's pages.
///
/// \sa SetPagePadding
int      wxAccordion::GetPagePadding()             const {return pagePadding;}

/// \brief Returns a copy of the bitmap used for drawing the background of the accordion's pages.
///
/// \sa SetPageBGBitmap
wxBitmap wxAccordion::GetPageBGBitmap()            const {return pageBG;}

/// \brief Returns a copy of the gradient stops used for drawing the background of the accordion's pages.
///
/// \sa AddPageGradientStop, ClearPageGradientStops
wxGraphicsGradientStops wxAccordion::GetPageGradientStops() const {return pageStops;}

/// \brief Returns the first (top) colour used for drawing the background of the accordion's pages.
///
/// \sa SetPageColour1
wxColour wxAccordion::GetPageColour1()             const {return pageStops.GetStartColour();}

/// \brief Returns the second (bottom) colour used for drawing the background of the accordion's pages.
///
/// \sa SetPageColour2
wxColour wxAccordion::GetPageColour2()             const {return pageStops.GetEndColour();}

/// \brief Returns the border colour used for drawing the background of the accordion's pages.
///
/// \sa SetPageBorderColour
wxColour wxAccordion::GetPageBorderColour()        const {return pageBorderColour;}

/// \brief Sets the radius of the accordion's pages (in pixels).
///
/// This setting only determines the radius used for the bottom corners.  The radius of the top corners are set with SetCaptionRadius().
/// \sa SetCaptionRadius, GetPageRadius
void     wxAccordion::SetPageRadius(double d){pageRadius=d>=0?d:0;changesMade=true;}

/// \brief Sets the padding (in pixels) of the accordion's pages.
///
/// The page padding is the space between the edge of the page and the edge of the window it contains.
/// \sa GetPagePadding
void wxAccordion::SetPagePadding(int m)
{
    int old = pagePadding;
    pagePadding=m>=0?m:0;

    if(pagePadding==old)
    {
        return;
    }

    changesMade = true;

    //See the notes for SetWindowBorderColour for an explanation of
    //why we're adjusting the spacer here:

    int newheight(0);

    if(pagePadding!=0)
    {
        newheight=pageBorderColour!=wxTransparentColour?pagePadding-1:pagePadding;
    }

    for(size_t i=0;i<GetPageCount();i++)
    {
        wxAccordionFold* m_fold=getFold(i);
        m_fold->SetWindowSpacer(newheight);
        m_fold->SetWindowMargin(pagePadding);
    }
}

/// \brief Sets the bitmap used for drawing the background of the accordion's pages.
///
/// To clear the bitmap, call this function with wxNullBitmap as the parameter.
/// \sa GetPageBGBitmap
void     wxAccordion::SetPageBGBitmap(const wxBitmap& b){pageBG=b;changesMade=true;}

/// \brief Adds a gradient stop to the set of colours used for drawing the background of the accordion's pages.
///
/// \sa GetPageGradientStops, ClearPageGradientStops
void     wxAccordion::AddPageGradientStop(const wxColour& col,float pos){pageStops.Add( col, pos);changesMade=true;}

/// \brief Clears any gradient stops that have been set for drawing the background of the accordion's pages.
///
/// \sa GetPageGradientStops, AddPageGradientStop
void     wxAccordion::ClearPageGradientStops(){pageStops=wxGraphicsGradientStops(pageStops.GetStartColour(), pageStops.GetEndColour());changesMade=true;}

/// \brief Sets the first (top) colour used for drawing the background of the accordion's pages.
///
/// \sa GetPageColour1
void     wxAccordion::SetPageColour1(const wxColour& col){pageStops.SetStartColour(col);changesMade=true;}

/// \brief Sets the second (bottom) colour used for drawing the background of the accordion's pages.
///
/// \sa GetPageColour2
void     wxAccordion::SetPageColour2(const wxColour& col){pageStops.SetEndColour(col);changesMade=true;}

/// \brief Sets the border colour used for drawing the background of the accordion's pages.
///
/// \sa GetPageBorderColour
void wxAccordion::SetPageBorderColour(const wxColour& col)
{
    //Since the top border for the window isn't drawn, there is a spacer 1 pixel
    //less than the margin when the border colour is not transparent and
    //equal to the margin when the border colour is transparent.
    //So if we're changing to or from a transparent colour, all of those spacers need
    //to be adjusted.

    if(col==pageBorderColour)
    {
        return;
    }

    wxColour old = pageBorderColour;
    bool adjust(false);
    int newheight(0);

    pageBorderColour=col;
    changesMade=true;

    if(pagePadding!=0)
    {
        if(old == wxTransparentColour && col != wxTransparentColour)
        {
            adjust=true;
            newheight=pagePadding-1;
        }
        else if(old != wxTransparentColour && col == wxTransparentColour)
        {
            adjust=true;
            newheight=pagePadding;
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

/////////////////////////////////////////////////
////  Caption bar style get/set functions
//////////////////////////////////////////////////

/// \brief Returns the radius of the accordion's caption bars corners (in pixels).
///
/// \sa SetCaptionRadius
double   wxAccordion::GetCaptionRadius()           const {return captionBarRadius;}

/// \brief Returns the angle (in degrees) that the transition lines will take in the accordion's caption bars.
///
/// \sa SetCaptionGradientAngle
double   wxAccordion::GetCaptionGradientAngle()    const {return gradientAngle;}

/// \brief Returns whether or not the bottom border of the caption bar will be drawn when a page is expanded.
///
/// \sa SetExpandedBottomBorder
bool     wxAccordion::GetExpandedBottomBorder()    const {return hasBottomBorderWhenExpanded;}

/// \brief Returns the minimum height of the accordion's caption bars.
///
/// \sa SetMinCaptionHeight
int      wxAccordion::GetMinCaptionHeight()        const {return minCaptionHeight;}

/// \brief Returns the size of the margins (in pixels) that will be used for the icons when drawing the accordion's caption bars.
///
/// \sa SetIconMargin
wxSize   wxAccordion::GetIconMargin()              const {return iconMargin;}

/// \brief Returns the size of the margins (in pixels) that will be used for the label when drawing the accordion's caption bars.
///
/// \sa SetTextMargin
wxSize   wxAccordion::GetTextMargin()              const {return textMargin;}

/// \brief Returns the size of the margins (in pixels) that will be used for the expand and collapse buttons when drawing the accordion's caption bars.
///
/// \sa SetButtonMargin
wxSize   wxAccordion::GetButtonMargin()            const {return buttonMargin;}

/// \brief Returns a copy of the bitmap used for the expand button of the accordion's caption bars.
///
/// \sa SetCollapseButton
wxBitmap wxAccordion::GetCollapseButton()          const {return collapseButton;}

/// \brief Returns a copy of the bitmap used for the collapse button of the accordion's caption bars.
///
/// \sa SetExpandButton
wxBitmap wxAccordion::GetExpandButton()            const {return expandButton;}

/// \brief Returns whether or not the accordion's caption bars will change to the highlighted style when the mouse cursor is over them.
///
/// \sa SetUseHighlighting
bool     wxAccordion::GetUseHighlighting()         const {return useHighlighting;}

/// \brief Returns whether or not the accordion's caption bars will use a custom disables state.
///
/// \sa SetCustomDisabledState
bool     wxAccordion::GetCustomDisabledState()     const {return customDisabledState;}

/// \brief Returns the disabled brightness of the accordion's caption bars.
///
/// \sa SetDisabledBrightness
int      wxAccordion::GetDisabledBrightness()      const {return disabledBrightness;}

/// \brief Sets the radius of the accordion's caption bars corners (in pixels).
///
/// \sa GetCaptionRadius
void wxAccordion::SetCaptionRadius(double d){captionBarRadius=d>=0?d:0;changesMade=true;}

/// \brief Sets the angle (in degrees) that the transition lines will take in the accordion's caption bars.
///
/// With an angle of zero, the gradient will run from the top to the bottom.  With an angle of 45, the gradient will run from the top left to the bottom right.  With an angle of 90, the gradient will run from left to right.  And so on.
/// \sa GetCaptionGradientAngle
void wxAccordion::SetCaptionGradientAngle(double a){gradientAngle=a;changesMade=true;}

/// \brief Sets whether or not the bottom border of the caption bar will be drawn when a page is expanded.
///
/// \sa GetExpandedBottomBorder
void wxAccordion::SetExpandedBottomBorder(bool b){hasBottomBorderWhenExpanded=b;changesMade=true;}

/// \brief Sets the minimum height of the accordion's caption bars.
///
/// \sa GetMinCaptionHeight
void wxAccordion::SetMinCaptionHeight(int m){minCaptionHeight=m>=0?m:0;changesMade=true;}

/// \brief Sets the size of the margins (in pixels) that will be used for the icons when drawing the accordion's caption bars.
///
/// \sa GetIconMargin
void wxAccordion::SetIconMargin(const wxSize& p){iconMargin=p;changesMade=true;}

/// \brief Sets the size of the margins (in pixels) that will be used for the label when drawing the accordion's caption bars.
///
/// \sa GetTextMargin
void wxAccordion::SetTextMargin(const wxSize& p){textMargin=p;changesMade=true;}

/// \brief Sets the size of the margins (in pixels) that will be used for the expand and collapse buttons when drawing the accordion's caption bars.
///
/// \sa GetButtonMargin
void wxAccordion::SetButtonMargin(const wxSize& p){buttonMargin=p;changesMade=true;}

/// \brief Sets the bitmap used for the expand button of the accordion's caption bars.
///
/// \sa GetCollapseButton
void wxAccordion::SetCollapseButton(const wxBitmap& b){collapseButton=b;changesMade=true;}

/// \brief Sets the bitmap used for the collapse button of the accordion's caption bars.
///
/// \sa GetExpandButton
void wxAccordion::SetExpandButton(const wxBitmap& b){expandButton=b;changesMade=true;}

/// \brief Sets whether or not the accordion's caption bars will change to the highlighted style when the mouse cursor is over them.
///
/// If this function is called with a value of true, the SetCollapsedHLXXX and SetExpandedHLXXX functions should be used to set styles for the highlighted states appropriate for the current styles of the collapsed and expanded states.
/// \sa GetUseHighlighting
void wxAccordion::SetUseHighlighting(bool b)
{
    if(useHighlighting!=b)
    {
        useHighlighting=b;
        changesMade=true;

        for(size_t i=0;i<GetPageCount();i++)
        {
            getFold(i)->SetHighlighting(b);
        }
    }
}

/// \brief Sets whether or not the accordion's caption bars will use a custom disables state.
///
/// If this function is called with a value of true, the disabled state of the caption bars will be drawn with the style determined by the SetDisabledXXX functions.  Otherwise, the disabled state will be drawn as a grayed version of the collapsed state.
/// \sa GetCustomDisabledState
void wxAccordion::SetCustomDisabledState(bool b)      { customDisabledState=b;;changesMade=true;}

/// \brief Sets the disabled brightness of the accordion's caption bars.
///
/// By default, the disabled state of a caption bar is a grayed version of the collapsed caption bar style.  This function determines the brightness of that grayness.  Values close to 0 will produce a very dark caption bar.  Values close to 255 will produce a very light caption bar.
/// This setting will not be used if SetCustomDisabledState() is called with a value of true.  Instead the disabled state of the caption bars will be drawn with the style determined by the SetDisabledXXX functions.
/// \sa GetDisabledBrightness
void wxAccordion::SetDisabledBrightness(int b)
{
    if(b<0)
    {
        b=0;
    }

    if(b>255)
    {
        b=255;
    }

    if(disabledBrightness!=b)
    {
        disabledBrightness=b;
        changesMade=true;
    }
}


/////////////////////////////////////////////////
////  Caption bar collapsed state style get/set functions
//////////////////////////////////////////////////

/// \brief Returns a copy of the gradient stops used for drawing the caption bar in the collapsed state.
///
/// \sa AddCollapsedGradientStop, ClearCollapsedGradientStops
wxGraphicsGradientStops wxAccordion::GetCollapsedGradientStops() const {return collapsedStops;}

/// \brief Returns a copy of the bitmap used for drawing the caption bar in the collapsed state.
///
/// \sa SetCollapsedBGBitmap
wxBitmap wxAccordion::GetCollapsedBGBitmap()       const {return collapsedBG;}

/// \brief Returns the first (top) colour used for drawing the caption bar in the collapsed state.
///
/// \sa SetCollapsedColour1
wxColour wxAccordion::GetCollapsedColour1()        const {return collapsedStops.GetStartColour();}

/// \brief Returns the second (bottom) colour used for drawing the caption bar in the collapsed state.
///
/// \sa SetCollapsedColour2
wxColour wxAccordion::GetCollapsedColour2()        const {return collapsedStops.GetEndColour();}

/// \brief Returns the border colour used for drawing the caption bar in the collapsed state.
///
/// \sa SetCollapsedBorderColour
wxColour wxAccordion::GetCollapsedBorderColour()   const {return collapsedBorderColour;}

/// \brief Returns the text colour used for the caption bar in the collapsed state.
///
/// \sa SetCollapsedTextColour
wxColour wxAccordion::GetCollapsedTextColour()     const {return collapsedTextColour;}

/// \brief Returns a copy of the font used for the caption bar in the collapsed state.
///
/// \sa SetCollapsedFont
wxFont   wxAccordion::GetCollapsedFont()           const {return collapsedFont;}

/// \brief Clears any gradient stops that have been set for drawing the caption bar in the collapsed state.
///
/// \sa GetCollapsedGradientStops, AddCollapsedGradientStop
void wxAccordion::ClearCollapsedGradientStops(){collapsedStops=wxGraphicsGradientStops(collapsedStops.GetStartColour(), collapsedStops.GetEndColour());changesMade=true;}

/// \brief Adds a gradient stop to the set of colours used for drawing the caption bar in the collapsed state.
///
/// \sa GetCollapsedGradientStops, ClearCollapsedGradientStops
void   wxAccordion::AddCollapsedGradientStop (const wxColour& col,float pos){collapsedStops.Add( col, pos);changesMade=true;}

/// \brief Sets the bitmap used for drawing the caption bar in the collapsed state.
///
/// To clear the bitmap, call this function with wxNullBitmap as the parameter.
/// \sa GetCollapsedBGBitmap
void   wxAccordion::SetCollapsedBGBitmap     (const wxBitmap& b){collapsedBG=b;changesMade=true;}

/// \brief Sets the first (top) colour used for drawing the caption bar in the collapsed state.
///
/// \sa GetCollapsedColour1
void   wxAccordion::SetCollapsedColour1      (const wxColour& col){collapsedStops.SetStartColour(col);changesMade=true;}

/// \brief Sets the second (bottom) colour used for drawing the caption bar in the collapsed state.
///
/// \sa GetCollapsedColour2
void   wxAccordion::SetCollapsedColour2      (const wxColour& col){collapsedStops.SetEndColour(col);changesMade=true;}

/// \brief Sets the border colour used for drawing the caption bar in the collapsed state.
///
/// \sa GetCollapsedBorderColour
void   wxAccordion::SetCollapsedBorderColour (const wxColour& col){collapsedBorderColour=col;lastBorder->SetForegroundColour(col);changesMade=true;}

/// \brief Sets the text colour used for the caption bar in the collapsed state.
///
/// \sa GetCollapsedTextColour
void   wxAccordion::SetCollapsedTextColour   (const wxColour& col){collapsedTextColour=col;changesMade=true;}

/// \brief Sets the font used for the caption bar in the collapsed state.
///
/// \sa GetCollapsedFont
void   wxAccordion::SetCollapsedFont         (const wxFont& f){collapsedFont=f;changesMade=true;}


/////////////////////////////////////////////////
////  Caption bar expanded state style get/set functions
//////////////////////////////////////////////////

/// \brief Returns a copy of the gradient stops used for drawing the caption bar in the expanded state.
///
/// \sa AddExpandedGradientStop, ClearExpandedGradientStops
wxGraphicsGradientStops wxAccordion::GetExpandedGradientStops() const {return expandedStops;}

/// \brief Returns a copy of the bitmap used for drawing the caption bar in the expanded state.
///
/// \sa SetExpandedBGBitmap
wxBitmap wxAccordion::GetExpandedBGBitmap()        const {return expandedBG;}

/// \brief Returns the first (top) colour used for drawing the caption bar in the expanded state.
///
/// \sa SetExpandedColour1
wxColour wxAccordion::GetExpandedColour1()         const {return expandedStops.GetStartColour();}

/// \brief Returns the second (bottom) colour used for drawing the caption bar in the expanded state.
///
/// \sa SetExpandedColour2
wxColour wxAccordion::GetExpandedColour2()         const {return expandedStops.GetEndColour();}

/// \brief Returns the border colour used for drawing the caption bar in the expanded state.
///
/// \sa SetExpandedBorderColour
wxColour wxAccordion::GetExpandedBorderColour()    const {return expandedBorderColour;}

/// \brief Returns the text colour used for the caption bar in the expanded state.
///
/// \sa SetExpandedTextColour
wxColour wxAccordion::GetExpandedTextColour()      const {return expandedTextColour;}

/// \brief Returns a copy of the font used for the caption bar in the expanded state.
///
/// \sa SetExpandedFont
wxFont   wxAccordion::GetExpandedFont()            const {return expandedFont;}

/// \brief Clears any gradient stops that have been set for drawing the caption bar in the expanded state.
///
/// \sa GetExpandedGradientStops, AddExpandedGradientStop
void wxAccordion::ClearExpandedGradientStops(){expandedStops=wxGraphicsGradientStops(expandedStops.GetStartColour(), expandedStops.GetEndColour());changesMade=true;}

/// \brief Adds a gradient stop to the set of colours used for drawing the caption bar in the expanded state.
///
/// \sa GetExpandedGradientStops, ClearExpandedGradientStops
void   wxAccordion::AddExpandedGradientStop (const wxColour& col,float pos){expandedStops.Add( col, pos);changesMade=true;}

/// \brief Sets the bitmap used for drawing the caption bar in the expanded state.
///
/// To clear the bitmap, call this function with wxNullBitmap as the parameter.
/// \sa GetExpandedBGBitmap
void   wxAccordion::SetExpandedBGBitmap     (const wxBitmap& b) {expandedBG=b;changesMade=true;}

/// \brief Sets the first (top) colour used for drawing the caption bar in the expanded state.
///
/// \sa GetExpandedColour1
void   wxAccordion::SetExpandedColour1      (const wxColour& col){expandedStops.SetStartColour(col);changesMade=true;}

/// \brief Sets the second (bottom) colour used for drawing the caption bar in the expanded state.
///
/// \sa GetExpandedColour2
void   wxAccordion::SetExpandedColour2      (const wxColour& col){expandedStops.SetEndColour(col);changesMade=true;}

/// \brief Sets the border colour used for drawing the caption bar in the expanded state.
///
/// \sa GetExpandedBorderColour
void   wxAccordion::SetExpandedBorderColour (const wxColour& col){expandedBorderColour=col;changesMade=true;}

/// \brief Sets the text colour used for the caption bar in the expanded state.
///
/// \sa GetExpandedTextColour
void   wxAccordion::SetExpandedTextColour   (const wxColour& col){expandedTextColour=col;changesMade=true;}

/// \brief Sets the font used for the caption bar in the expanded state.
///
/// \sa GetExpandedFont
void   wxAccordion::SetExpandedFont         (const wxFont& f)   {expandedFont=f;changesMade=true;}


/////////////////////////////////////////////////
////  Caption bar collapsed highlighted state style get/set functions
//////////////////////////////////////////////////

/// \brief Returns a copy of the gradient stops used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, AddCollapsedHLGradientStop, ClearCollapsedHLGradientStops
wxGraphicsGradientStops wxAccordion::GetCollapsedHLGradientStops() const {return collapsedHLStops;}

/// \brief Returns a copy of the bitmap used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetCollapsedHLBGBitmap
wxBitmap wxAccordion::GetCollapsedHLBGBitmap()     const {return collapsedHLBG;}

/// \brief Returns the first (top) colour used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetCollapsedHLColour1
wxColour wxAccordion::GetCollapsedHLColour1()      const {return collapsedHLStops.GetStartColour();}

/// \brief Returns the second (bottom) colour used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetCollapsedHLColour2
wxColour wxAccordion::GetCollapsedHLColour2()      const {return collapsedHLStops.GetEndColour();}

/// \brief Returns the border colour used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetCollapsedHLBorderColour
wxColour wxAccordion::GetCollapsedHLBorderColour() const {return collapsedHLBorderColour;}

/// \brief Returns the text colour used for the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetCollapsedHLTextColour
wxColour wxAccordion::GetCollapsedHLTextColour()   const {return collapsedHLTextColour;}

/// \brief Returns a copy of the font used for the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetCollapsedHLFont
wxFont   wxAccordion::GetCollapsedHLFont()         const {return collapsedHLFont;}

/// \brief Clears any gradient stops that have been set for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetCollapsedHLGradientStops, AddCollapsedHLGradientStop
void wxAccordion::ClearCollapsedHLGradientStops(){collapsedHLStops=wxGraphicsGradientStops(collapsedHLStops.GetStartColour(), collapsedHLStops.GetEndColour());changesMade=true;}

/// \brief Adds a gradient stop to the set of colours used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetCollapsedHLGradientStops, ClearCollapsedHLGradientStops
void   wxAccordion::AddCollapsedHLGradientStop (const wxColour& col,float pos){collapsedHLStops.Add( col, pos);changesMade=true;}

/// \brief Sets the bitmap used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// To clear the bitmap, call this function with wxNullBitmap as the parameter.
/// \sa SetUseHighlighting, GetCollapsedHLBGBitmap
void   wxAccordion::SetCollapsedHLBGBitmap     (const wxBitmap& b){collapsedHLBG=b;changesMade=true;}

/// \brief Sets the first (top) colour used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetCollapsedHLColour1
void   wxAccordion::SetCollapsedHLColour1      (const wxColour& col){collapsedHLStops.SetStartColour(col);changesMade=true;}

/// \brief Sets the second (bottom) colour used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetCollapsedColour2
void   wxAccordion::SetCollapsedHLColour2      (const wxColour& col){collapsedHLStops.SetEndColour(col);changesMade=true;}

/// \brief Sets the border colour used for drawing the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetCollapsedHLBorderColour
void   wxAccordion::SetCollapsedHLBorderColour (const wxColour& col){collapsedHLBorderColour=col;changesMade=true;}

/// \brief Sets the text colour used for the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetCollapsedHLTextColour
void   wxAccordion::SetCollapsedHLTextColour   (const wxColour& col){collapsedHLTextColour=col;changesMade=true;}

/// \brief Sets the font used for the caption bar in the collapsed highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetCollapsedHLFont
void   wxAccordion::SetCollapsedHLFont         (const wxFont& f){collapsedHLFont=f;changesMade=true;}


/////////////////////////////////////////////////
////  Caption bar expanded highlighted state style get/set functions
//////////////////////////////////////////////////

/// \brief Returns a copy of the gradient stops used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, ClearExpandedHLGradientStops, AddExpandedHLGradientStop
wxGraphicsGradientStops wxAccordion::GetExpandedHLGradientStops() const {return expandedHLStops;}

/// \brief Returns a copy of the bitmap used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetExpandedHLBGBitmap
wxBitmap wxAccordion::GetExpandedHLBGBitmap()      const {return expandedHLBG;}

/// \brief Returns the first (top) colour used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetExpandedHLColour1
wxColour wxAccordion::GetExpandedHLColour1()       const {return expandedHLStops.GetStartColour();}

/// \brief Returns the second (bottom) colour used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetExpandedHLColour2
wxColour wxAccordion::GetExpandedHLColour2()       const {return expandedHLStops.GetEndColour();}

/// \brief Returns the border colour used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetExpandedHLBorderColour
wxColour wxAccordion::GetExpandedHLBorderColour()  const {return expandedHLBorderColour;}

/// \brief Returns the text colour used for the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetExpandedHLTextColour
wxColour wxAccordion::GetExpandedHLTextColour()    const {return expandedHLTextColour;}

/// \brief Returns a copy of the font used for the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, SetExpandedHLFont
wxFont   wxAccordion::GetExpandedHLFont()          const {return expandedHLFont;}

/// \brief Clears any gradient stops that have been set for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetExpandedHLGradientStops, AddExpandedHLGradientStop
void wxAccordion::ClearExpandedHLGradientStops(){expandedHLStops=wxGraphicsGradientStops(expandedHLStops.GetStartColour(), expandedHLStops.GetEndColour());changesMade=true;}

/// \brief Adds a gradient stop to the set of colours used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetExpandedHLGradientStops, ClearExpandedHLGradientStops
void   wxAccordion::AddExpandedHLGradientStop (const wxColour& col,float pos){expandedHLStops.Add( col, pos);changesMade=true;}

/// \brief Sets the bitmap used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// To clear the bitmap, call this function with wxNullBitmap as the parameter.
/// \sa SetUseHighlighting, GetExpandedHLBGBitmap
void   wxAccordion::SetExpandedHLBGBitmap     (const wxBitmap& b) {expandedHLBG=b;changesMade=true;}

/// \brief Sets the first (top) colour used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetExpandedHLColour1
void   wxAccordion::SetExpandedHLColour1      (const wxColour& col){expandedHLStops.SetStartColour(col);changesMade=true;}

/// \brief Sets the second (bottom) colour used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetExpandedHLColour2
void   wxAccordion::SetExpandedHLColour2      (const wxColour& col){expandedHLStops.SetEndColour(col);changesMade=true;}

/// \brief Sets the border colour used for drawing the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetExpandedHLBorderColour
void   wxAccordion::SetExpandedHLBorderColour (const wxColour& col){expandedHLBorderColour=col;changesMade=true;}

/// \brief Sets the text colour used for the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetExpandedHLTextColour
void   wxAccordion::SetExpandedHLTextColour   (const wxColour& col){expandedHLTextColour=col;changesMade=true;}

/// \brief Sets the font used for the caption bar in the expanded highlighted state.
///
/// This setting will not be used unless SetUseHighlighting() has been called with a value of true.
/// \sa SetUseHighlighting, GetExpandedHLFont
void   wxAccordion::SetExpandedHLFont         (const wxFont& f)   {expandedHLFont=f;changesMade=true;}

/////////////////////////////////////////////////
////  Caption bar disabled state style get/set functions
//////////////////////////////////////////////////

/// \brief Returns a copy of the gradient stops used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, AddDisabledGradientStop, ClearDisabledGradientStops
wxGraphicsGradientStops wxAccordion::GetDisabledGradientStops() const {return disabledStops;}

/// \brief Returns a copy of the bitmap used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, SetDisabledBGBitmap
wxBitmap wxAccordion::GetDisabledBGBitmap()        const {return disabledBG;}

/// \brief Returns the first (top) colour used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, SetDisabledColour1
wxColour wxAccordion::GetDisabledColour1()         const {return disabledStops.GetStartColour();}

/// \brief Returns the second (bottom) colour used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, SetDisabledColour2
wxColour wxAccordion::GetDisabledColour2()         const {return disabledStops.GetEndColour();}

/// \brief Returns the border colour used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, SetDisabledBorderColour
wxColour wxAccordion::GetDisabledBorderColour()    const {return disabledBorderColour;}

/// \brief Returns the text colour used for the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, SetDisabledTextColour
wxColour wxAccordion::GetDisabledTextColour()      const {return disabledTextColour;}

/// \brief Returns a copy of the font used for the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, SetDisabledFont
wxFont   wxAccordion::GetDisabledFont()            const {return disabledFont;}

/// \brief Clears any gradient stops that have been set for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, GetDisabledGradientStops, AddDisabledGradientStop
void wxAccordion::ClearDisabledGradientStops(){disabledStops=wxGraphicsGradientStops(disabledStops.GetStartColour(), disabledStops.GetEndColour());changesMade=true;}

/// \brief Adds a gradient stop to the set of colours used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, GetDisabledGradientStops, ClearDisabledGradientStops
void   wxAccordion::AddDisabledGradientStop (const wxColour& col,float pos){disabledStops.Add( col, pos);changesMade=true;}

/// \brief Sets the bitmap used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// To clear the bitmap, call this function with wxNullBitmap as the parameter.
/// \sa SetCustomDisabledState, GetDisabledBGBitmap
void   wxAccordion::SetDisabledBGBitmap     (const wxBitmap& b) {disabledBG=b;changesMade=true;}

/// \brief Sets the first (top) colour used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, GetDisabledColour1
void   wxAccordion::SetDisabledColour1      (const wxColour& col){disabledStops.SetStartColour(col);changesMade=true;}

/// \brief Sets the second (bottom) colour used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, GetDisabledColour2
void   wxAccordion::SetDisabledColour2      (const wxColour& col){disabledStops.SetEndColour(col);changesMade=true;}

/// \brief Sets the border colour used for drawing the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, GetDisabledBorderColour
void   wxAccordion::SetDisabledBorderColour (const wxColour& col){disabledBorderColour=col;changesMade=true;}

/// \brief Sets the text colour used for the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, GetDisabledTextColour
void   wxAccordion::SetDisabledTextColour   (const wxColour& col){disabledTextColour=col;changesMade=true;}

/// \brief Sets the font used for the caption bar in the disabled state.
///
/// This setting will not be used unless SetCustomDisabledState() has been called with a value of true.
/// \sa SetCustomDisabledState, GetDisabledFont
void   wxAccordion::SetDisabledFont         (const wxFont& f)   {disabledFont=f;changesMade=true;}


