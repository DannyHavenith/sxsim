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
    GENERIC_SX_IMPLEMENT_INSTRUCTION( add_w_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( add_fr_w,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( and_w_lit,	(int arg_lit8),(arg_lit8))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( and_w_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( and_fr_w,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( clrb_fr_bit,	(int arg_register, int bit), (arg_register, bit))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( setb_fr_bit,	(int arg_register, int bit), (arg_register, bit))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( snb_fr_bit,	(int arg_register, int bit), (arg_register, bit))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( sb_fr_bit,	(int arg_register, int bit), (arg_register, bit))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( call,			(int arg_addr8),(arg_addr8))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( clr_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( clr_w,		(), ())
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_not_fr,	(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( not_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_dec_fr,	(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( dec_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( movsz_w_dec_fr, (int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( decsz_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( jmp,			(int addr9_),(addr9_))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_inc_fr,(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( inc_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( movsz_w_inc_fr, (int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( incsz_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( or_w_lit,		(int arg_lit8),(arg_lit8))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( or_w_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( or_fr_w,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( test_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_lit,	(int arg_lit8),(arg_lit8))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_fr_w,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( retw,			(int arg_lit8),(arg_lit8))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_shiftleft_fr, (int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( rl_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_shiftright_fr, (int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( rr_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_fr_minus_w, (int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( sub_fr_w,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_swap_fr,(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( swap_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_special_rx_w, (int arg_cregister),(arg_cregister))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( xor_w_lit,	(int arg_lit8),(arg_lit8))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( xor_w_fr,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( xor_fr_w,		(int arg_register),(arg_register))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( ret,			(), ())
    GENERIC_SX_IMPLEMENT_INSTRUCTION( retp,			(), ())
    GENERIC_SX_IMPLEMENT_INSTRUCTION( reti,			(), ())
    GENERIC_SX_IMPLEMENT_INSTRUCTION( retiw,		(), ())
    GENERIC_SX_IMPLEMENT_INSTRUCTION( page,			(int lit3),(lit3))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( bank,			(int lit3),(lit3))
    GENERIC_SX_IMPLEMENT_INSTRUCTION( iread,		(), ())
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_w_m,		(), ())
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_m_w,		(), ())
    GENERIC_SX_IMPLEMENT_INSTRUCTION( mov_m_lit,	(int arg_lit4),(arg_lit4))
};
