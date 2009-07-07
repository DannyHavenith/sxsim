
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
		SetCellValue( i, 1, wxString( listing.lines[i].c_str(), wxConvUTF8));
		SetReadOnly( i, 0, true);
		SetReadOnly( i, 1, true);
	}
	InitColWidths();
	AutoSizeColumn( 1);
	SetColSize( 0, 16);
	
	int red_start = wxWHITE->Red();
	int green_start = wxWHITE->Green();
	int blue_start = wxWHITE->Blue();
	int red_end = wxGREEN->Red();
	int green_end = wxGREEN->Green();
	int blue_end = wxGREEN->Blue();
	for (int gradient = 0; gradient < gradient_count; ++gradient)
	{
		int red_gradient = red_start + (red_end - red_start) * (((float)(gradient + 1))/gradient_count);
		int green_gradient = green_start + (green_end - green_start) * (((float)(gradient + 1))/gradient_count);
		int blue_gradient = blue_start + (blue_end - blue_start) * (((float)(gradient + 1))/gradient_count);
		gradients[gradient] = wxColour( red_gradient, green_gradient, blue_gradient); 
	}
	
}

void sxgo_listing_window::ShowLine( unsigned short line)
{
	MakeCellVisible( line, 1);
	SelectBlock( line, 1, line, 1);
}

/// take a profile and visualize it by coloring listing lines
/// This should have the effect that lines that are hit more often get a more intense colour,
/// and the lines that are hit les offten are lighter.
void sxgo_listing_window::ShowProfile( const sxgo_listing_window::profile_type &profile, bool jump_to_most_active)
{
	if (profile.empty()) return;

	// determine the range and map the colour gradient on it.
	profile_type::const_iterator i;

	int max_line = 0;
	// erase the previously set colours, if not overwritten by the 
	// new colours
	for (i = previous_profile.begin(); i != previous_profile.end(); ++i)
	{
		SetCellBackgroundColour( i->first, 1, *wxWHITE);
	}

	// now set the appropriate colours
	int counter = 0;
	int gradient=0;
	int last_value = 0;
	for (i = profile.begin(); i != profile.end(); ++i)
	{
		
		if (i->second != last_value) 
		{
			gradient = counter * gradient_count / profile.size();
		}
		last_value = i->second;
		SetCellBackgroundColour( i->first, 1, gradients[ gradient]);
		max_line = i->first;
		++counter;
	}


	// store the currenly set colours
	previous_profile = profile;
	if (jump_to_most_active)
	{
		JumpToLine( max_line);
	}
	else
	{
		Refresh();
	}
	
}

void sxgo_listing_window::ClearProfile()
{
	if (previous_profile.empty()) return;
	// reset all colours to white.
	for (profile_type::const_iterator i = previous_profile.begin(); i != previous_profile.end(); ++i)
	{
		SetCellBackgroundColour( i->first, 1, *wxWHITE);
	}
	previous_profile.clear();
	JumpToLine( current_line);
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
		val = wxString("X",wxConvUTF8);
	}
	else
	{
		val.clear();
	}
	SetCellValue( line, 0, val);
	return !val.empty();
}
