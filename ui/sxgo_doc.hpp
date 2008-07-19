#if !defined (SXGO_DOC_HPP)
#define SXGO_DOC_HPP

#include "wx/docview.h"

class wxString;
class TextEditDocument: public wxDocument
{
	DECLARE_DYNAMIC_CLASS(TextEditDocument)
private:
public:

	virtual bool OnSaveDocument(const wxString& filename);
	virtual bool OnOpenDocument(const wxString& filename);
};

#endif //SXGO_DOC_HPP