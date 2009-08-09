
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#if !defined( SXGO_LABEL_WINDOW_HPP)
#define SXGO_LABEL_WINDOW_HPP
#include "wx/treectrl.h"

DECLARE_EVENT_TYPE(EVT_JUMPTO_LABEL_LINE, -1)

class sxgo_document;
class sxgo_label_window : public wxTreeCtrl 
{
public:
	explicit sxgo_label_window( wxWindow *parent)
		:wxTreeCtrl( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT),
		m_last_document(0), m_last_id(0)
	{
		SetMinSize( wxSize( 150, 100));
	}

	void Update( const sxgo_document *doc);

	void LabelActivated(wxTreeEvent& event);

private:
    DECLARE_EVENT_TABLE()

	void DoUpdate( const sxgo_document *doc);
	const sxgo_document *m_last_document;
    int m_last_id; // id of last document
};

#endif //SXGO_LABEL_WINDOW_HPP