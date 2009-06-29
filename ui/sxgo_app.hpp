
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// Definition of the wxWindows application class for this project

#include "wx/app.h"
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
