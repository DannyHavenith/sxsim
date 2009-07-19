
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "wx/docview.h"
#include "wx/docmdi.h"
#include "wx/menu.h"
#include "wx/config.h"
#include "wx/cmdline.h"

#include "sxgo_frame.hpp"
#include "sxgo_app.hpp"
#include "sxgo_doc.hpp"
#include "sxgo_view.hpp"

#include "sxgoui.xpm"
IMPLEMENT_APP(MyApp)

namespace {
    static const wxCmdLineEntryDesc cmdLineDesc[] =
    {
        { wxCMD_LINE_SWITCH, _T("h"), _T("help"), _T("show this help message"),
            wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
        { wxCMD_LINE_SWITCH, _T("v"), _T("verbose"), _T("be verbose") },
        { wxCMD_LINE_PARAM,  NULL, NULL, _T("input file"),
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_MULTIPLE |wxCMD_LINE_PARAM_OPTIONAL },
        { wxCMD_LINE_NONE }
    };

}
/*
* Centralised code for creating a document frame.
* Called from view.cpp, when a view is created.
*/

wxMDIChildFrame *MyApp::CreateChildFrame(wxDocument *doc, wxView *view)
{
	//// Make a child frame

	wxDocMDIChildFrame *subframe =
		new wxDocMDIChildFrame(doc, view, frame, wxID_ANY, _T("Child Frame"),
		wxPoint(0,0), wxSize(300, 300),
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
	wxConfig *config = new wxConfig( _("sxgo"));
	int x = config->Read( _("InitialWindow/x"), (long)0);
	int y = config->Read( _("InitialWindow/y"), (long)0);
	int w = config->Read( _("InitialWindow/w"), 800);
	int h = config->Read( _("InitialWindows/h"), 600);

	frame = new MyFrame((wxDocManager *) m_docManager, (wxFrame *) NULL,
		_T("sxgo!"), wxPoint(x, y), wxSize(w, h),
		wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE);

	SetTopWindow(frame);
	frame->Show();
	return wxApp::OnInit();
}


void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc (cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("-"));
}


bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{

	if (parser.GetParamCount() >= 1)
	{
		m_docManager->CreateDocument( parser.GetParam(), wxDOC_SILENT);
	}
    return true;
}

