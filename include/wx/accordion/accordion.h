/////////////////////////////////////////////////////////////////////////////
// Name:        accordion.h
// Purpose:     wxAccordion class interface
// Author:      ()
// Created:     2015-10-10
// Copyright:   ()
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ACCORDION_ACCORDION_H_
#define _WX_ACCORDION_ACCORDION_H_

#ifdef WXUSING_ACCORDION_SOURCE
    #define WXDLLIMPEXP_ACCORDION
#elif WXMAKINGDLL_ACCORDION
    #define WXDLLIMPEXP_ACCORDION WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_ACCORDION WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_ACCORDION
#endif

#include <wx/bookctrl.h>
#include <wx/graphics.h>

// wxAccordion style flags
enum
{
    wxAC_DEFAULT_STYLE      = 0,
    wxAC_BUTTON_LEFT        = 1 << 0,
    wxAC_SINGLE_FOLD        = 1 << 1,
    wxAC_COLLAPSE_TO_BOTTOM = 1 << 2,
    wxAC_FLOAT_TO_TOP       = 1 << 3
};

enum wxAccordionToggleStyle
{
    wxAC_TOGGLE_ONLY_ON_BUTTON_CLICKS,  ///< Toggle only if the click was on the button.  This is the default behaviour.
    wxAC_TOGGLE_ON_ANY_CLICK            ///< Toggle on any click even in a button is visible.
};

enum wxAccordionBorderStyle
{
    wxAC_BORDERS_FULL,                     ///< The caption bars are always drawn with all borders.  This is the default behaviour.
    wxAC_BORDERS_NO_BOTTOM_WHEN_EXPANDED   ///< When a page is expanded, the bottom border of the caption bar is not shown.
};

enum wxAccordionHighlighting
{
    wxAC_HIGHLIGHTING_NONE,         ///< No highlighting is done.
    wxAC_HIGHLIGHTING_MOUSEOVER     ///< A caption bar changes to a highlighted state when the mouse is over it.
};

enum wxAccordionDisabledState
{
    wxAC_DISABLED_STATE_DEFAULT,   ///< A disables caption bar is drawn as a grayed version of the collapsed state.
    wxAC_DISABLED_STATE_CUSTOM     ///< The user can specify style options for the disabled caption bar as with all the other states.
};

// values for the `flags' parameter of wxAccordion::HitTest()
static const int wxACCORDION_HITTEST_NOWHERE         = 1 << 0;
static const int wxACCORDION_HITTEST_ONCAPTION       = 1 << 1;
static const int wxACCORDION_HITTEST_ONCAPTIONBORDER = 1 << 2;
static const int wxACCORDION_HITTEST_ONCAPTIONLABEL  = 1 << 3;
static const int wxACCORDION_HITTEST_ONCAPTIONICON   = 1 << 4;
static const int wxACCORDION_HITTEST_ONCAPTIONBUTTON = 1 << 5;
static const int wxACCORDION_HITTEST_ONPAGE          = 1 << 6;
static const int wxACCORDION_HITTEST_ONPAGEBORDER    = 1 << 7;
static const int wxACCORDION_HITTEST_INPAGEPADDING   = 1 << 8;

class WXDLLIMPEXP_ACCORDION wxAccordionStyle
{
    friend class wxAccordion;
    friend class wxAccordionFold;

    public:
        wxAccordionStyle();

        wxColour GetColour1() const;
        wxGraphicsGradientStops GetGradientStops() const;
        wxColour GetColour2() const;
        wxBitmap GetBGBitmap() const;
        wxColour GetBorderColour() const;
        wxColour GetTextColour() const;
        wxFont GetFont() const;

        void SetColour1(const wxColour& col);
        void ClearGradientStops();
        void AddGradientStop(const wxColour& col,float pos);
        void SetColour2(const wxColour& col);
        void SetBGBitmap(const wxBitmap& b);
        void SetBorderColour(const wxColour& col);
        void SetTextColour(const wxColour& col);
        void SetFont(const wxFont& f);

    private:
        wxGraphicsGradientStops m_stops;
        wxColour m_borderColour;
        wxBitmap m_background;
        wxColour m_textColour;
        wxFont m_font;
        wxAccordion* m_accordion;
        bool m_isPageStyle;
};

class wxAccordionFold;
class wxPanel;

class WXDLLIMPEXP_ACCORDION wxAccordion:public wxBookCtrlBase
{
    wxDECLARE_CLASS(wxAccordion);
    friend class wxAccordionFold;
    friend class wxAccordionStyle;

    public:
        //house keeping
        wxAccordion();
        wxAccordion( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style=wxTAB_TRAVERSAL, const wxString &name="accordion" );
        virtual ~wxAccordion();
        bool Create(wxWindow* parent,wxWindowID id = wxID_ANY,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxTAB_TRAVERSAL,const wxString &name="accordion");

        #ifndef _DOXYGEN_
            //There is no need to document this stuff
            //abstract wxBookCtrlBase methods that MUST be implimented:
            virtual bool     SetPageText(size_t n, const wxString& strText);
            virtual wxString GetPageText(size_t n) const;
            virtual int      GetPageImage(size_t n) const;
            virtual bool     SetPageImage(size_t n, int imageId);
            virtual bool     InsertPage( size_t n, wxWindow *page, const wxString& text, bool bSelect = false, int imageId = NO_IMAGE );
            virtual int      SetSelection(size_t n);
            virtual int      ChangeSelection(size_t n);
            virtual wxWindow *DoRemovePage(size_t page);

            //other virtual methods that were reimplimented
            virtual bool Layout();
            virtual bool SetBackgroundColour(const wxColour &colour);
            virtual void SetWindowStyleFlag(long style);
            virtual void Refresh(bool eraseBackground=true, const wxRect *rect = NULL);
        #endif


        ///@{ \name Accordion management functions
            void Expand(size_t n);
            void Collapse(size_t n);
            void Toggle(size_t n);
            void Enable(size_t n);
            void Disable(size_t n);

            bool IsExpanded(size_t n) const;
            bool IsCollapsed(size_t n) const;
            bool IsEnabled(size_t n) const;
            bool IsDisabled(size_t n) const;

            void SetFixedSize(size_t n);
            void SetFixedWidth(size_t n);
            void SetFixedHeight(size_t n);

            //Getters for computed quantities
            int GetCaptionBarHeight() const;

            //technically a virtual method that was reimplimented
            virtual int HitTest( const wxPoint& pt, long* flags = NULL ) const;
        ///@}


        ///@{ \name Accordion style get/set functions
            int                    GetAccordionPadding()  const;
            wxBitmap               GetAccordionBGBitmap() const;
            wxAccordionToggleStyle GetToggleStyle()       const;

            void SetAccordionPadding(int m);
            void SetAccordionBGBitmap(const wxBitmap&);
            void SetToggleStyle(wxAccordionToggleStyle toggleStyle);
        ///@}


        ///@{ \name Caption bar style get/set functions
            double                   GetCaptionRadius()        const;
            double                   GetCaptionGradientAngle() const;
            wxAccordionBorderStyle   GetCaptionBorderStyle()   const;
            int                      GetMinCaptionHeight()     const;
            wxSize                   GetIconMargin()           const;
            wxSize                   GetTextMargin()           const;
            wxSize                   GetButtonMargin()         const;
            wxBitmap                 GetCollapseButton()       const;
            wxBitmap                 GetExpandButton()         const;
            wxAccordionHighlighting  GetHighlighting()         const;
            wxAccordionDisabledState GetDisabledState()        const;
            int                      GetDisabledBrightness()   const;
            wxAccordionStyle&        GetCollapsedStyle();
            wxAccordionStyle&        GetExpandedStyle();
            wxAccordionStyle&        GetCollapsedHLStyle();
            wxAccordionStyle&        GetExpandedHLStyle();
            wxAccordionStyle&        GetDisabledStyle();

            void SetCaptionRadius(double d);
            void SetCaptionGradientAngle(double a);
            void SetCaptionBorderStyle(wxAccordionBorderStyle borderStyle);
            void SetMinCaptionHeight(int m);
            void SetIconMargin(const wxSize& p);
            void SetTextMargin(const wxSize& p);
            void SetButtonMargin(const wxSize& p);
            void SetCollapseButton(const wxBitmap& b);
            void SetExpandButton(const wxBitmap& b);
            void SetHighlighting(wxAccordionHighlighting highlighting);
            void SetDisabledState(wxAccordionDisabledState state);
            void SetDisabledBrightness(int b);
            void SetCollapsedStyle(const wxAccordionStyle&);
            void SetExpandedStyle(const wxAccordionStyle&);
            void SetCollapsedHLStyle(const wxAccordionStyle&);
            void SetExpandedHLStyle(const wxAccordionStyle&);
            void SetDisabledStyle(const wxAccordionStyle&);
        ///@}


        ///@{ \name Page style get/set functions
            double            GetPageRadius()  const;
            int               GetPagePadding() const;
            wxAccordionStyle& GetPageStyle();

            void SetPageRadius(double d);
            void SetPagePadding(int m);
            void SetPageStyle(const wxAccordionStyle&);
        ///@}

    protected:
        void Init();
        void InitAccordion(long style);

    private:
        //event handlers
        void onSize( wxSizeEvent& event );
        void onLeftUp( wxMouseEvent& event );
        void onLeftDClick( wxMouseEvent& event );
        void onPaint( wxPaintEvent& event );
        void onPaintBorder( wxPaintEvent& event );

        //helper methods
        wxAccordionFold* getFold(size_t) const;
        void computeCaptionInfo();
        void toggle(wxAccordionFold*,size_t ,bool);
        void setSecondSpacerProp(int p);
        void setFirstSpacerHeight(int h);
        void rebuildFloats();
        bool changesWereMade();
        void setPageBorderColourHelper(const wxColour& col);
        void buildBitmap(wxBitmap& out, bool bottomBorder, int drawtype, int width,int height, double x1,double y1,double x2,double y2, double radius, const wxAccordionStyle& style);
        void adjustForStyle( long style );

        //private data
        //helper data:
        bool m_changesMade;
        wxImageList* m_trackedImageList;
        wxPanel* m_lastBorder;

        //computed:
        int m_captionBarHeight;
        wxRect m_buttonRect;
        wxRect m_iconRect;
        wxPoint m_collapsedTextPoint;
        wxPoint m_expandedTextPoint;
        wxPoint m_collapsedHLTextPoint;
        wxPoint m_expandedHLTextPoint;
        wxPoint m_disabledTextPoint;
        int m_textWidth;
        wxBitmap m_captionBarCollapsed;
        wxBitmap m_captionBarExpanded;
        wxBitmap m_captionBarCollapsedHL;
        wxBitmap m_captionBarExpandedHL;
        wxBitmap m_captionBarDisabled;

        //style items:
        int m_accordionPadding;
        wxAccordionToggleStyle m_toggleStyle;
        double m_captionBarRadius;
        double m_gradientAngle;
        wxAccordionBorderStyle m_captionBorderStyle;
        int m_minCaptionHeight;
        wxSize m_iconMargin;
        wxSize m_textMargin;
        wxSize m_buttonMargin;
        wxBitmap m_collapseButton;
        wxBitmap m_expandButton;
        wxAccordionDisabledState m_disabledState;
        wxAccordionHighlighting m_highlighting;
        int  m_disabledBrightness;

        int m_pagePadding;
        double m_pageRadius;

        wxBitmap m_accordionBG;

        wxAccordionStyle m_collapsedStyle;
        wxAccordionStyle m_expandedStyle;
        wxAccordionStyle m_collapsedHLStyle;
        wxAccordionStyle m_expandedHLStyle;
        wxAccordionStyle m_disabledStyle;
        wxAccordionStyle m_pageStyle;
};

//Event IDs

wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_ACCORDION, wxEVT_ACCORDION_COLLAPSED, wxBookCtrlEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_ACCORDION, wxEVT_ACCORDION_COLLAPSING, wxBookCtrlEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_ACCORDION, wxEVT_ACCORDION_EXPANDED, wxBookCtrlEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_ACCORDION, wxEVT_ACCORDION_EXPANDING, wxBookCtrlEvent);

#define EVT_ACCORDION_COLLAPSED(winid, fn) \
    wx__DECLARE_EVT1(wxEVT_ACCORDION_COLLAPSED, winid, wxBookCtrlEventHandler(fn))
#define EVT_ACCORDION_COLLAPSING(winid, fn) \
    wx__DECLARE_EVT1(wxEVT_ACCORDION_COLLAPSING, winid, wxBookCtrlEventHandler(fn))
#define EVT_ACCORDION_EXPANDED(winid, fn) \
    wx__DECLARE_EVT1(wxEVT_ACCORDION_EXPANDED, winid, wxBookCtrlEventHandler(fn))
#define EVT_ACCORDION_EXPANDING(winid, fn) \
    wx__DECLARE_EVT1(wxEVT_ACCORDION_EXPANDING, winid, wxBookCtrlEventHandler(fn))

/// \file accordion.h The include file for the wxAccordion class.

#endif // _WX_ACCORDION_ACCORDION_H_
