// sxsim.cpp : Defines the entry point for the console application.
//
// can I do this in a header?
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include <iostream>
#include "sx_instruction_list.hpp"
#include "instruction_decoder.hpp"
#include "sx_nop.hpp"
#include "sx_print.hpp"

using namespace micro_emulator;

template< size_t size>
void print_instructions( const int (&instructions)[size])
{
	typedef instruction_decoder< 
		sx_instruction_list<
		sx_print
		>
	> sx_printer;
	sx_printer printer;


	for (int count = 1;
		count;
		--count)
	{
		for (const int *i = &instructions[0];
			i!= &instructions[0] + size;
			++i)
		{
			printer.feed( *i);
		}
	}

}

volatile int x = 0x02F3;
int main(int argc, char* argv[])
{
	int instructions[] = {
		0x403,
		0x02F3,
		0x0A10,
		0x0C0A,
		0x0033,
		0x0232,
		0x0643,
		0x0A10,
		0x0503,
		0x0330,
		0x0331,
		0x00F2,
		0x06D1,
		0x0425,
		0x07D1,
		0x0525,
		0x0705,
		0x0503,
		0x0605,
		0x0403,
		0x0234,
		0x0743,
		0x0A1C,
		0x0C09,
		0x0703,
		0x0034,
		0x0C10,
		0x0035,
		0x02F5,
		0x0A25,
		0x0C0A,
		0x0035,
		0x00F4,
		0x0743,
		0x0336,
		0x0643,
		0x0517,
		0x0C59,
		0x000F	
	};

	print_instructions( instructions);

/* should translate to:
clrb_fr_bit 3 0
decsz_fr 19
jmp 16
mov_w_lit 10
mov_fr_w 19
test_fr 18
snb_fr_bit 3 2
jmp 16
setb_fr_bit 3 0
rr_fr 16
rr_fr 17
dec_fr 18
snb_fr_bit 17 6
clrb_fr_bit 5 1
sb_fr_bit 17 6
setb_fr_bit 5 1
sb_fr_bit 5 0
setb_fr_bit 3 0
snb_fr_bit 5 0
clrb_fr_bit 3 0
test_fr 20
sb_fr_bit 3 2
jmp 28
mov_w_lit 9
sb_fr_bit 3 0
mov_fr_w 20
mov_w_lit 16
mov_fr_w 21
decsz_fr 21
jmp 37
mov_w_lit 10
mov_fr_w 21
dec_fr 20
sb_fr_bit 3 2
rr_fr 22
snb_fr_bit 3 2
setb_fr_bit 23 0
mov_w_lit 89
retiw
*/

	return 0;
}

