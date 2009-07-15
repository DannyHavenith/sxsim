
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// This file contains the list of SX instructions and encodes information about the operands of
/// these instructions.

#if !defined( SX_INSTRUCTION_LIST_INCLUDED)
#define SX_INSTRUCTION_LIST_INCLUDED

#include <boost/mpl/joint_view.hpp>
#include "instruction_list.hpp"

namespace sx_emulator
{
	namespace mpl = boost::mpl;
	using boost::mpl::_;
	using boost::mpl::_1;
	using boost::mpl::_2;
	using binary_numbers::as_binary;
	using binary_numbers::to_binary_digits;

	using micro_emulator::masked_argument;
	using micro_emulator::word;


	// define where we can find different arguments in the instruction word.
	// e.g. a 'bit' argument is encoded in bits 5-7, while a
	// 'port'-argument is in bits 0-2.
	struct bit_ 		:	masked_argument< 011100000> {};

	// make a distinction between destination registers and source registers
	// this allows me to optimize the ram breakpoint implementation:
	// instructions that do not write to ram do not have to check for ram 
	// breakpoints.
	struct dest_reg_	:	masked_argument< 000011111> {};
	struct register_	:	masked_argument< 000011111> {};

	struct lit3_		:	masked_argument< 000000111> {};
	struct lit4_		:	masked_argument< 000001111> {};
	struct lit8_ 		:	masked_argument< 011111111> {};
	struct addr8_ 		:	masked_argument< 011111111> {};
	struct addr9_ 		:	masked_argument< 111111111> {};
	struct port_		:	masked_argument< 000000111> {};

	// special instruction: this instruction is not part of the sx instruction set
	// but is used by this emulator to implement breakpoints.
	struct breakpoint	: word< 000001001111> {};

	// instruction tags for the SX instruction set.
	// each tag also happens to encode the bit layout of the instruction.
	struct iread		: word< 000001000001> {};
	struct mov_w_m		: word< 000001000010> {};
	struct mov_m_w		: word< 000001000011> {};
	struct clr_w		: word< 000001000000> {};
	struct ret			: word< 000000001100> {};
	struct retp			: word< 000000001101> {};
	struct reti			: word< 000000001110> {};
	struct retiw		: word< 000000001111> {};
	struct page			: word< 000000010, lit3_> {};
	struct bank			: word< 000000011, lit3_> {};
	struct mov_special_rx_w	: word< 000000000, port_> {};
	struct mov_m_lit	: word< 00000101,  lit4_> {};
	struct add_w_fr		: word< 0001110, register_> {};
	struct add_fr_w		: word< 0001111, dest_reg_> {};
	struct and_w_fr		: word< 0001010, register_> {};
	struct and_fr_w		: word< 0001011, dest_reg_> {};
	struct clr_fr		: word< 0000011, dest_reg_> {};
	struct mov_w_not_fr	: word< 0010010, register_> {};
	struct not_fr		: word< 0010011, dest_reg_> {};
	struct mov_w_dec_fr	: word< 0000110, register_> {};
	struct dec_fr		: word< 0000111, dest_reg_> {};
	struct movsz_w_dec_fr	: word< 0010110, register_> {};
	struct decsz_fr		: word< 0010111, dest_reg_> {};
	struct mov_w_inc_fr	: word< 0010100, register_> {};
	struct inc_fr		: word< 0010101, dest_reg_> {};
	struct movsz_w_inc_fr	: word< 0011110, register_> {};
	struct incsz_fr		: word< 0011111, dest_reg_> {};
	struct or_w_fr		: word< 0001000, register_> {};
	struct or_fr_w		: word< 0001001, dest_reg_> {};
	struct mov_w_fr		: word< 0010000, register_> {};
	struct test_fr		: word< 0010001, register_> {};
	struct mov_fr_w		: word< 0000001, dest_reg_> {};
	struct mov_w_shiftleft_fr	: word< 0011010, register_> {};
	struct rl_fr		: word< 0011011, dest_reg_> {};
	struct mov_w_shiftright_fr	: word< 0011000, register_> {};
	struct mov_w_fr_minus_w	: word< 0000100, register_> {};
	struct sub_fr_w		: word< 0000101, dest_reg_> {};
	struct mov_w_swap_fr: word< 0011100, register_> {};
	struct swap_fr		: word< 0011101, dest_reg_> {};
	struct xor_w_fr		: word< 0001100, register_> {};
	struct xor_fr_w		: word< 0001101, dest_reg_> {};
	struct rr_fr		: word< 0011001, dest_reg_> {};
	struct clrb_fr_bit	: word< 0100 , dest_reg_, bit_> {};
	struct setb_fr_bit	: word< 0101 , dest_reg_, bit_> {};
	struct snb_fr_bit	: word< 0110 , register_, bit_> {};
	struct sb_fr_bit	: word< 0111 , register_, bit_> {};
	struct call			: word< 1001 , addr8_> {};
	struct or_w_lit		: word< 1101 , lit8_> {};
	struct and_w_lit	: word< 1110 , lit8_> {};
	struct mov_w_lit	: word< 1100 , lit8_> {};
	struct retw			: word< 1000 , lit8_> {};
	struct xor_w_lit	: word< 1111 , lit8_> {};
	struct jmp			: word< 101, addr9_> {};



	struct sx_instruction_list
	{


		/* 
		some information about bit patterns that have no official meaning.
		you can safely ignore this:

		hole: 1 bits: 2 to 1
		000000001Xzz
		hole: 100 bits: 3 to 1
		00000100XXzz
		*/

		// define a list of sx instructions.
		typedef mpl::vector<
			breakpoint,
			iread,
			mov_w_m,
			mov_m_w,
			clr_w,
			ret,
			retp,
			reti,
			retiw,
			page,
			bank,
			mov_special_rx_w,
			mov_m_lit,
			add_w_fr,
			add_fr_w,
			and_w_fr,
			and_fr_w,
			clr_fr,
			mov_w_not_fr,
			not_fr,
			mov_w_dec_fr,
			dec_fr,
			movsz_w_dec_fr,
			decsz_fr,
			mov_w_inc_fr,
			inc_fr,
			movsz_w_inc_fr
		> i1;

		typedef mpl::vector<
			incsz_fr,
			or_w_fr,
			or_fr_w,
			mov_w_fr,
			test_fr,
			mov_fr_w,
			mov_w_shiftleft_fr,
			rl_fr,
			mov_w_shiftright_fr,
			mov_w_fr_minus_w,
			sub_fr_w,
			mov_w_swap_fr,
			swap_fr,
			xor_w_fr,
			xor_fr_w,
			rr_fr,
			clrb_fr_bit,
			setb_fr_bit,
			snb_fr_bit,
			sb_fr_bit,
			call,
			or_w_lit,
			and_w_lit,
			mov_w_lit,
			retw,
			xor_w_lit,
			jmp
		> i2;

		// we're joining two vectors because currently the absolute limit to
		// the size of an mpl::vector is 50.
		// since this creates a ForwardSequence instead of a Random Access Sequence
		// we'd probably be better of starting with lists in the first place.
		typedef mpl::joint_view< i1,i2> instructions;
	};
}
#endif //SX_INSTRUCTION_LIST_INCLUDED
