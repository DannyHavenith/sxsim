#if !defined( SXGO_LISTING_WINDOW_HPP)
#define SXGO_LISTING_WINDOW_HPP

#include "wx/wxhtml.h"

class sxgo_listing_window : public wxHtmlWindow
{
public:
    explicit sxgo_listing_window(
				wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxHW_DEFAULT_STYLE,
                 const wxString& name = wxT("htmlWindow"))
		:wxHtmlWindow( parent, id, pos, size, style, name)
	{
	}
	
};

#endif