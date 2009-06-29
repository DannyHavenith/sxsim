
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "wx/docview.h"
#include "wx/docmdi.h"
#include "wx/menu.h"

#include "sxgo_frame.hpp"
#include "sxgo_app.hpp"
#include "sxgo_doc.hpp"
#include "sxgo_view.hpp"

#include "sxgoui.xpm"

IMPLEMENT_APP(MyApp)

/*
* Centralised code for creating a document frame.
* Called from view.cpp, when a view is created.
*/

wxMDIChildFrame *MyApp::CreateChildFrame(wxDocument *doc, wxView *view)
{
	//// Make a child frame
	wxDocMDIChildFrame *subframe =
		new wxDocMDIChildFrame(doc, view, frame, wxID_ANY, _T("Child Frame"),
		wxPoint(10, 10), wxSize(300, 300),
		wxDEFAULT_FRAME_STYLE |
		wxNO_FULL_REPAINT_ON_RESIZE);

#ifdef __WXMSW__
	subframe->SetIcon(wxIcon( sample_xpm));
#endif
#ifdef __X__
	subframe->SetIcon(wxIcon(_T("doc.xbm")));
#endif

	//// Make a menubar
	wxMenu *file_menu = new wxMenu;

	file_menu->Append(wxID_NEW, _T("&New..."));
	file_menu->Append(wxID_OPEN, _T("&Open..."));
	file_menu->Append(wxID_CLOSE, _T("&Close"));
	file_menu->Append(wxID_SAVE, _T("&Save"));
	file_menu->Append(wxID_SAVEAS, _T("Save &As..."));


	file_menu->AppendSeparator();
	file_menu->Append(wxID_EXIT, _T("E&xit"));

	wxMenuBar *menu_bar = new wxMenuBar;

	menu_bar->Append(file_menu, _T("&File"));

	//// Associate the menu bar with the frame
	subframe->SetMenuBar(menu_bar);

	return subframe;
}


bool MyApp::OnInit()
{
	//// Create a document manager
	m_docManager = new wxDocManager;
	//// Create a template relating drawing documents to their views
	(void) new wxDocTemplate((wxDocManager *) m_docManager, _T("sx listing"), _T("*.lst"), _T(""), _T("lst"), _T("Drawing Doc"), _T("Drawing View"),
		CLASSINFO(sxgo_document), CLASSINFO(sxgo_view));

	frame = new MyFrame((wxDocManager *) m_docManager, (wxFrame *) NULL,
		_T("sxgo!"), wxPoint(0, 0), wxSize(500, 400),
		wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE);

	SetTopWindow(frame);
	frame->Show();
	return true;
}


