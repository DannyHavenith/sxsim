#if !defined( SX_CONTROLLER_INCLUDED)
#define SX_CONTROLLER_INCLUDED

#include "arithmetic_with_flags.hpp"
#include "sx_memory.hpp"

namespace micro_emulator
{
	using arithmetic_with_flags::flagged;

#define MEM memory( arg_register)

	struct sx_controller
	{
		sx_controller()
			:real_w(0), w( real_w), 
			pc_register( memory( sx_memory::PC)),
			flags( memory( sx_memory::STATUS))
		{
		}

		void  add_w_fr(int arg_register)
		{
			flagged<>(w, flags) += MEM;
		}

		void  add_fr_w(int arg_register)
		{
			flagged<>( MEM, flags) += w;
		}

		void  and_w_lit(int arg_lit8)
		{
			w &= arg_lit8;
		}

		void  and_w_fr(int arg_register)
		{
			w &= MEM; 
		}

		void  and_fr_w(int arg_register)
		{
			MEM &= w;
		}

		void  clrb_fr_bit(int arg_register, int bit)
		{
			MEM &= ~bitmask(bit);
		}


		void  setb_fr_bit(int arg_register, int bit)
		{
			MEM |= bitmask(bit);
		}

		void  snb_fr_bit(int arg_register, int bit)
		{
			if (!(MEM&bitmask(bit)))
			{
				++program_counter;
				update_pc();
			}
		}

		void  sb_fr_bit(int arg_register, int bit)
		{
			if ((MEM&bitmask(bit)))
			{
				++program_counter;
				update_pc();
			}
		}

		void  call(int arg_addr8)
		{
		}

		void  clr_fr(int arg_register)
		{
		}

		void  clr_w()
		{
		}

		void  clr_special_wdt()
		{
		}

		void  mov_w_not_fr(int arg_register)
		{
		}

		void  not_fr(int arg_register)
		{
		}

		void  mov_w_dec_fr(int arg_register)
		{
		}

		void  dec_fr(int arg_register)
		{
			--flagged<>( MEM, flags);
		}

		void  movsz_w_dec_fr(int arg_register)
		{
		}

		void  decsz_fr(int arg_register)
		{
			if (!(--flagged<>( MEM, flags))) 
			{
				++program_counter;
				update_pc();
			}
		}

		void  jmp(int addr9_)
		{
			program_counter = addr9_;
			update_pc();
		}

		void  mov_w_inc_fr(int arg_register)
		{
		}

		void  inc_fr(int arg_register)
		{
		}

		void  movsz_w_inc_fr(int arg_register)
		{
		}

		void  incsz_fr(int arg_register)
		{
		}

		void  or_w_lit(int arg_lit8)
		{
		}

		void  or_w_fr(int arg_register)
		{
		}

		void  or_fr_w(int arg_register)
		{
		}

		void  mov_w_fr(int arg_register)
		{
		}

		void  test_fr(int arg_register)
		{
			flagged<>( MEM, flags).test();
		}

		void  mov_w_lit(int arg_lit8)
		{
			w = arg_lit8;
		}

		void  mov_fr_w(int arg_register)
		{
			MEM = w;
		}

		void  nop()
		{
		}

		void  mov_special_option_w()
		{
		}

		void  retw(int arg_lit8)
		{
		}

		void  mov_w_shiftleft_fr(int arg_register)
		{
		}

		void  rl_fr(int arg_register)
		{
			flagged<>( MEM, flags).rl();
		}

		void  mov_w_shiftright_fr(int arg_register)
		{
		}

		void  rr_fr(int arg_register)
		{
			flagged<>( MEM, flags).rr();
		}

		void  sleep()
		{
		}

		void  mov_w_fr_minus_w(int arg_register)
		{
		}

		void  sub_fr_w(int arg_register)
		{
		}

		void  mov_w_swap_fr(int arg_register)
		{
		}

		void  swap_fr(int arg_register)
		{
		}

		void  mov_special_rx_w(int arg_cregister)
		{
		}

		void  xor_w_lit(int arg_lit8)
		{
		}

		void  xor_w_fr(int arg_register)
		{
		}

		void  xor_fr_w(int arg_register)
		{
		}

		void  ret()
		{
		}

		void  retp()
		{
		}

		void  reti()
		{
		}

		void  retiw()
		{
		}

		void  page(int lit3)
		{
		}

		void  bank(int lit3)
		{
			memory.set_bank( lit3);
		}

		void  jmp_w(int arg_register)
		{
		}

		void  iread()
		{
		}

		void  mov_w_m()
		{
		}

		void  mov_m_w()
		{
		}

		void  mov_m_lit(int arg_lit4)
		{
		}

		void  jmp_pc_plus_w(int arg_register)
		{
		}

		void  clc()
		{
		}

		void  clz()
		{
		}

		void  skip()
		{
		}

		void  sc()
		{
		}

		void  not_w()
		{
		}

	private:
		typedef sx_memory::register_t register_t;

		int bitmask( int bit)
		{
			return (1<<bit);
		}

		register_t real_w;
		register_t &w;
		register_t &flags;

		int program_counter; // program_counter is not a single register.
		register_t &pc_register;

		void update_pc()
		{
			pc_register = program_counter;
		}

		sx_memory memory;
	};
}
#undef MEM
#undef IMPLEMENT_INSTRUCTION
#endif //SX_CONTROLLER_INCLUDED