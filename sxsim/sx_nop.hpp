#if !defined( SX_NOP_INCLUDED)
#define SX_NOP_INCLUDED

#define IMPLEMENT_INSTRUCTION( fname, args_in, args_out) \
	void fname args_in	\
	{											\
		store_arg args_out;	\
	}

namespace {
	static volatile int secret = 0;
}

struct sx_nop
{
	IMPLEMENT_INSTRUCTION( add_w_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( add_fr_w,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( and_w_lit,  (int arg_lit8),(arg_lit8))
	IMPLEMENT_INSTRUCTION( and_w_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( and_fr_w,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( clrb_fr_bit,  (int arg_register, int bit), (arg_register, bit))
	IMPLEMENT_INSTRUCTION( setb_fr_bit,  (int arg_register, int bit), (arg_register, bit))
	IMPLEMENT_INSTRUCTION( snb_fr_bit,  (int arg_register, int bit), (arg_register, bit))
	IMPLEMENT_INSTRUCTION( sb_fr_bit,  (int arg_register, int bit), (arg_register, bit))
	IMPLEMENT_INSTRUCTION( call,  (int arg_addr8),(arg_addr8))
	IMPLEMENT_INSTRUCTION( clr_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( clr_w, (), ())
	IMPLEMENT_INSTRUCTION( clr_special_wdt, (), ())
	IMPLEMENT_INSTRUCTION( mov_w_not_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( not_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( mov_w_dec_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( dec_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( movsz_w_dec_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( decsz_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( jmp,  (int addr9_),(addr9_))
	IMPLEMENT_INSTRUCTION( mov_w_inc_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( inc_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( movsz_w_inc_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( incsz_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( or_w_lit,  (int arg_lit8),(arg_lit8))
	IMPLEMENT_INSTRUCTION( or_w_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( or_fr_w,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( mov_w_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( test_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( mov_w_lit,  (int arg_lit8),(arg_lit8))
	IMPLEMENT_INSTRUCTION( mov_fr_w,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( nop, (), ())
	IMPLEMENT_INSTRUCTION( mov_special_option_w, (), ())
	IMPLEMENT_INSTRUCTION( retw,  (int arg_lit8),(arg_lit8))
	IMPLEMENT_INSTRUCTION( mov_w_shiftleft_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( rl_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( mov_w_shiftright_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( rr_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( sleep, (), ())
	IMPLEMENT_INSTRUCTION( mov_w_fr_minus_w,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( sub_fr_w,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( mov_w_swap_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( swap_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( mov_special_rx_w,  (int arg_cregister),(arg_cregister))
	IMPLEMENT_INSTRUCTION( xor_w_lit,  (int arg_lit8),(arg_lit8))
	IMPLEMENT_INSTRUCTION( xor_w_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( xor_fr_w,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( ret, (), ())
	IMPLEMENT_INSTRUCTION( retp, (), ())
	IMPLEMENT_INSTRUCTION( reti, (), ())
	IMPLEMENT_INSTRUCTION( retiw, (), ())
	IMPLEMENT_INSTRUCTION( page,  (int lit3),(lit3))
	IMPLEMENT_INSTRUCTION( bank,  (int lit3),(lit3))
	IMPLEMENT_INSTRUCTION( jmp_w,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( iread, (), ())
	IMPLEMENT_INSTRUCTION( mov_w_m, (), ())
	IMPLEMENT_INSTRUCTION( mov_m_w, (), ())
	IMPLEMENT_INSTRUCTION( mov_m_lit,  (int arg_lit4),(arg_lit4))
	IMPLEMENT_INSTRUCTION( jmp_pc_plus_w,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( clc, (), ())
	IMPLEMENT_INSTRUCTION( clz, (), ())
	IMPLEMENT_INSTRUCTION( skip, (), ())
	IMPLEMENT_INSTRUCTION( sc, (), ())
	IMPLEMENT_INSTRUCTION( not_w, (), ())

	void unknow_instruction( int instruction)
	{
		store_arg( instruction);
	}

private:
	
	void store_arg( int a1 = -1, int a2 = -1)
	{
		secret = a1;
	}
};
#undef IMPLEMENT_INSTRUCTION
#endif //SX_NOP_INCLUDED
