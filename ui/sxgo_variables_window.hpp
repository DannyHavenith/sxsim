
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined( SXGO_VARIABLES_WINDOWS_HPP)
#define SXGO_VARIABLES_WINDOWS_HPP
#include "sx_simulator.hpp"
#include "wx/grid.h"

class sxgo_document;
struct sx_state;

class sxgo_variables_window : public wxGrid
{
public:
	    explicit sxgo_variables_window(
				wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxWANTS_CHARS,
                 const wxString& name = wxT("Listing"));

	    void AddRegisterValue( int cell_index, const wxString &label, int ram_location, const sx_state &s);

		void Update( const sxgo_document &doc);

};

#endif //SXGO_VARIABLES_WINDOWS_HPP
