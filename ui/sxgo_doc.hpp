
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// wxWindows Document class for this project

#if !defined (SXGO_DOC_HPP)
#define SXGO_DOC_HPP
#include <vector>
#include <utility> // for std::pair
#include "wx/docview.h"
#include "listing_parser.hpp"
#include "sx_simulator.hpp"
#include <boost/scoped_ptr.hpp>

class wxString;

class sxgo_document: public wxDocument
{
public:
	typedef std::vector< std::pair<unsigned short, unsigned long> > profile_type;
	DECLARE_DYNAMIC_CLASS(sxgo_document)

    sxgo_document();

	virtual bool OnSaveDocument(const wxString& filename);

	// implement for both wxInputStream and std::istream to deal with
	// different wxWidgets versions
    virtual wxInputStream& LoadObject(wxInputStream& stream);
    virtual std::istream& LoadObject(std::istream& stream);

	listing_info GetListing() const;
	unsigned short SingleStep();
	unsigned long Run( unsigned long count);
	profile_type GetProfile() const;
	void SetBreakpoint( unsigned short address, bool do_set = true);
	sx_simulator::state GetState() const;
	void SetState( const sx_simulator::state &new_state);
	void Reset();

	bool IsLoaded() const 
	{
		return simulator_ptr;
	}

    static wxMenuBar *CreateDocMenuBar();

    /// return some pseudo-unique id. This is to recognize different documents
    /// which have been created on different moments (but which could be at the same address).
    int GetId() const
    {
        return id;
    }

private:
    const int id;
	listing_info listing;
	boost::scoped_ptr< sx_simulator> simulator_ptr;
    DECLARE_EVENT_TABLE()
};

#endif //SXGO_DOC_HPP
