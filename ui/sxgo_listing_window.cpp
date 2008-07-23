#include "sxgo_listing_window.hpp"
#include "listing_parser.hpp"
#include "wx/log.h"
void sxgo_listing_window::SetListing( const listing_info &listing_)
{
	listing = listing_;
	CreateGrid( listing.lines.size(), 2);
	//int max_width = 0;
	for (unsigned int i = 0; i < listing.lines.size(); ++i)
	{
		//int width;
		//int height;

		//GetTextExtent( listing.lines[i].c_str(), &width, &height);
		//max_width = std::max( width, max_width);

		SetCellBackgroundColour( i, 0, *wxLIGHT_GREY);
		SetCellValue( i, 1, listing.lines[i].c_str());
		SetReadOnly( i, 0, true);
		SetReadOnly( i, 1, true);
	}
	InitColWidths();
	AutoSizeColumn( 1);
	SetColSize( 0, 16);
}

void sxgo_listing_window::JumpToAddress( unsigned short address)
{
	int line = listing.address_to_line[address];
	MakeCellVisible( line, 1);
	SetCellBackgroundColour( current_line, 1, *wxWHITE);
	SetCellBackgroundColour( line, 1, *wxGREEN);
	current_line = line;
	::wxLogStatus( "At address %d, line %d", (int)address, (int) line);
	Refresh();
}