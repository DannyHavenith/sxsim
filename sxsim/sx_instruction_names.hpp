
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SX_INSTRUCTION_NAMES_HPP
#define SX_INSTRUCTION_NAMES_HPP

/// meta-functions to provide the names of sx instructions as strings
template< typename instruction_tag> 
struct instruction_name {};

template<> struct instruction_name<iread> { const char *value = "iread";};
template<> struct instruction_name<mov_w_m> { const char *value = "mov_w_m";};
template<> struct instruction_name<mov_m_w> { const char *value = "mov_m_w";};
template<> struct instruction_name<clr_w> { const char *value = "clr_w";};
template<> struct instruction_name<ret> { const char *value = "ret";};
template<> struct instruction_name<retp> { const char *value = "retp";};
template<> struct instruction_name<reti> { const char *value = "reti";};
template<> struct instruction_name<retiw> { const char *value = "retiw";};
template<> struct instruction_name<page> { const char *value = "page";};
template<> struct instruction_name<bank> { const char *value = "bank";};
template<> struct instruction_name<mov_special_rx_w> { const char *value = "mov_special_rx_w";};
template<> struct instruction_name<mov_m_lit> { const char *value = "mov_m_lit";};
template<> struct instruction_name<add_w_fr> { const char *value = "add_w_fr";};
template<> struct instruction_name<add_fr_w> { const char *value = "add_fr_w";};
template<> struct instruction_name<and_w_fr> { const char *value = "and_w_fr";};
template<> struct instruction_name<and_fr_w> { const char *value = "and_fr_w";};
template<> struct instruction_name<clr_fr> { const char *value = "clr_fr";};
template<> struct instruction_name<mov_w_not_fr> { const char *value = "mov_w_not_fr";};
template<> struct instruction_name<not_fr> { const char *value = "not_fr";};
template<> struct instruction_name<mov_w_dec_fr> { const char *value = "mov_w_dec_fr";};
template<> struct instruction_name<dec_fr> { const char *value = "dec_fr";};
template<> struct instruction_name<movsz_w_dec_fr> { const char *value = "movsz_w_dec_fr";};
template<> struct instruction_name<decsz_fr> { const char *value = "decsz_fr";};
template<> struct instruction_name<mov_w_inc_fr> { const char *value = "mov_w_inc_fr";};
template<> struct instruction_name<inc_fr> { const char *value = "inc_fr";};
template<> struct instruction_name<movsz_w_inc_fr> { const char *value = "movsz_w_inc_fr";};
template<> struct instruction_name<incsz_fr> { const char *value = "incsz_fr";};
template<> struct instruction_name<or_w_fr> { const char *value = "or_w_fr";};
template<> struct instruction_name<or_fr_w> { const char *value = "or_fr_w";};
template<> struct instruction_name<mov_w_fr> { const char *value = "mov_w_fr";};
template<> struct instruction_name<test_fr> { const char *value = "test_fr";};
template<> struct instruction_name<mov_fr_w> { const char *value = "mov_fr_w";};
template<> struct instruction_name<mov_w_shiftleft_fr> { const char *value = "mov_w_shiftleft_fr";};
template<> struct instruction_name<rl_fr> { const char *value = "rl_fr";};
template<> struct instruction_name<mov_w_shiftright_fr> { const char *value = "mov_w_shiftright_fr";};
template<> struct instruction_name<mov_w_fr_minus_w> { const char *value = "mov_w_fr_minus_w";};
template<> struct instruction_name<sub_fr_w> { const char *value = "sub_fr_w";};
template<> struct instruction_name<mov_w_swap_fr:> { const char *value = "mov_w_swap_fr:";};
template<> struct instruction_name<swap_fr> { const char *value = "swap_fr";};
template<> struct instruction_name<xor_w_fr> { const char *value = "xor_w_fr";};
template<> struct instruction_name<xor_fr_w> { const char *value = "xor_fr_w";};
template<> struct instruction_name<rr_fr> { const char *value = "rr_fr";};
template<> struct instruction_name<clrb_fr_bit> { const char *value = "clrb_fr_bit";};
template<> struct instruction_name<setb_fr_bit> { const char *value = "setb_fr_bit";};
template<> struct instruction_name<snb_fr_bit> { const char *value = "snb_fr_bit";};
template<> struct instruction_name<sb_fr_bit> { const char *value = "sb_fr_bit";};
template<> struct instruction_name<call> { const char *value = "call";};
template<> struct instruction_name<or_w_lit> { const char *value = "or_w_lit";};
template<> struct instruction_name<and_w_lit> { const char *value = "and_w_lit";};
template<> struct instruction_name<mov_w_lit> { const char *value = "mov_w_lit";};
template<> struct instruction_name<retw> { const char *value = "retw";};
template<> struct instruction_name<xor_w_lit> { const char *value = "xor_w_lit";};
template<> struct instruction_name<jmp> { const char *value = "jmp";};
#endif //SX_INSTRUCTION_NAMES_HPP