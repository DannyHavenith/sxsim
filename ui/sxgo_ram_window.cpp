#include <string>
#include <boost/lexical_cast.hpp>
#include "sxgo_ram_window.hpp"
#include "sx_simulator.hpp"
#include <sstream>

std::string ToHex( int value)
{
	std::ostringstream s;
	s << std::hex << value;
	return s.str();
}

sxgo_ram_window::sxgo_ram_window(
	wxWindow *parent, wxWindowID id ,
	const wxPoint& pos ,
	const wxSize& size,
	long style,
	const wxString& name)
	:wxGrid( parent, id, pos, size, style, name)
{ 
	SetMinSize( wxSize( 20 * 9, 16 * 17));
	static const char *rowlabels[] = 
	{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

	static const char *collabels[] = 
	{ "0X", "1x", "3x", "5x", "7x", "9x", "Bx", "Dx", "Fx"};

	CreateGrid( 16, 9);
	SetRowLabelSize(20);
	SetColLabelSize(20);
	for (int count = 0; count < 16; ++count)
	{
		SetRowSize( count, 16);
		SetCellBackgroundColour( count, 0, *wxLIGHT_GREY);
		SetRowLabelValue( count, rowlabels[count]);
	}

	for (int count = 0; count < 9; ++count)
	{
		SetColSize( count, 20);
		SetColLabelValue( count, collabels[count]);
	}
	ForceRefresh();
}


void sxgo_ram_window::Update(const sx_simulator::state &state)
{
	static const char *lower_labels[] = 
	{"0x", "2x", "4x", "6x", "8x", "Ax", "Cx", "Ex"};

	for (int row = 0; row < 16; ++row)
	{
		SetCellValue( row, 0, ToHex( state.ram.get_absolute( row)));
	}

	for (int col = 0; col < 8; ++col)
	{
		for (int row = 0; row < 16; ++row)
		{
			SetCellBackgroundColour( row, col + 1, *wxWHITE);
			SetCellValue( row, col + 1, ToHex( state.ram.get_absolute( 16 + row + 32 * col)));
		}
	}

	int bank = state.ram( micro_emulator::sx_ram::FSR)>>5;

	SetColLabelValue( 0, lower_labels[bank]);
	for (int row = 0; row < 16; ++row)
	{
		SetCellBackgroundColour( row, bank + 1, *wxLIGHT_GREY);
	}
}