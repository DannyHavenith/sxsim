#include "wx/docview.h"
#include "sxgo_event_definitions.hpp"

class	sxgo_listing_window;

class	wxMDIChildFrame;
class	wxDocument;
class	wxDC;
class	wxObject;
class wxGridEvent;
class sxgo_document;

class sxgo_view: public wxView, public sxgo_event_definitions
{
public:
	wxMDIChildFrame *frame;

	sxgo_view(): wxView(), frame(0), textsw(0) { }

	bool OnCreate(wxDocument *doc, long flags);
	void OnDraw(wxDC *dc){};
	void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
	bool OnClose(bool deleteWindow = true);
	void SingleStep(wxCommandEvent& event);
	void Run(wxCommandEvent& event);
	void DoubleClick( wxGridEvent& event);
	sxgo_document *SafeGetDocument() const
	{
		return (sxgo_document *)GetDocument();
	}
private:
	sxgo_listing_window *textsw;
	DECLARE_DYNAMIC_CLASS(sxgo_view)
    DECLARE_EVENT_TABLE()
};

