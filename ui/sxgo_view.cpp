
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <boost/timer.hpp>
#include "wx/docview.h"
#include "wx/docmdi.h"
#include "wx/grid.h"

#include "sxgo_app.hpp"
#include "sxgo_view.hpp"
#include "sxgo_doc.hpp"
#include "sxgo_frame.hpp"
#include "sxgo_listing_window.hpp"
#include "sxgo_label_window.hpp"
#include "sxgo_ram_window.hpp"
#include "wx/statusbr.h"

IMPLEMENT_DYNAMIC_CLASS(sxgo_view, wxView)

BEGIN_EVENT_TABLE(sxgo_view, wxView)
	EVT_IDLE(sxgo_view::OnIdle)
	EVT_MENU(ID_SingleStep, sxgo_view::SingleStep)
	EVT_MENU(ID_Run, sxgo_view::Run)
	EVT_MENU(ID_Pause, sxgo_view::Pause)
	EVT_GRID_CELL_LEFT_DCLICK( sxgo_view::DoubleClick)
	EVT_COMMAND(wxID_ANY, EVT_JUMPTO_LABEL_LINE, sxgo_view::ShowLine)
	EVT_COMMAND(wxID_ANY, EVT_CHANGE_RAMVALUE, sxgo_view::ChangeRam)
END_EVENT_TABLE()

//
// someone doubleclicked on a label in the
// label window.
//
void sxgo_view::ShowLine(wxCommandEvent & event)
{
	int line = event.GetInt();
	textsw->ShowLine( line);
}

//
// change a ram value
//
void sxgo_view::ChangeRam( wxCommandEvent &event)
{
	// still a bit cumbersome: retrieve the state, change one value
	// and then push the altered state back.
	//
	sx_simulator::state state = SafeGetDocument()->GetState();
	state.ram.set_absolute( event.GetInt(), event.GetExtraLong());
	SafeGetDocument()->SetState( state);
	MyFrame::GetMainFrame()->UpdateAll( *SafeGetDocument());
}

void sxgo_view::DoubleClick( wxGridEvent &event)
{
	if (event.GetCol() == 0)
	{
		int line = event.GetRow();
		unsigned short address = SafeGetDocument()->GetListing().GetNearestAddress( line);
		line = SafeGetDocument()->GetListing().GetLine( address);
		bool set = textsw->ToggleBreakpoint( line);
		SafeGetDocument()->SetBreakpoint( address, set);
	}
}

bool sxgo_view::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
	frame = wxGetApp().CreateChildFrame(doc, this);


	int width, height;
	frame->GetClientSize(&width, &height);
	textsw = new sxgo_listing_window( frame, wxID_ANY);//, wxPoint(0,0), wxSize( width, height));

	frame->SetTitle(doc->GetTitle());

#ifdef __X__
	// X seems to require a forced resize
	int x, y;
	frame->GetSize(&x, &y);
	frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif

	frame->Show(true);
	Activate(true);
	return true;
}

bool sxgo_view::OnClose(bool deleteWindow)
{
	if (!GetDocument()->Close())
		return false;

	Activate(false);

	if (deleteWindow)
	{
		delete frame;
		return true;
	}
	return true;
}

void sxgo_view::SingleStep(wxCommandEvent& WXUNUSED(event))
{
	sxgo_document *doc = (sxgo_document *)GetDocument();
	doc->SingleStep();
	unsigned short address = doc->GetState().pc;
	int line = doc->GetListing().GetLine(address);
	textsw->JumpToLine( line);

	running = false;

	MyFrame::GetMainFrame()->UpdateAll( *doc);
}

void sxgo_view::Pause(wxCommandEvent& WXUNUSED(event))
{
	running = false;
}

//
// running the emulator happens during idle time
// to allow UI updates while running.
// this function starts idle time processing
//
void sxgo_view::Run(wxCommandEvent& event)
{
    MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText(_("Running"));
    running = true;
	wxWakeUpIdle();
	RunSome();
}

//
// Run for a fraction of a second and request
// more idle events.
//
void sxgo_view::OnIdle(wxIdleEvent& event)
{
	if (running)
	{
		RunSome();
		event.RequestMore();
	}
}

//
// run for a fraction of a second.
//
void sxgo_view::RunSome()
{
	sxgo_document *doc = (sxgo_document *)GetDocument();
	// 1000 000 clockticks should be short enough
	unsigned long run_count = 10000000;

	boost::timer t;
	if (doc->Run( run_count) != 0)
	{
	    MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText(_("Ready"));
		running = false;
	}
	else
	{
	    std::stringstream s;
	    unsigned long ips = ((double)run_count)/t.elapsed();
	    s << "Running, " << ips << " instructions per second";
	    MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText( wxString( s.str()));
	}

	unsigned short address = doc->GetState().pc;
	int line = doc->GetListing().GetLine(address);
	textsw->JumpToLine( line);
	MyFrame::GetMainFrame()->UpdateAll( *doc);
}

void sxgo_view::OnUpdate(wxView *sender, wxObject *hint)
{
	sxgo_document *doc = (sxgo_document *)GetDocument();
	textsw->SetListing( doc->GetListing());
	textsw->SetFont( wxFont( 9, wxFONTFAMILY_MODERN,
		wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ));
}
