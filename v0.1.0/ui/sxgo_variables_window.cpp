
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <boost/lexical_cast.hpp>
#include "sxgo_variables_window.hpp"
#include "sx_simulator.hpp"
#include "sxgo_doc.hpp"
#include <sstream>

namespace
{
	char DigitToHex( int value)
	{
		if (value >9)
		{
			return 'A' - 10 + value;
		}
		else
		{
			return '0' + value;
		}
	}

	wxString ToHex( int value)
	{
		char hex[] = {
			DigitToHex( value/16),
			DigitToHex( value %16),
			0
		};
		return wxString( hex,  wxConvUTF8);
	}
}

sxgo_variables_window::sxgo_variables_window(
	wxWindow *parent, wxWindowID id ,
	const wxPoint& pos ,
	const wxSize& size,
	long style,
	const wxString& name)
	:wxGrid( parent, id, pos, size, style, name)
{
	SetMinSize( wxSize( 20 * 9, 16 * 17));
	CreateGrid( 1, 3);
	SetRowLabelSize(0);

	SetColLabelValue( 0, wxT("name"));
	SetColLabelValue( 1, wxT("address"));
	SetColLabelValue( 2, wxT("value"));
	ForceRefresh();
}


void sxgo_variables_window::Update(const sxgo_document &doc)
{
	const listing_info listing = doc.GetListing();
	int rowcount = listing.data_labels.size() + 1;

	if (GetNumberRows() > rowcount)
	{
		DeleteRows( rowcount, GetNumberRows() - rowcount);
	}
	else if (GetNumberRows() < rowcount)
	{
		InsertRows( GetNumberRows(), rowcount - GetNumberRows());
	}

	sx_state s = doc.GetState();

	SetCellValue( 0, 0, wxT("w"));
	SetCellValue( 0, 1, wxT("na"));
	SetCellValue( 0, 2, ToHex( s.w));

	int row = 1;
	for (listing_info::label_container_type::const_iterator i = listing.data_labels.begin();
		i != listing.data_labels.end();
		++i)
	{
		SetCellValue( row, 0, wxString( i->first.c_str(), wxConvUTF8));
		SetCellValue( row, 1, ToHex( i->second));
		SetCellValue( row, 2, ToHex( s.ram.get_absolute( i->second)));
		++row;
	}
}
