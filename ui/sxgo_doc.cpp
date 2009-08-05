
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <sstream> // for istringstream

#include "wx/menuitem.h"
#include "wx/menu.h"

#include "sxgo_doc.hpp"
#include "sxgo_view.hpp"
#include "sxgo_frame.hpp"
#include "sx_simulator.hpp"
#include "sxgo_event_definitions.hpp"

IMPLEMENT_DYNAMIC_CLASS(sxgo_document, wxDocument)

BEGIN_EVENT_TABLE(sxgo_document, wxDocument)
//	EVT_MENU(ID_Stop, sxgo_view::Stop)
//	EVT_COMMAND(wxID_ANY, EVT_CHANGE_RAMVALUE, sxgo_view::ChangeRam)
END_EVENT_TABLE()

using namespace std;


bool sxgo_document::OnSaveDocument(const wxString& filename)
{
	return true;
}

/// Load a listing file, parse it and store the program bytes in the emulators ROM
wxInputStream& sxgo_document::LoadObject(wxInputStream& stream)
{
    static const size_t buffer_size = 4096;
    char buffer[buffer_size];
    string contents;
	stream.Read( buffer, buffer_size);
    while (stream.LastRead())
    {
        contents.append( buffer, stream.LastRead());
		stream.Read( buffer, buffer_size);
    }

    std::istringstream strm( contents);
    listing = ParseListingFile( strm);
    simulator_ptr.reset( new sx_simulator());
    simulator_ptr->load_rom( listing.instructions);
    return stream;
}

std::istream& sxgo_document::LoadObject(std::istream & stream)
{
    listing = ParseListingFile( stream);
    simulator_ptr.reset( new sx_simulator());
    simulator_ptr->load_rom( listing.instructions);
	simulator_ptr->reset_histogram();
    return stream;
}

listing_info sxgo_document::GetListing() const
{
	return listing;
}

unsigned short sxgo_document::SingleStep()
{
	return simulator_ptr->run( 1);
}

unsigned long sxgo_document::Run( unsigned long count)
{
	return simulator_ptr->run( count);
}

namespace
{
	bool second_is_smaller(
		const std::pair< unsigned short, unsigned long> &lhs,
		const std::pair< unsigned short, unsigned long> &rhs)
	{
		return lhs.second < rhs.second;
	}
}
/// Returns a list of linenumbers and the number of times the line was hit.
sxgo_document::profile_type sxgo_document::GetProfile() const
{
	profile_type profile;
	const sx_simulator::histogram_type &histogram =
		simulator_ptr->get_histogram();

	for( int address = 0; address != sx_simulator::rom_size; ++address)
	{
		if (histogram[address])
		{
			profile.push_back( std::make_pair( listing.GetLine( address), histogram[address]));
		}
	}

	std::sort( profile.begin(), profile.end(), &second_is_smaller);
	simulator_ptr->reset_histogram();
	return profile;
}

void sxgo_document::SetBreakpoint( unsigned short address, bool do_set)
{
	simulator_ptr->set_breakpoint( address, do_set);
}

sx_simulator::state sxgo_document::GetState() const
{
	return simulator_ptr->get_state();
};

void sxgo_document::SetState( const sx_simulator::state &new_state)
{
	return simulator_ptr->set_state( new_state);
};

void sxgo_document::Reset()
{
	simulator_ptr->reset();
}

wxMenuBar *sxgo_document::CreateDocMenuBar()
{
    wxMenuBar *menu_bar = MyFrame::CreateFrameMenuBar();

    wxMenu *file = new wxMenu();
    file->Append(sxgo_event_definitions::ID_Load_State, _T("Load &state"));
    file->Append(sxgo_event_definitions::ID_Save_State, _T("Save s&tate"));

    //menu_bar->Append( file, _T("&State"));

    wxMenu *run_menu = new wxMenu;
    run_menu->Append( sxgo_event_definitions::ID_Stop, wxT("Stop\tS"));
    run_menu->Append( sxgo_event_definitions::ID_Pause, wxT("Pause\tP"));
    run_menu->Append( sxgo_event_definitions::ID_SingleStep, wxT("Single step\tT"));
    run_menu->Append( sxgo_event_definitions::ID_Run, wxT("Run\tR"));

    menu_bar->Insert(1, run_menu, _T("&Run"));

    return menu_bar;
}
