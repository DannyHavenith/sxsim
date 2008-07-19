#include "wx/docview.h"
class	sxgo_listing_window;

class	wxMDIChildFrame;
class	wxDocument;
class	wxDC;
class	wxObject;
class sxgo_view: public wxView
{
public:
	wxMDIChildFrame *frame;
	sxgo_listing_window *textsw;

	sxgo_view(): wxView(), frame(0), textsw(0) { }

	bool OnCreate(wxDocument *doc, long flags);
	void OnDraw(wxDC *dc){};
	void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL){};
	bool OnClose(bool deleteWindow = true);

private:
	DECLARE_DYNAMIC_CLASS(sxgo_view)
};

