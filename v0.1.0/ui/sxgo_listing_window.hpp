
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined( SXGO_LISTING_WINDOW_HPP)
#define SXGO_LISTING_WINDOW_HPP

#include "wx/grid.h"
#include "listing_parser.hpp"

class sxgo_listing_window : public wxGrid
{
public:
    explicit sxgo_listing_window(
				wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxWANTS_CHARS,
                 const wxString& name = wxT("Listing"))
		:wxGrid( parent, id, pos, wxSize( 10*20, 17 * 16), style, name)
	{
		SetColLabelSize( 0);
		SetRowLabelSize(0);
		EnableGridLines( false);
	}

	void ShowLine( unsigned short line);

	void JumpToLine( unsigned short address);
	void SetListing( const listing_info &listing);
	bool ToggleBreakpoint( int line);
private:
	listing_info listing;
	int current_line;
};

#endif