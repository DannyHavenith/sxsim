#if !defined( SXGO_VARIABLES_WINDOWS_HPP)
#define SXGO_VARIABLES_WINDOWS_HPP
#include "sx_simulator.hpp"
#include "wx/grid.h"

class sxgo_document;
class sxgo_variables_window : public wxGrid
{
public:
	    explicit sxgo_variables_window(
				wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxWANTS_CHARS,
                 const wxString& name = wxT("Listing"));
		

		void Update( const sxgo_document &doc);

};

#endif //SXGO_VARIABLES_WINDOWS_HPP