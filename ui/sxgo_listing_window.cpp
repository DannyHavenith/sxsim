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

void sxgo_listing_window::ShowLine( unsigned short line)
{
	MakeCellVisible( line, 1);
	SelectBlock( line, 1, line, 1);
}

void sxgo_listing_window::JumpToLine( unsigned short line)
{
	MakeCellVisible( line, 1);
	SetCellBackgroundColour( current_line, 1, *wxWHITE);
	SetCellBackgroundColour( line, 1, *wxGREEN);
	current_line = line;
	Refresh();
}

bool sxgo_listing_window::ToggleBreakpoint( int line)
{
	wxString val = GetCellValue( line, 0);
	if (val.empty())
	{
		val = "X";
	}
	else
	{
		val.clear();
	}
	SetCellValue( line, 0, val);
	return !val.empty();
}
