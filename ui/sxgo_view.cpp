#include "wx/docview.h"
#include "wx/docmdi.h"
#include "wx/grid.h"

#include "sxgo_app.hpp"
#include "sxgo_view.hpp"
#include "sxgo_doc.hpp"
#include "sxgo_frame.hpp"
#include "sxgo_listing_window.hpp"

IMPLEMENT_DYNAMIC_CLASS(sxgo_view, wxView)

BEGIN_EVENT_TABLE(sxgo_view, wxView)
	EVT_MENU(ID_SingleStep, sxgo_view::SingleStep)
	EVT_MENU(ID_Run, sxgo_view::Run)
	EVT_GRID_CELL_LEFT_DCLICK( sxgo_view::DoubleClick)
END_EVENT_TABLE()

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
	textsw = new sxgo_listing_window( frame, wxID_ANY, wxPoint(0,0), wxSize( width, height));
	
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
	unsigned short address = doc->SingleStep();
	textsw->JumpToAddress( address);
	MyFrame::GetMainFrame()->UpdateAll( doc->GetState());
}

void sxgo_view::Run(wxCommandEvent& WXUNUSED(event))
{
	sxgo_document *doc = (sxgo_document *)GetDocument();

	// run one milion cycles
	unsigned short address = doc->Run( 1000000);

	textsw->JumpToAddress( address);
	MyFrame::GetMainFrame()->UpdateAll( doc->GetState());
}

void sxgo_view::OnUpdate(wxView *sender, wxObject *hint)
{
	sxgo_document *doc = (sxgo_document *)GetDocument();
	textsw->SetListing( doc->GetListing());
}
