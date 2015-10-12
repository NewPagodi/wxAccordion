/***************************************************************
 * Name:      accordion.h
 * Purpose:   wxAccordion class interface
 * Author:    ()
 * Created:   2015-10-10
 * Copyright: ()
 * License:   wxWindows licence
 **************************************************************/

#ifndef WXACCORDION_H_INCLUDED
#define WXACCORDION_H_INCLUDED

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
#include <wx/panel.h>

// ----------------------------------------------------------------------------
// wxAccordion flags
// ----------------------------------------------------------------------------
enum wxAccordionOption
{
    wxACCORDION_BUTTON_LEFT        = 1 << 0,
    wxACCORDION_SINGLE_FOLD        = 1 << 1,
    wxACCORDION_COLLAPSE_TO_BOTTOM = 1 << 2,
    wxACCORDION_FLOAT_TO_TOP       = 1 << 3,

    wxACCORDION_DEFAULT_STYLE    = 0
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

class wxAccordionFold;

class WXDLLIMPEXP_ACCORDION wxAccordion:public wxBookCtrlBase
{
    wxDECLARE_CLASS(wxAccordion);
    friend class wxAccordionFold;

    public:
        //house keeping
        wxAccordion();
        wxAccordion( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style=wxTAB_TRAVERSAL, const wxString &name="accordion" );
        virtual ~wxAccordion();
        bool Create(wxWindow* parent,wxWindowID id = wxID_ANY,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,long style = wxTAB_TRAVERSAL,const wxString &name="accordion");

        //There is no need to document these
        #ifndef _DOXYGEN_
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
            //Accordion methods
            void Expand(size_t n);
            void Collapse(size_t n);
            void Toggle(size_t n);
            bool IsExpanded(size_t n) const;
            bool IsCollapsed(size_t n) const;
            void Enable(size_t n);
            void Disable(size_t n);
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


        //Style item getters/setters
        ///@{ \name Accordion style get/set functions
            int      GetAccordionPadding()        const;
            wxBitmap GetAccordionBGBitmap()       const;
            bool     GetOnlyToggleWithButton()    const;

            void     SetAccordionPadding(int m);
            void     SetAccordionBGBitmap(const wxBitmap&);
            void     SetOnlyToggleWithButton(bool b);
        ///@}


        ///@{ \name Page style get/set functions
            double   GetPageRadius()              const;
            int      GetPagePadding()             const;
            wxBitmap GetPageBGBitmap()            const;
            wxGraphicsGradientStops GetPageGradientStops() const;
            wxColour GetPageColour1()             const;
            wxColour GetPageColour2()             const;
            wxColour GetPageBorderColour()        const;

            void     SetPageRadius(double d);
            void     SetPagePadding(int m);
            void     SetPageBGBitmap(const wxBitmap& b);
            void     AddPageGradientStop(const wxColour& col,float pos);
            void     ClearPageGradientStops();
            void     SetPageColour1(const wxColour& col);
            void     SetPageColour2(const wxColour& col);
            void     SetPageBorderColour(const wxColour& col);
        ///@}


        ///@{ \name Caption bar style get/set functions
            double   GetCaptionRadius()           const;
            double   GetCaptionGradientAngle()    const;
            bool     GetExpandedBottomBorder()    const;
            int      GetMinCaptionHeight()        const;
            wxSize   GetIconMargin()              const;
            wxSize   GetTextMargin()              const;
            wxSize   GetButtonMargin()            const;
            wxBitmap GetCollapseButton()          const;
            wxBitmap GetExpandButton()            const;
            bool     GetUseHighlighting()         const;
            bool     GetCustomDisabledState()     const;
            int      GetDisabledBrightness()      const;

            void SetCaptionRadius(double d);
            void SetCaptionGradientAngle(double a);
            void SetExpandedBottomBorder(bool b);
            void SetMinCaptionHeight(int m);
            void SetIconMargin(const wxSize& p);
            void SetTextMargin(const wxSize& p);
            void SetButtonMargin(const wxSize& p);
            void SetCollapseButton(const wxBitmap& b);
            void SetExpandButton(const wxBitmap& b);
            void SetUseHighlighting(bool b);
            void SetCustomDisabledState(bool b);
            void SetDisabledBrightness(int b);
        ///@}


        ///@{ \name Caption bar collapsed state style get/set functions
            wxGraphicsGradientStops GetCollapsedGradientStops() const;
            wxBitmap GetCollapsedBGBitmap()       const;
            wxColour GetCollapsedColour1()        const;
            wxColour GetCollapsedColour2()        const;
            wxColour GetCollapsedBorderColour()   const;
            wxColour GetCollapsedTextColour()     const;
            wxFont   GetCollapsedFont()           const;

            void     ClearCollapsedGradientStops();
            void     AddCollapsedGradientStop (const wxColour& col,float pos);
            void     SetCollapsedBGBitmap     (const wxBitmap& b);
            void     SetCollapsedColour1      (const wxColour& col);
            void     SetCollapsedColour2      (const wxColour& col);
            void     SetCollapsedBorderColour (const wxColour& col);
            void     SetCollapsedTextColour   (const wxColour& col);
            void     SetCollapsedFont         (const wxFont& f);
        ///@}


        ///@{ \name Caption bar expanded state style get/set functions
            wxGraphicsGradientStops GetExpandedGradientStops() const;
            wxBitmap GetExpandedBGBitmap()        const;
            wxColour GetExpandedColour1()         const;
            wxColour GetExpandedColour2()         const;
            wxColour GetExpandedBorderColour()    const;
            wxColour GetExpandedTextColour()      const;
            wxFont   GetExpandedFont()            const;

            void     ClearExpandedGradientStops();
            void     AddExpandedGradientStop (const wxColour& col,float pos);
            void     SetExpandedBGBitmap     (const wxBitmap& b);
            void     SetExpandedColour1      (const wxColour& col);
            void     SetExpandedColour2      (const wxColour& col);
            void     SetExpandedBorderColour (const wxColour& col);
            void     SetExpandedTextColour   (const wxColour& col);
            void     SetExpandedFont         (const wxFont& f);
        ///@}


        ///@{ \name Caption bar collapsed highlighted state style get/set functions
            wxGraphicsGradientStops GetCollapsedHLGradientStops() const;
            wxBitmap GetCollapsedHLBGBitmap()     const;
            wxColour GetCollapsedHLColour1()      const;
            wxColour GetCollapsedHLColour2()      const;
            wxColour GetCollapsedHLBorderColour() const;
            wxColour GetCollapsedHLTextColour()   const;
            wxFont   GetCollapsedHLFont()         const;

            void     ClearCollapsedHLGradientStops();
            void     AddCollapsedHLGradientStop (const wxColour& col,float pos);
            void     SetCollapsedHLBGBitmap     (const wxBitmap& b);
            void     SetCollapsedHLColour1      (const wxColour& col);
            void     SetCollapsedHLColour2      (const wxColour& col);
            void     SetCollapsedHLBorderColour (const wxColour& col);
            void     SetCollapsedHLTextColour   (const wxColour& col);
            void     SetCollapsedHLFont         (const wxFont& f);
        ///@}


        ///@{ \name Caption bar expanded highlighted state style get/set functions
            wxGraphicsGradientStops GetExpandedHLGradientStops() const;
            wxBitmap GetExpandedHLBGBitmap()      const;
            wxColour GetExpandedHLColour1()       const;
            wxColour GetExpandedHLColour2()       const;
            wxColour GetExpandedHLBorderColour()  const;
            wxColour GetExpandedHLTextColour()    const;
            wxFont   GetExpandedHLFont()          const;

            void ClearExpandedHLGradientStops();
            void   AddExpandedHLGradientStop (const wxColour& col,float pos);
            void   SetExpandedHLBGBitmap     (const wxBitmap& b);
            void   SetExpandedHLColour1      (const wxColour& col);
            void   SetExpandedHLColour2      (const wxColour& col);
            void   SetExpandedHLBorderColour (const wxColour& col);
            void   SetExpandedHLTextColour   (const wxColour& col);
            void   SetExpandedHLFont         (const wxFont& f);
        ///@}


        ///@{ \name Caption bar disabled state style get/set functions
            wxGraphicsGradientStops GetDisabledGradientStops() const;
            wxBitmap GetDisabledBGBitmap()        const;
            wxColour GetDisabledColour1()         const;
            wxColour GetDisabledColour2()         const;
            wxColour GetDisabledBorderColour()    const;
            wxColour GetDisabledTextColour()      const;
            wxFont   GetDisabledFont()            const;

            void ClearDisabledGradientStops();
            void   AddDisabledGradientStop (const wxColour& col,float pos);
            void   SetDisabledBGBitmap     (const wxBitmap& b);
            void   SetDisabledColour1      (const wxColour& col);
            void   SetDisabledColour2      (const wxColour& col);
            void   SetDisabledBorderColour (const wxColour& col);
            void   SetDisabledTextColour   (const wxColour& col);
            void   SetDisabledFont         (const wxFont& f);
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
        void buildBitmap(wxBitmap& out, bool bottomBorder, int drawtype, int width,int height, double x1,double y1,double x2,double y2, double radius, const wxColour& collapsedBorderColour,const wxBitmap& collapsedBG,const wxGraphicsGradientStops& collapsedStops);
        void adjustForStyle( long style );

        //private data
        //helper data:
        bool changesMade;
        wxImageList* trackedImageList;
        wxPanel* lastBorder;

        //computed:
        int captionBarHeight;
        wxRect buttonRect;
        wxRect iconRect;
        wxPoint collapsedTextPoint;
        wxPoint expandedTextPoint;
        wxPoint collapsedHLTextPoint;
        wxPoint expandedHLTextPoint;
        wxPoint disabledTextPoint;
        int textWidth;
        wxBitmap captionBarCollapsedHL;
        wxBitmap captionBarExpandedHL;
        wxBitmap captionBarCollapsed;
        wxBitmap captionBarExpanded;
        wxBitmap captionBarDisabled;

        //style items:
        int accordionPadding;
        bool onlyToggleWithButton;

        double captionBarRadius;
        double gradientAngle;
        bool hasBottomBorderWhenExpanded;
        int minCaptionHeight;
        wxSize iconMargin;
        wxSize textMargin;
        wxSize buttonMargin;
        wxBitmap collapseButton;
        wxBitmap expandButton;
        bool customDisabledState;
        bool useHighlighting;
        int  disabledBrightness;

        int pagePadding;
        double pageRadius;

        wxBitmap accordionBG;
        wxBitmap collapsedBG;
        wxBitmap expandedBG;
        wxBitmap collapsedHLBG;
        wxBitmap expandedHLBG;
        wxBitmap disabledBG;
        wxBitmap pageBG;

        wxGraphicsGradientStops expandedStops;
        wxColour expandedBorderColour;
        wxColour expandedTextColour;
        wxFont expandedFont;

        wxGraphicsGradientStops collapsedStops;
        wxColour collapsedBorderColour;
        wxColour collapsedTextColour;
        wxFont collapsedFont;

        wxGraphicsGradientStops expandedHLStops;
        wxColour expandedHLBorderColour;
        wxColour expandedHLTextColour;
        wxFont expandedHLFont;

        wxGraphicsGradientStops collapsedHLStops;
        wxColour collapsedHLBorderColour;
        wxColour collapsedHLTextColour;
        wxFont collapsedHLFont;

        wxGraphicsGradientStops disabledStops;
        wxColour disabledBorderColour;
        wxColour disabledTextColour;
        wxFont disabledFont;

        wxGraphicsGradientStops pageStops;
        wxColour pageBorderColour;
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

#endif // WXACCORDION_H_INCLUDED
