
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>
#include "wx/docview.h"
#include "wx/docmdi.h"
#include "wx/grid.h"
#include "wx/msgdlg.h"

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
	EVT_MENU(ID_Stop, sxgo_view::Stop)
	EVT_GRID_CELL_LEFT_DCLICK( sxgo_view::DoubleClick)
	EVT_SET_FOCUS(sxgo_view::OnSetFocus)
	EVT_COMMAND(wxID_ANY, EVT_JUMPTO_LABEL_LINE, sxgo_view::ShowLine)
	EVT_COMMAND(wxID_ANY, EVT_CHANGE_RAMVALUE, sxgo_view::ChangeRam)
END_EVENT_TABLE()

namespace
{
	wxString SimplifyTitle( const wxString &filename)
	{
		using namespace boost::filesystem;
		path p( (const char *)filename);

		return wxString( p.stem().c_str());
	}
}
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
/// if we receive focus, after somebody else had focus, we're going to update all view windows.
///
void sxgo_view::OnSetFocus( wxFocusEvent &event)
{
	UpdateAll();
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

/// Someone doubleclicked on a row in the listing, set or reset a breakpoint.
void sxgo_view::DoubleClick( wxGridEvent &event)
{
	int line = event.GetRow();
	unsigned short address = SafeGetDocument()->GetListing().GetNearestAddress( line);
	line = SafeGetDocument()->GetListing().GetLine( address);
	bool set = textsw->ToggleBreakpoint( line);
	SafeGetDocument()->SetBreakpoint( address, set);
}

bool sxgo_view::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
	frame = wxGetApp().CreateChildFrame(doc, this);


	int width, height;
	frame->GetClientSize(&width, &height);
	textsw = new sxgo_listing_window( frame, wxID_ANY, wxPoint(0,0), wxSize( width, height));
    textsw->SetFont( wxFont( 9, wxFONTFAMILY_MODERN,
		wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ));

	int x, y;
	textsw->SetSize( wxDefaultCoord, wxDefaultCoord, width, height);
	frame->Maximize();
	frame->Show(true);
	frame->GetSize(&x, &y);
	frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
	Activate(true);
	MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText(_("Reset, press Run or Singlestep button"));
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

/// perform one instruction.
void sxgo_view::SingleStep(wxCommandEvent& WXUNUSED(event))
{
	sxgo_document *doc = (sxgo_document *)GetDocument();
	doc->SingleStep();
	unsigned short address = doc->GetState().pc;
	int line = doc->GetListing().GetLine(address);
	textsw->JumpToLine( line);

	running = false;

	MyFrame::GetMainFrame()->UpdateAll( *doc);
	MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText(_("Paused"));
}

/// pause execution of the program.
void sxgo_view::Pause(wxCommandEvent& WXUNUSED(event))
{
	running = false;
	MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText(_("Paused"));

}

/// stop executing (if running) and fully reset the emulator
void sxgo_view::Stop(wxCommandEvent& WXUNUSED(event))
{
	if (running)
	{
		running = false;
		reset_on_stop = true;
	}
	else
	{
		SafeGetDocument()->Reset();
		UpdateAll();
		MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText(_("Reset"));
	}

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
	RunSome( true);
}

//
/// Run for a fraction of a second and request
/// more idle events.
void sxgo_view::OnIdle(wxIdleEvent& event)
{
	if (running)
	{
		RunSome( false);
		event.RequestMore();
	}
	else
	{
		event.RequestMore(false);
		if (reset_on_stop)
		{
			reset_on_stop = false;
			SafeGetDocument()->Reset();
			MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText(_("Reset"));
			UpdateAll();
		}
		textsw->ClearProfile();
	}
}

// update all views on this document.
void sxgo_view::UpdateAll()
{
	sxgo_document *doc = SafeGetDocument();
	if (doc->IsLoaded())
	{
		unsigned short address = doc->GetState().pc;
		int line = doc->GetListing().GetLine(address);
		textsw->JumpToLine( line);
		MyFrame::GetMainFrame()->UpdateAll( *doc);
	}
}

///
/// run for a fraction of a second.
/// This method is typically called from OnIdle.
void sxgo_view::RunSome( bool first_run)
{
	sxgo_document *doc = SafeGetDocument();
	// 1000 000 clockticks should be short enough
	// choose a prime number close to a million, to avoid
	// the screen update coming in sync with some loop in
	// the program.
	unsigned long run_count = 1005989;
	static int profile_refresh_counter = 0;
	static const int profile_refresh_treshold = 5;

	try
	{
	if (doc->Run( run_count) != 0)
	{
		// we hit a breakpoint
		unsigned short address = doc->GetState().pc;

        std::stringstream strm;
        strm << "Breakpoint hit at address " << std::hex << address << 'H';
	    MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText( wxString( strm.str().c_str(), wxConvUTF8));

        running = false;

        // jump to the active line and reset the profiling shades.
		int line = doc->GetListing().GetLine(address);
		textsw->ClearProfile();
		textsw->JumpToLine( line);
	}
	else
	{
	    MyFrame::GetMainFrame()->GetStatusBar()->SetStatusText( _( "Running"));
	    if (first_run || ++profile_refresh_counter > profile_refresh_treshold)
	    {
            // show profiling shades (that show how often a certain line was touched in the past run)
			sxgo_document::profile_type profile = doc->GetProfile();
			textsw->ShowProfile( profile, first_run);
			profile_refresh_counter = 0;
	    }
	}
	}
	catch (std::exception &e)
	{
		::wxMessageBox( e.what(), "SX emulator error");
	}
	MyFrame::GetMainFrame()->UpdateAll( *doc);
}

/// on update, set the frame title and the font
void sxgo_view::OnUpdate(wxView *sender, wxObject *hint)
{
	sxgo_document *doc = (sxgo_document *)GetDocument();
	frame->SetTitle(SimplifyTitle( doc->GetFilename()));
	textsw->SetListing( doc->GetListing());
	textsw->SetFont( wxFont( 9, wxFONTFAMILY_MODERN,
		wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ));
	UpdateAll();
}
