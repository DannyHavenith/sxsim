#if !defined( SX_PRINT_INCLUDED)
#define SX_PRINT_INCLUDED

#define IMPLEMENT_INSTRUCTION( fname, args_in, args_out) \
	void fname args_in	\
	{											\
		instruction_printer(#fname).print args_out;	\
	}

struct sx_print
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
	IMPLEMENT_INSTRUCTION( mov_w_minus_minus_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( dec_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( movsz_w_minus_minus_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( decsz_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( jmp,  (int addr9_),(addr9_))
	IMPLEMENT_INSTRUCTION( mov_w_plus_plus_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( inc_fr,  (int arg_register),(arg_register))
	IMPLEMENT_INSTRUCTION( movsz_w_plus_plus_fr,  (int arg_register),(arg_register))
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
		instruction_printer( "unknown_instruction").print( instruction);
	}

private:
	struct instruction_printer
	{
		instruction_printer( const char *instruction)
			: name( instruction)
		{}

		void print( int arg1 = -1 , int arg2 = -1)
		{
			std::cout << name << " ";
			if (arg1 != -1)
			{
				std::cout << arg1 << " ";
				if (arg2 != -1)
				{
					std::cout << arg2;
				}
			}
			std::cout << std::endl;
		}
	private:
		const char * const name;
	};
};

#endif //SX_PRINT_INCLUDED