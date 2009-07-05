
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// NOTE THAT THERE IS NO INCLUDE GUARD HERE!

// This include file is essentially a piece of text that facilitates
// creating generic implementations of sx instructions. 
// This is typically used for situations where the implementation is 
// almost the same for all instructions, as in 
// for instance for a clock-cycle calculator, a 'disassembler', etc.

// USAGE:
// * define a macro GENERIC_SX_CLASS_HEADER with the name of your class
//   and optionally a base class that you've implemented.
// * Then define the MACRO GENERIC_SX_IMPLEMENT_INSTRUCTION, that specifies
//   how each function must be implemented.
// * include this file.
// * UNDEFine GENERIC_SX_CLASS_HEADER and GENERIC_SX_IMPLEMENT_INSTRUCTION
//
// see sx_print.hpp for an example.
//
struct GENERIC_SX_CLASS_HEADER
{
    GENERIC_SX_IMPLEMENT_INSTRUCTION( add_w_fr,		(const add_w_fr &, int arg_register), ( add_w_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( add_fr_w,		(const add_fr_w &, int arg_register), ( add_fr_w(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( and_w_lit,	(const and_w_lit &, int arg_lit8), ( and_w_lit(), arg_lit8)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( and_w_fr,		(const and_w_fr &, int arg_register), ( and_w_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( and_fr_w,		(const and_fr_w &, int arg_register), ( and_fr_w(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( clrb_fr_bit,	(const clrb_fr_bit &, int arg_register, int bit), ( clrb_fr_bit(), arg_register, bit)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( setb_fr_bit,	(const setb_fr_bit &, int arg_register, int bit), ( setb_fr_bit(), arg_register, bit)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( snb_fr_bit,	(const snb_fr_bit &, int arg_register, int bit), ( snb_fr_bit(), arg_register, bit)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( sb_fr_bit,	(const sb_fr_bit &, int arg_register, int bit), ( sb_fr_bit(), arg_register, bit)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( call,			(const call &, int arg_addr8), ( call(), arg_addr8)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( clr_fr,		(const clr_fr &, int arg_register), ( clr_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( clr_w,		(const clr_w &), ( clr_w())) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_not_fr,	(const mov_w_not_fr &, int arg_register), ( mov_w_not_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( not_fr,		(const not_fr &, int arg_register), ( not_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_dec_fr,	(const mov_w_dec_fr &, int arg_register), ( mov_w_dec_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( dec_fr,		(const dec_fr &, int arg_register), ( dec_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( movsz_w_dec_fr,(const movsz_w_dec_fr &, int arg_register), ( movsz_w_dec_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( decsz_fr,		(const decsz_fr &, int arg_register), ( decsz_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( jmp,			(const jmp &, int addr9_), ( jmp(), addr9_)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_inc_fr, (const mov_w_inc_fr &, int arg_register), ( mov_w_inc_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( inc_fr,		(const inc_fr &, int arg_register), ( inc_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( movsz_w_inc_fr,(const movsz_w_inc_fr &, int arg_register), ( movsz_w_inc_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( incsz_fr,		(const incsz_fr &, int arg_register), ( incsz_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( or_w_lit,		(const or_w_lit &, int arg_lit8), ( or_w_lit(), arg_lit8)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( or_w_fr,		(const or_w_fr &, int arg_register), ( or_w_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( or_fr_w,		(const or_fr_w &, int arg_register), ( or_fr_w(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_fr,		(const mov_w_fr &, int arg_register), ( mov_w_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( test_fr,		(const test_fr &, int arg_register), ( test_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_lit,	(const mov_w_lit &, int arg_lit8), ( mov_w_lit(), arg_lit8)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_fr_w,		(const mov_fr_w &, int arg_register), ( mov_fr_w(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( retw,			(const retw &, int arg_lit8), ( retw(), arg_lit8)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_shiftleft_fr,  (const mov_w_shiftleft_fr &, int arg_register), ( mov_w_shiftleft_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( rl_fr,		(const rl_fr &, int arg_register), ( rl_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_shiftright_fr, (const mov_w_shiftright_fr &, int arg_register), ( mov_w_shiftright_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( rr_fr,		(const rr_fr &, int arg_register), ( rr_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_fr_minus_w, (const mov_w_fr_minus_w &, int arg_register), ( mov_w_fr_minus_w(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( sub_fr_w,		(const sub_fr_w &, int arg_register), ( sub_fr_w(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_swap_fr,(const mov_w_swap_fr &, int arg_register), ( mov_w_swap_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( swap_fr,		(const swap_fr &, int arg_register), ( swap_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_special_rx_w, (const mov_special_rx_w &, int arg_cregister), ( mov_special_rx_w(), arg_cregister)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( xor_w_lit,	(const xor_w_lit &, int arg_lit8), ( xor_w_lit(), arg_lit8)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( xor_w_fr,		(const xor_w_fr &, int arg_register), ( xor_w_fr(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( xor_fr_w,		(const xor_fr_w &, int arg_register), ( xor_fr_w(), arg_register)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( ret,			(const ret &), ( ret())) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( retp,			(const retp &), ( retp())) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( reti,			(const reti &), ( reti())) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( retiw,		(const retiw &), ( retiw())) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( page,			(const page &, int lit3), ( page(), lit3)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( bank,			(const bank &, int lit3), ( bank(), lit3)) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( iread,		(const iread &), ( iread())) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_m,		(const mov_w_m &), ( mov_w_m())) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_m_w,		(const mov_m_w &), ( mov_m_w())) 
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_m_lit,	(const mov_m_lit &, int arg_lit4), ( mov_m_lit(), arg_lit4)) 
};
