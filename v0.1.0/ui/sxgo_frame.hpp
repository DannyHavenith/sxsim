
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// wxWindows class for the main frame of this application.

#include "wx/aui/aui.h"
#include "sxgo_event_definitions.hpp"

class wxMenu;
class sxgo_ram_window;
class sxgo_variables_window;
class sxgo_document;
class sxgo_label_window;

class MyFrame : public wxDocMDIParentFrame , public sxgo_event_definitions
{

public:
	static MyFrame *GetMainFrame(){return main_frame;}
	MyFrame(wxDocManager *manager, wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size,
		long type);

	~MyFrame();

	wxAuiDockArt* GetDockArt();

	void DoUpdate();

	wxAuiManager &GetUIManager() { return m_mgr;}
	void UpdateAll( const sxgo_document &);

private:

	void OnEraseBackground(wxEraseEvent& evt);
	void OnSize(wxSizeEvent& evt);

	void OnExit(wxCommandEvent& evt);
	void OnAbout(wxCommandEvent& evt);
	void OnTabAlignment(wxCommandEvent &evt);

	void OnUpdateUI(wxUpdateUIEvent& evt);

	void OnRestorePerspective(wxCommandEvent& evt);
	void OnCreatePerspective(wxCommandEvent& evt);
	void OnCopyPerspectiveCode(wxCommandEvent& evt);

private:
	static MyFrame *main_frame;
	wxAuiManager m_mgr;
	wxArrayString m_perspectives;
	wxMenu* m_perspectives_menu;
	sxgo_ram_window *m_ram_window;
	sxgo_variables_window *m_variables_window;
	sxgo_label_window *m_label_window;
	DECLARE_EVENT_TABLE()
};

