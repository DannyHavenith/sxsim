#if !defined( SX_INSTRUCTION_LIST_INCLUDED)
#define SX_INSTRUCTION_LIST_INCLUDED

#include <boost/mpl/joint_view.hpp>
#include "instruction_list.hpp"

// I'm adding this to the micro_emulator because I'm lazy and
// for no other reason (it should be in a dedicated sx-microcontroller namespace).
namespace micro_emulator 
{
	namespace mpl = boost::mpl;
	using boost::mpl::_;
	using boost::mpl::_1;
	using boost::mpl::_2;
	using binary_numbers::as_binary;
	using binary_numbers::to_binary_digits;

	template< typename implementation>
	struct sx_instruction_list : public instruction_list< implementation>
	{
		// define where we can find different arguments in the instruction word.
		// e.g. a 'bit' argument is encoded in bits 5-7, while a 
		// 'port'-argument is in bits 0-2.
		struct bit_ 		:	masked_argument< 011100000> {};
		struct register_	:	masked_argument< 000011111> {};
		struct lit3_		:	masked_argument< 000000111> {};
		struct lit4_		:	masked_argument< 000001111> {};
		struct lit8_ 		:	masked_argument< 011111111> {};
		struct addr8_ 		:	masked_argument< 011111111> {};
		struct addr9_ 		:	masked_argument< 111111111> {};
		struct port_		:	masked_argument< 000000111> {};

		typedef mpl::vector<
// some instructions are aliases for special cases of more general instructions.
//			instruction< word< 111111111111>,			&impl::not_w>  // is xor_w_lit (%11111111)
//			instruction< word< 010000000011>,			&impl::clc>, // is clrb_fr_bit
//			instruction< word< 000000000100>,			&impl::clr_special_wdt>,// special, is also mov_special_rx_w
//			instruction< word< 000000000000>,			&impl::nop>,//is mov_special_rx_w
//			instruction< word< 000000000010>,			&impl::mov_special_option_w>, // is mov_special_rx_w
//			instruction< word< 000000000011>,			&impl::sleep>, // special, is also mov_special_rx_w
//			instruction< word< 010001000011>,			&impl::clz>, // clrb_fr_bit
//			instruction< word< 011000000010>,			&impl::skip>, // is snb_fr_bit/sb_fr_bit
//			instruction< word< 011100000011>,			&impl::sc> // is sb_fr_bit
//			instruction< word< 0000001, register_>,		&impl::jmp_w>, // these are just move pc, w and 
//			instruction< word< 0001111, register_>,		&impl::jmp_pc_plus_w>,// add pc, w

			instruction< word< 000001000001>,			&impl::iread>,
			instruction< word< 000001000010>,			&impl::mov_w_m>,
			instruction< word< 000001000011>,			&impl::mov_m_w>,
			instruction< word< 000001000000>,			&impl::clr_w>,
			instruction< word< 000000001100>,			&impl::ret>,
			instruction< word< 000000001101>,			&impl::retp>,
			instruction< word< 000000001110>,			&impl::reti>,
			instruction< word< 000000001111>,			&impl::retiw>
		> i1;

		// the instructions do not need to be sorted on operand size, 
		// I just did this for the aesthetics of it...
		typedef mpl::vector<
			instruction< word< 000000010, lit3_>,		&impl::page>,
			instruction< word< 000000011, lit3_>,		&impl::bank>,
			instruction< word< 000000000, port_>,		&impl::mov_special_rx_w>,
			instruction< word< 00000101 , lit4_>,		&impl::mov_m_lit>,
			instruction< word< 0001110, register_>,		&impl::add_w_fr>,
			instruction< word< 0001111, register_>,		&impl::add_fr_w>,
			instruction< word< 0001010, register_>,		&impl::and_w_fr>,
			instruction< word< 0001011, register_>,		&impl::and_fr_w>,
			instruction< word< 0000011, register_>,		&impl::clr_fr>,
			instruction< word< 0010010, register_>,		&impl::mov_w_not_fr>,
			instruction< word< 0010011, register_>,		&impl::not_fr>,
			instruction< word< 0000110, register_>,		&impl::mov_w_dec_fr>,
			instruction< word< 0000111, register_>,		&impl::dec_fr>,
			instruction< word< 0010110, register_>,		&impl::movsz_w_dec_fr>,
			instruction< word< 0010111, register_>,		&impl::decsz_fr>,
			instruction< word< 0010100, register_>,		&impl::mov_w_inc_fr>,
			instruction< word< 0010101, register_>,		&impl::inc_fr>,
			instruction< word< 0011110, register_>,		&impl::movsz_w_inc_fr>,
			instruction< word< 0011111, register_>,		&impl::incsz_fr>,
			instruction< word< 0001000, register_>,		&impl::or_w_fr>,
			instruction< word< 0001001, register_>,		&impl::or_fr_w>,
			instruction< word< 0010000, register_>,		&impl::mov_w_fr>,
			instruction< word< 0010001, register_>,		&impl::test_fr>,
			instruction< word< 0000001, register_>,		&impl::mov_fr_w>,
			instruction< word< 0011010, register_>,		&impl::mov_w_shiftleft_fr>,
			instruction< word< 0011011, register_>,		&impl::rl_fr>,
			instruction< word< 0011000, register_>,		&impl::mov_w_shiftright_fr>,
			instruction< word< 0000100, register_>,		&impl::mov_w_fr_minus_w>,
			instruction< word< 0000101, register_>,		&impl::sub_fr_w>,
			instruction< word< 0011100, register_>,		&impl::mov_w_swap_fr>,
			instruction< word< 0011101, register_>,		&impl::swap_fr>,
			instruction< word< 0001100, register_>,		&impl::xor_w_fr>,
			instruction< word< 0001101, register_>,		&impl::xor_fr_w>,
			instruction< word< 0011001, register_>,		&impl::rr_fr>,
			instruction< word< 0100 , register_, bit_>, &impl::clrb_fr_bit>,
			instruction< word< 0101 , register_, bit_>, &impl::setb_fr_bit>,
			instruction< word< 0110 , register_, bit_>, &impl::snb_fr_bit>,
			instruction< word< 0111 , register_, bit_>, &impl::sb_fr_bit>,
			instruction< word< 1001 , addr8_>,			&impl::call>,
			instruction< word< 1101 , lit8_>,			&impl::or_w_lit>,
			instruction< word< 1110 , lit8_>,			&impl::and_w_lit>,
			instruction< word< 1100 , lit8_>,			&impl::mov_w_lit>,
			instruction< word< 1000 , lit8_>,			&impl::retw>,
			instruction< word< 1111 , lit8_>,			&impl::xor_w_lit>,
			instruction< word< 101, addr9_>,			&impl::jmp>
		> i2;

		// we're joining two vectors because currently the absolute limit to
		// the size of an mpl::vector is 50.
		// since this creates a ForwardSequence instead of a Random Access Sequence
		// we'd probably be better of starting with lists in the first place.
		typedef mpl::joint_view< i1,i2> instructions;
	};
}
#endif //SX_INSTRUCTION_LIST_INCLUDED