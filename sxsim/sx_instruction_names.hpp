//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SX_INSTRUCTION_NAMES_HPP
#define SX_INSTRUCTION_NAMES_HPP

namespace sx_emulator
{

/// meta-functions to provide the names of sx instructions as strings
template<typename instruction_tag>
struct instruction_name
{
};

template<> struct instruction_name<iread>
{
	static const char * value() { return "iread";}
};
template<> struct instruction_name<mov_w_m>
{
	static const char * value() { return "mov_w_m";}
};
template<> struct instruction_name<mov_m_w>
{
	static const char * value() { return "mov_m_w";}
};
template<> struct instruction_name<clr_w>
{
	static const char * value() { return "clr_w";}
};
template<> struct instruction_name<ret>
{
	static const char * value() { return "ret";}
};
template<> struct instruction_name<retp>
{
	static const char * value() { return "retp";}
};
template<> struct instruction_name<reti>
{
	static const char * value() { return "reti";}
};
template<> struct instruction_name<retiw>
{
	static const char * value() { return "retiw";}
};
template<> struct instruction_name<page>
{
	static const char * value() { return "page";}
};
template<> struct instruction_name<bank>
{
	static const char * value() { return "bank";}
};
template<> struct instruction_name<mov_special_rx_w>
{
	static const char * value() { return "mov_special_rx_w";}
};
template<> struct instruction_name<mov_m_lit>
{
	static const char * value() { return "mov_m_lit";}
};
template<> struct instruction_name<add_w_fr>
{
	static const char * value() { return "add_w_fr";}
};
template<> struct instruction_name<add_fr_w>
{
	static const char * value() { return "add_fr_w";}
};
template<> struct instruction_name<and_w_fr>
{
	static const char * value() { return "and_w_fr";}
};
template<> struct instruction_name<and_fr_w>
{
	static const char * value() { return "and_fr_w";}
};
template<> struct instruction_name<clr_fr>
{
	static const char * value() { return "clr_fr";}
};
template<> struct instruction_name<mov_w_not_fr>
{
	static const char * value() { return "mov_w_not_fr";}
};
template<> struct instruction_name<not_fr>
{
	static const char * value() { return "not_fr";}
};
template<> struct instruction_name<mov_w_dec_fr>
{
	static const char * value() { return "mov_w_dec_fr";}
};
template<> struct instruction_name<dec_fr>
{
	static const char * value() { return "dec_fr";}
};
template<> struct instruction_name<movsz_w_dec_fr>
{
	static const char * value() { return "movsz_w_dec_fr";}
};
template<> struct instruction_name<decsz_fr>
{
	static const char * value() { return "decsz_fr";}
};
template<> struct instruction_name<mov_w_inc_fr>
{
	static const char * value() { return "mov_w_inc_fr";}
};
template<> struct instruction_name<inc_fr>
{
	static const char * value() { return "inc_fr";}
};
template<> struct instruction_name<movsz_w_inc_fr>
{
	static const char * value() { return "movsz_w_inc_fr";}
};
template<> struct instruction_name<incsz_fr>
{
	static const char * value() { return "incsz_fr";}
};
template<> struct instruction_name<or_w_fr>
{
	static const char * value() { return "or_w_fr";}
};
template<> struct instruction_name<or_fr_w>
{
	static const char * value() { return "or_fr_w";}
};
template<> struct instruction_name<mov_w_fr>
{
	static const char * value() { return "mov_w_fr";}
};
template<> struct instruction_name<test_fr>
{
	static const char * value() { return "test_fr";}
};
template<> struct instruction_name<mov_fr_w>
{
	static const char * value() { return "mov_fr_w";}
};
template<> struct instruction_name<mov_w_shiftleft_fr>
{
	static const char * value() { return "mov_w_shiftleft_fr";}
};
template<> struct instruction_name<rl_fr>
{
	static const char * value() { return "rl_fr";}
};
template<> struct instruction_name<mov_w_shiftright_fr>
{
	static const char * value() { return "mov_w_shiftright_fr";}
};
template<> struct instruction_name<mov_w_fr_minus_w>
{
	static const char * value() { return "mov_w_fr_minus_w";}
};
template<> struct instruction_name<sub_fr_w>
{
	static const char * value() { return "sub_fr_w";}
};

template<> struct instruction_name<mov_w_swap_fr>
{
	static const char * value() { return "mov_w_swap_fr:";}
};

template<> struct instruction_name<swap_fr>
{
	static const char * value() { return "swap_fr";}
};
template<> struct instruction_name<xor_w_fr>
{
	static const char * value() { return "xor_w_fr";}
};
template<> struct instruction_name<xor_fr_w>
{
	static const char * value() { return "xor_fr_w";}
};
template<> struct instruction_name<rr_fr>
{
	static const char * value() { return "rr_fr";}
};
template<> struct instruction_name<clrb_fr_bit>
{
	static const char * value() { return "clrb_fr_bit";}
};
template<> struct instruction_name<setb_fr_bit>
{
	static const char * value() { return "setb_fr_bit";}
};
template<> struct instruction_name<snb_fr_bit>
{
	static const char * value() { return "snb_fr_bit";}
};
template<> struct instruction_name<sb_fr_bit>
{
	static const char * value() { return "sb_fr_bit";}
};
template<> struct instruction_name<call>
{
	static const char * value() { return "call";}
};
template<> struct instruction_name<or_w_lit>
{
	static const char * value() { return "or_w_lit";}
};
template<> struct instruction_name<and_w_lit>
{
	static const char * value() { return "and_w_lit";}
};
template<> struct instruction_name<mov_w_lit>
{
	static const char * value() { return "mov_w_lit";}
};
template<> struct instruction_name<retw>
{
	static const char * value() { return "retw";}
};
template<> struct instruction_name<xor_w_lit>
{
	static const char * value() { return "xor_w_lit";}
};
template<> struct instruction_name<jmp>
{
	static const char * value() { return "jmp";}
};

template<> struct instruction_name<breakpoint>
{
	static const char * value() { return "BREAKPOINT";}
};

}
#endif //SX_INSTRUCTION_NAMES_HPP
