
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <sstream> // for istringstream
#include "sxgo_doc.hpp"
#include "sxgo_view.hpp"
#include "sx_simulator.hpp"

IMPLEMENT_DYNAMIC_CLASS(sxgo_document, wxDocument)

using namespace std;

bool sxgo_document::OnSaveDocument(const wxString& filename)
{
	return true;
}

wxInputStream& sxgo_document::LoadObject(wxInputStream& stream)
{
    static const size_t buffer_size = 4096;
    char buffer[buffer_size];
    string contents;
    while (!stream.Read( buffer, buffer_size).Eof())
    {
        contents.append( buffer, stream.LastRead());
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