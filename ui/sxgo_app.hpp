#include "wx/app.h"
// -- application --
class MyFrame;
class wxMDIChildFrame;

class MyApp : public wxApp
{
public:
	bool OnInit();
	wxMDIChildFrame *CreateChildFrame(wxDocument *doc, wxView *view);
	MyApp() : frame(0), m_docManager(0) {}
	MyFrame *GetFrame() { return frame;}
private:
	MyFrame *frame;
	wxDocManager* m_docManager;
};

DECLARE_APP(MyApp)
