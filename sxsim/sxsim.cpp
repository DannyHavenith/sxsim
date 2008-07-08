// sxsim.cpp : Defines the entry point for the console application.
//
// can I do this in a header?
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include <iostream>
#include <fstream>
#include <iomanip>
#include "listing_parser.hpp"
#include "sx_instruction_list.hpp"
#include "instruction_decoder.hpp"
#include "sx_nop.hpp"
#include "sx_print.hpp"
#include "sx_controller.hpp"

using namespace micro_emulator;
using namespace std;

typedef sx_nop impl;
typedef sx_instruction_list< sx_nop> list;
typedef instruction_decoder< list> decoder;


void single_step( const listing_info &listing)
{
	sx_controller sx;
	typedef instruction_decoder< sx_instruction_list< sx_print> > print_decoder;

	sx_print printer;

	sx.load_rom( listing.instructions);
	unsigned short last_known_address = 0;
	try
	{

		unsigned long counter = 50000000;
		while (counter--)
		{
				last_known_address = sx.get_pc();
				// "breakpoint"
				//if (last_known_address == 0)
				//{
			//		cout << "interrupt***";
		//			cin.get();
	//			}
				//cout << setw(4) << hex << last_known_address << ":" << hex << setw(4) << sx.get_rom()( last_known_address) << ":" << listing.source_lines[ last_known_address] << "\n";
				//print_decoder::feed( sx.get_rom()( last_known_address), printer);
				//cout << std::endl;
				//cin.get();
				sx.tick();
			
				if (last_known_address == 0x0407)
				{
					cout << '.' << char(sx.get_ram()( 0x12));
				}
		}
	}
	catch (const recoverable_error &e)
	{
		cerr << "error on address (" << last_known_address << "):" << e.what() << endl;
		cerr << "source: " << listing.source_lines[ last_known_address] << endl;
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

