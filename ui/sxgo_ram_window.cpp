#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "sxgo_ram_window.hpp"
#include "sx_simulator.hpp"

DEFINE_EVENT_TYPE(EVT_CHANGE_RAMVALUE)


BEGIN_EVENT_TABLE(sxgo_ram_window, wxGrid)
	EVT_GRID_CELL_CHANGE( sxgo_ram_window::ChangeValue)
END_EVENT_TABLE()


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
		return wxString(hex,wxConvUTF8);
	}

	int HexToDigit( char hex_digit)
	{
		if (hex_digit >= '0' && hex_digit <= '9' ) return hex_digit - '0';
		if (hex_digit >= 'A' && hex_digit <= 'F' ) return hex_digit - 'A' + 10;
		return -1;
	}

	// quickie implementation of a hex-to-int function
	int FromHex( const std::string &value)
	{
		if (value.length() > 2 || value.empty()) return -1;

		std::string upper( boost::algorithm::to_upper_copy( value));

		if (upper.length() == 1)
		{
			return HexToDigit( upper[0]);
		}
		else
		{
			return HexToDigit( upper[0]) * 16 + HexToDigit( upper[1]);
		}
	}


}


sxgo_ram_window::sxgo_ram_window(
	wxWindow *parent, wxWindowID id ,
	const wxPoint& pos ,
	const wxSize& size,
	long style,
	const wxString& name)
	:wxGrid( parent, id, pos, size, style, name)
{
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
		SetRowLabelValue( count, wxString( rowlabels[count],wxConvUTF8));
	}

	for (int count = 0; count < 9; ++count)
	{
		SetColSize( count, 20);
		SetColLabelValue( count, wxString( collabels[count], wxConvUTF8));
	}
	SetMinSize( wxSize( 20 * 11, 16 * 17));
	ForceRefresh();
}

void sxgo_ram_window::ChangeValue( wxGridEvent &event)
{
	// a cell value has changed. If the value can be converted to hex,
	// send an EVT_CHANGE_RAMVALUE event with the absolute ram location in
	// the Int value and the new value in the ExtraLong value.
	int value = FromHex( GetCellValue(
		event.GetRow(), event.GetCol()).mb_str()/*.data()*/
		);

	if (value < 0)
	{
		// we can't translate from hex. veto the event.
		event.Veto();
	}
	else
	{
		int col = event.GetCol();
		int row = event.GetRow();

		wxCommandEvent new_event( EVT_CHANGE_RAMVALUE, GetId() );
		new_event.SetEventObject( this );
		if (col == 0)
		{
			new_event.SetInt( row);
		}
		else
		{
			new_event.SetInt( 16 + row + 32*(col -1));
		}
		new_event.SetExtraLong( value);

		GetEventHandler()->ProcessEvent( new_event);
	}

}

void sxgo_ram_window::Update(const sx_simulator::state &state)
{

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

	int bank = state.ram( sx_emulator::sx_ram::FSR)>>5;

	for (int row = 0; row < 16; ++row)
	{
		SetCellBackgroundColour( row, bank + 1, *wxLIGHT_GREY);
	}
}
