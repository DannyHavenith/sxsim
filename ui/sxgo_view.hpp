
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "wx/docview.h"
#include "sxgo_event_definitions.hpp"

class	sxgo_listing_window;

class	wxMDIChildFrame;
class	wxDocument;
class	wxDC;
class	wxObject;
class wxGridEvent;
class wxTreeEvent;
class sxgo_document;

class sxgo_view: public wxView, public sxgo_event_definitions
{
public:
	wxMDIChildFrame *frame;

	sxgo_view(): wxView(), frame(0), textsw(0), running(false), reset_on_stop(false) { }

	bool OnCreate(wxDocument *doc, long flags);
	void OnDraw(wxDC *dc){};
	void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
	bool OnClose(bool deleteWindow = true);
	void SingleStep(wxCommandEvent& event);
	void Run(wxCommandEvent& event);
	void Pause(wxCommandEvent& event);
	void Stop(wxCommandEvent& event);
	void DoubleClick( wxGridEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnSetFocus( wxFocusEvent &event);
	void LabelActivated(wxTreeEvent& event);
	void ShowLine(wxCommandEvent & event);
	void ChangeRam( wxCommandEvent &event);
private:

	sxgo_document *SafeGetDocument() const
	{
		return (sxgo_document *)GetDocument();
	}

	void RunSome( bool first_run);
	void UpdateAll();

	bool running;
	bool reset_on_stop;
	sxgo_listing_window *textsw;

	DECLARE_DYNAMIC_CLASS(sxgo_view)
    DECLARE_EVENT_TABLE()
};

