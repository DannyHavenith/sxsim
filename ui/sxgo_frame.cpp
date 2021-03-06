
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// based on auidemo.cpp by Benjamin I. Williams

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"
#include "wx/aui/aui.h"
#include "wx/docview.h"
#include "wx/docmdi.h"
#include "wx/log.h"

#include "sxgoui.xpm"
#include "play.xpm"
#include "stop.xpm"
#include "step.xpm"
#include "pause.xpm"

#include "sxgo_frame.hpp"
#include "sxgo_app.hpp"
#include "sxgo_listing_window.hpp"
#include "sxgo_ram_window.hpp"
#include "sxgo_variables_window.hpp"
#include "sxgo_label_window.hpp"
#include "sxgo_doc.hpp"


MyFrame *MyFrame::main_frame = 0;

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
//EVT_ERASE_BACKGROUND(MyFrame::OnEraseBackground)
//EVT_SIZE(MyFrame::OnSize)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_MENU_RANGE(MyFrame::ID_FirstPerspective, MyFrame::ID_FirstPerspective+1000,
			   MyFrame::OnRestorePerspective)
END_EVENT_TABLE()



MyFrame::MyFrame(wxDocManager *manager, wxFrame *frame, const wxString& title,
const wxPoint& pos, const wxSize& size, long type):
wxDocMDIParentFrame(manager, frame, wxID_ANY, title, pos, size, type, _T("myFrame"))
{
	main_frame = this;
	// tell wxAuiManager to manage this frame
	m_mgr.SetManagedWindow(this);

	// set frame icon
	SetIcon(wxIcon(sample_xpm));

	// create menu
	SetMenuBar( CreateFrameMenuBar());

	CreateStatusBar();
	GetStatusBar()->SetStatusText(_("Ready"));

	// min size for the frame itself isn't completely done.
	// see the end up wxAuiManager::Update() for the test
	// code. For now, just hard code a frame minimum size
	SetMinSize(wxSize(800,600));

	// create the run-toolbar
	wxToolBar* controls_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxTB_FLAT | wxTB_NODIVIDER);
	controls_toolbar->SetToolBitmapSize(wxSize(16,16));

	controls_toolbar->AddTool(ID_Stop, wxT("stop"), wxBitmap( stop_xpm), wxT("Stop and reset the emulator"));
	controls_toolbar->AddTool(ID_Pause, wxT("pause"), wxBitmap( pause_xpm), wxT("Pause"));
	controls_toolbar->AddTool(ID_SingleStep, wxT("step"), wxBitmap( step_xpm), wxT("Single step"));
	controls_toolbar->AddTool(ID_Run, wxT("play"), wxBitmap( play_xpm), wxT( "Run"));
	controls_toolbar->Realize();

	m_mgr.AddPane(controls_toolbar, wxAuiPaneInfo().
		Name(wxT("controls_toolbar")).Caption(wxT("Control Toolbar")).
		ToolbarPane().Top().Row(1).
		LeftDockable(true).RightDockable(true));

	// prevent the toolbar to intervene with status bar texts
	// the toolbar would remember the texts in the status bar on mouse enter
	// and then reset those texts on mouse leave, ignoring the fact that
	// I would have changed the toolbar text in between (in reaction to a tool click).
	SetStatusBarPane( -1);

	m_ram_window = new sxgo_ram_window(this);
    m_mgr.AddPane(	m_ram_window,
					wxAuiPaneInfo().Name(wxT("ram window"))
					.MinSize( m_ram_window->GetMinSize())
				);

	m_variables_window = new sxgo_variables_window(this);
    m_mgr.AddPane(	m_variables_window,
					wxAuiPaneInfo().Name(wxT("variables window"))
					.MinSize( m_ram_window->GetMinSize())
				);

	m_label_window = new sxgo_label_window(this);
    m_mgr.AddPane(	m_label_window,
					wxAuiPaneInfo().Right().Name(wxT("labels window"))
					.MinSize( m_label_window->GetMinSize())
				);

	// make some default perspectives
	wxString perspective_all = m_mgr.SavePerspective();

	// "commit" all changes made to wxAuiManager
	m_mgr.Update();
}

void MyFrame::UpdateAll( const sxgo_document &doc)
{
	m_ram_window->Update( doc.GetState());
	m_variables_window->Update( doc);
	m_label_window->Update( &doc);
}

MyFrame::~MyFrame()
{
	m_mgr.UnInit();
}

wxAuiDockArt* MyFrame::GetDockArt()
{
	return m_mgr.GetArtProvider();
}

void MyFrame::DoUpdate()
{
	m_mgr.Update();
}

void MyFrame::OnEraseBackground(wxEraseEvent& event)
{
	event.Skip();
}

void MyFrame::OnSize(wxSizeEvent& event)
{
	//event.Skip();
	wxFrame::OnSize( event);
}

void MyFrame::OnUpdateUI(wxUpdateUIEvent& event)
{
	unsigned int flags = m_mgr.GetFlags();


}

void MyFrame::OnCreatePerspective(wxCommandEvent& WXUNUSED(event))
{
//	wxTextEntryDialog dlg(this, wxT("Enter a name for the new perspective:"),
//		wxT("wxAUI Test"));

//	dlg.SetValue(wxString::Format(wxT("Perspective %u"), unsigned(m_perspectives.GetCount() + 1)));
//	if (dlg.ShowModal() != wxID_OK)
//		return;

//	if (m_perspectives.GetCount() == 0)
//	{
//		m_perspectives_menu->AppendSeparator();
//	}

//	m_perspectives_menu->Append(ID_FirstPerspective + m_perspectives.GetCount(), dlg.GetValue());
//	m_perspectives.Add(m_mgr.SavePerspective());
}

void MyFrame::OnCopyPerspectiveCode(wxCommandEvent& WXUNUSED(evt))
{
	wxString s = m_mgr.SavePerspective();

#if wxUSE_CLIPBOARD
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(s));
		wxTheClipboard->Close();
	}
#endif
}

void MyFrame::OnRestorePerspective(wxCommandEvent& evt)
{
//	m_mgr.LoadPerspective(m_perspectives.Item(evt.GetId() - ID_FirstPerspective));
}


void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(_("sxgo! A fast SX28 emulator. Copyright 2006-2009 Danny Havenith"), _("About sxgo!"), wxOK, this);
}

wxMenuBar *MyFrame::CreateFrameMenuBar()
{
	wxMenuBar* mb = new wxMenuBar;

	wxMenu* file_menu = new wxMenu;
	file_menu->Append( wxID_OPEN);
	file_menu->Append(wxID_EXIT, _("Exit"));

	//m_perspectives_menu = new wxMenu;
	//m_perspectives_menu->Append(ID_CreatePerspective, _("Create Perspective"));
	//m_perspectives_menu->Append(ID_CopyPerspectiveCode, _("Copy Perspective Data To Clipboard"));
	//m_perspectives_menu->AppendSeparator();
	//m_perspectives_menu->Append(ID_FirstPerspective+0, _("Default Startup"));
	//m_perspectives_menu->Append(ID_FirstPerspective+1, _("All Panes"));


	wxMenu* help_menu = new wxMenu;
	help_menu->Append(wxID_ABOUT, _("About..."));



	mb->Append(file_menu, _("File"));
//	mb->Append(m_perspectives_menu, _("Perspectives"));

	mb->Append(help_menu, _("Help"));

    return mb;
}
