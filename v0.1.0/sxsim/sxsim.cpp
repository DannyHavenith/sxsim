
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// Very, very simple first approach at a command line simulator. 
/// Has been superseded by sxgoui and by pysix.

// can I do this in a header?
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "listing_parser.hpp"
#include "sx_instruction_list.hpp"
#include "instruction_decoder.hpp"
#include "sx_nop.hpp"
#include "sx_print.hpp"
#include "sx_controller.hpp"

using namespace micro_emulator;
using namespace std;
using namespace boost;
typedef sx_nop impl;
typedef sx_instruction_list< sx_nop> list;
typedef instruction_decoder< list> decoder;


void single_step( const listing_info &listing)
{
	sx_emulator::sx_controller sx;
	typedef instruction_decoder< sx_instruction_list< sx_print> > print_decoder;

	sx_print printer;

	sx.load_rom( listing.instructions);
	unsigned short last_known_address = 0;
	try
	{

		unsigned long counter = 5000000;
		while (counter--)
		{
				last_known_address = sx.get_pc();
				//cout << setw(4) << hex << last_known_address << ":" << hex << setw(4) << sx.get_rom()( last_known_address) << ":" << listing.source_lines[ last_known_address] << "\n";
				sx.tick();

				if (last_known_address == 0x0407)
				{
					cout << '.' << char(sx.get_ram().get_absolute( 0x50));
				}
		}
	}
	catch (const std::exception &e)
	{
		cerr << "error on address (" << last_known_address << "):" << e.what() << endl;
		cerr << "source: " << listing.GetLine( last_known_address) << endl;
	}



}
/**/


/*
template< typename T>
void print_bits()
{
	cout << decoder::opcode_bit_at< T, 11>::type::value;
	cout << decoder::opcode_bit_at< T, 10>::type::value;
	cout << decoder::opcode_bit_at< T, 9>::type::value;
	cout << decoder::opcode_bit_at< T, 8>::type::value;
	cout << decoder::opcode_bit_at< T, 7>::type::value;
	cout << decoder::opcode_bit_at< T, 6>::type::value;
	cout << decoder::opcode_bit_at< T, 5>::type::value;
	cout << decoder::opcode_bit_at< T, 4>::type::value;
	cout << decoder::opcode_bit_at< T, 3>::type::value;
	cout << decoder::opcode_bit_at< T, 2>::type::value;
	cout << decoder::opcode_bit_at< T, 1>::type::value;
	cout << decoder::opcode_bit_at< T, 0>::type::value;
	cout << endl;
}
/**/

int main(int argc, char* argv[])
{

	if (argc < 2)
	{
		cerr << "usage: sxsim <listfile>\n";
		return -1;
	}

	ifstream listfile( argv[1]);
	if (!listfile)
	{
		cerr << "could not open " << argv[1] << " for reading\n";
		return -2;
	}

	listing_info listing = ParseListingFile( listfile);

	single_step( listing);

	cout << "press enter>" << endl;
	cin.get();
}

