#include "sxgo_doc.hpp"
#include "sxgo_view.hpp"

// not a wanted dependency...
#include "sxgo_listing_window.hpp"

IMPLEMENT_DYNAMIC_CLASS(TextEditDocument, wxDocument)

bool TextEditDocument::OnSaveDocument(const wxString& filename)
{
	return true;
}

bool TextEditDocument::OnOpenDocument(const wxString& filename)
{
	SetFilename(filename, true);
	UpdateAllViews();
	sxgo_view *view = (sxgo_view *)GetFirstView();
	if (!view->textsw->LoadFile(filename))
		return false;

	Modify(false);
	
	return true;
}

