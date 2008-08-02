#if !defined (SXGO_DOC_HPP)
#define SXGO_DOC_HPP

#include "wx/docview.h"
#include "listing_parser.hpp"
#include "sx_simulator.hpp"
#include <boost/scoped_ptr.hpp>

class wxString;

class sxgo_document: public wxDocument
{
public:
	DECLARE_DYNAMIC_CLASS(sxgo_document)

	virtual bool OnSaveDocument(const wxString& filename);
	virtual wxInputStream& LoadObject(wxInputStream& stream);
	listing_info GetListing() const;
	unsigned short SingleStep();
	unsigned long Run( unsigned long count);
	void SetBreakpoint( unsigned short address, bool do_set = true);
	sx_simulator::state GetState() const;

private:
	listing_info listing;
	boost::scoped_ptr< sx_simulator> simulator_ptr;
};

#endif //SXGO_DOC_HPP