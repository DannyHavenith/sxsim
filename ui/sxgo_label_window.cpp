#include "sxgo_label_window.hpp"
#include "sxgo_doc.hpp"
#include "listing_parser.hpp"

DEFINE_EVENT_TYPE( EVT_JUMPTO_LABEL_LINE)

BEGIN_EVENT_TABLE(sxgo_label_window, wxTreeCtrl)
EVT_TREE_ITEM_ACTIVATED( wxID_ANY, sxgo_label_window::LabelActivated)
END_EVENT_TABLE()

struct item_data : public wxTreeItemData 
{
	item_data( int line_)
		:line( line_)
	{
	}

	int get_line() const
	{
		return line;
	}
private:
	int line;
};

//
// someone doubleclicked on a label in the
// label window.
//
void sxgo_label_window::LabelActivated(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	item_data *item = (item_data *)GetItemData(itemId);
	wxCommandEvent new_event( EVT_JUMPTO_LABEL_LINE, GetId() );
	new_event.SetEventObject( this );
	new_event.SetInt( item->get_line());
	GetEventHandler()->ProcessEvent( new_event);
}

void sxgo_label_window::Update( const sxgo_document *doc)
{
	if ( doc != m_last_document)
	{
		DoUpdate(doc);
		m_last_document = doc;
	}
}

void sxgo_label_window::DoUpdate( const sxgo_document *doc)
{
	DeleteAllItems();
	listing_info info = doc->GetListing();

	wxTreeItemId root = AddRoot( "labels");
	for (
		listing_info::jumplabel_container_type::const_iterator i = info.jump_labels.begin();
		i != info.jump_labels.end();
	++i)
	{
		wxTreeItemId id = AppendItem( root, i->first,-1, -1, 
			new item_data(i->second.line));
		for ( listing_info::label_container_type::const_iterator j = i->second.minor_labels.begin();
			j != i->second.minor_labels.end();
			++j)
		{
			AppendItem( id, j->first, -1, -1, 
				new item_data( j->second));
		}
	}
}