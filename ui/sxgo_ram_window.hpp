#if !defined( SXGO_RAM_WINDOW_HPP)
#define SXGO_RAM_WINDOW_HPP
#include "sx_simulator.hpp"
#include "wx/grid.h"

DECLARE_EVENT_TYPE(EVT_CHANGE_RAMVALUE, -1)

class sxgo_ram_window : public wxGrid
{
public:
	    explicit sxgo_ram_window(
				wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxWANTS_CHARS,
                 const wxString& name = wxT("Listing"));
		

		void Update( const sx_simulator::state &state);
		void ChangeValue( wxGridEvent &event);
		DECLARE_EVENT_TABLE()
};

#endif //SXGO_RAM_WINDOW_HPP