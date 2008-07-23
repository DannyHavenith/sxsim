#include "wx/aui/aui.h"
#include "sxgo_event_definitions.hpp"

class wxMenu;
class MyFrame : public wxDocMDIParentFrame , public sxgo_event_definitions
{

public:
	MyFrame(wxDocManager *manager, wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size,
		long type);

	~MyFrame();

	wxAuiDockArt* GetDockArt();
	void DoUpdate();

	wxAuiManager &GetUIManager() { return m_mgr;}

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

	wxAuiManager m_mgr;
	wxArrayString m_perspectives;
	wxMenu* m_perspectives_menu;

	DECLARE_EVENT_TABLE()
};

