#if !defined( SX_CONTROLLER_INCLUDED)
#define SX_CONTROLLER_INCLUDED

#include <limits>
#include <exception>
#include <stack>
#include <iostream>

#include "arithmetic_with_flags.hpp"
#include "sx_memory.hpp"

namespace micro_emulator
{
	using arithmetic_with_flags::flagged;
	using binary_numbers::as_binary;

#define MEM ram( arg_register)

	struct recoverable_error : public std::runtime_error
	{
		recoverable_error( const char *what)
			: std::runtime_error( what)
		{
		};
	};

	struct stack_overflow_exception : public recoverable_error
	{
		stack_overflow_exception()
			: recoverable_error( "stack overflow")
		{
		}
	};

	struct reti_outside_interrupt_exception : public recoverable_error
	{
		reti_outside_interrupt_exception()
			: recoverable_error( "reti outside interrupt")
		{
		}
	};

	struct sx_controller : public sx_flags_definition
	{
		typedef sx_ram::address_t address_t;

		sx_controller()
			:real_w(0), w( real_w), m(0), wdt(0),
			pc_register( ram( sx_ram::PC)),
			flags( ram( sx_ram::STATUS)),
			in_interrupt(false),
			sleeping( false),
			nop_delay( 0)
		{
			set_pc( 0x7ff);
		}

		template< typename Range>
		void load_rom( const Range &r, sx_rom::address_t offset = 0)
		{
			rom.load( r, offset);
		}



		void set_pc( address_t new_pc)
		{
			program_counter = new_pc;
			update_pc();
		}

		address_t get_pc() const
		{
			return program_counter;
		}

		const sx_rom &get_rom() const
		{
			return rom;
		}

		void  add_w_fr(int arg_register)
		{
			flagged(w, flags) += MEM;
		}

		void  add_fr_w(int arg_register)
		{
			flagged( MEM, flags) += w;
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
				skip();
			}
		}

		void  sb_fr_bit(int arg_register, int bit)
		{
			if ((MEM&bitmask(bit)))
			{
				skip();
			}
		}

		void  call(int arg_addr8)
		{
			push( program_counter);
			set_pc( arg_addr8 | ((address_t( ram( sx_ram::STATUS)) & 0x00e0) << 4));


			if (stack.size() > sx_stack_size)
			{
				throw stack_overflow_exception();
			}
		}

		void  clr_fr(int arg_register)
		{
			flagged( MEM, flags) = 0;
		}

		void  clr_w()
		{
			flagged( w, flags) = 0;
		}

		void  mov_w_not_fr(int arg_register)
		{
			flagged( w, flags) = ~MEM;
		}

		void  not_fr(int arg_register)
		{
			flagged( MEM, flags) = ~MEM;
		}

		void  mov_w_dec_fr(int arg_register)
		{
			flagged( w, flags) = MEM - 1;
		}

		void  dec_fr(int arg_register)
		{
			--flagged( MEM, flags);
		}

		void  movsz_w_dec_fr(int arg_register)
		{
			if (!( w = MEM - 1))
			{
				skip();
			}
		}

		void  decsz_fr(int arg_register)
		{
			// no flags
			if (!( --MEM)) 
			{
				skip();
			}
		}

		void  jmp(int addr9_)
		{
			set_pc( addr9_ | ((address_t( ram( sx_ram::STATUS)) & 0x00e0) << 4));
		}

		void  mov_w_inc_fr(int arg_register)
		{
			flagged( w, flags) = MEM;
		}

		void  inc_fr(int arg_register)
		{
			++flagged( MEM, flags);
		}

		void  movsz_w_inc_fr(int arg_register)
		{
			if (!(w = MEM + 1))
			{
				skip();
			}
		}

		void  incsz_fr(int arg_register)
		{
			if (!(++MEM)) 
			{
				skip();
			}
		}

		void  or_w_lit(int arg_lit8)
		{
			flagged(w, flags) |= arg_lit8;
		}

		void  or_w_fr(int arg_register)
		{
			flagged(w, flags) |= MEM;
		}

		void  or_fr_w(int arg_register)
		{
			flagged( MEM, flags) |= w;
		}

		void  mov_w_fr(int arg_register)
		{
			flagged( w, flags) = MEM;
		}

		void  test_fr(int arg_register)
		{
			flagged( MEM, flags).test();
		}

		void  mov_w_lit(int arg_lit8)
		{
			w = arg_lit8;
		}

		void  mov_fr_w(int arg_register)
		{
			MEM = w; // no flags
		}

		void  retw(int arg_lit8)
		{
			// investigate:
			// set only bottom 8 bits with return address.
			w = arg_lit8;
			ret();
		}

		void  mov_w_shiftleft_fr(int arg_register)
		{
			register_t temp = MEM;
			w = flagged( temp, flags).rl();
		}

		void  rl_fr(int arg_register)
		{
			flagged( MEM, flags).rl();
		}

		void  mov_w_shiftright_fr(int arg_register)
		{
			register_t temp = MEM;
			w = flagged( temp, flags).rr();
		}

		void  rr_fr(int arg_register)
		{
			flagged( MEM, flags).rr();
		}

		void  mov_w_fr_minus_w(int arg_register)
		{
			register_t temp = w;
			w = (flagged( temp, flags) -= MEM);
		}

		void  sub_fr_w(int arg_register)
		{
			flagged( MEM, flags)-= w;
		}

		void  mov_w_swap_fr(int arg_register)
		{
			w = ((MEM & 0xf) >> 4) | ((MEM & 0x0f) << 4);
		}

		void  swap_fr(int arg_register)
		{
			MEM = ((MEM & 0xf) >> 4) | ((MEM & 0x0f) << 4);
		}

		void  mov_special_rx_w(int arg_cregister)
		{
			port_options[arg_cregister][m] = w;
		}

		void  xor_w_lit(int arg_lit8)
		{
			flagged( w, flags) ^= arg_lit8;
		}

		void  xor_w_fr(int arg_register)
		{
			flagged( w, flags) ^= MEM;
		}

		void  xor_fr_w(int arg_register)
		{
			flagged( MEM, flags) ^= w;
		}

		void  ret()
		{
			set_pc( pop());
			set_nop_delay( 3);
		}

		void  retp()
		{
			address_t return_address = pop();
			set_pc( return_address);
			// put bits 9-11 of the PC into bits 5-7 of status
			const address_t status_mask = as_binary< 11100000>();
			flags = (flags & ~status_mask) | ( (return_address >> 2) & status_mask);
			set_nop_delay( 3);
		}

		void  reti()
		{
			if (!in_interrupt)
			{
				throw reti_outside_interrupt_exception();
			}

			in_interrupt = false;
			program_counter = interrupt_state.program_counter;
		}

		void  retiw()
		{
			ram( sx_ram::RTCC) += w;
			reti();
		}

		void  page(int lit3)
		{
			ram(sx_ram::STATUS) = ram(sx_ram::STATUS) & 0x1f | lit3 << 5;
		}

		void  bank(int lit3)
		{
			ram.set_bank( lit3);
		}

		void  iread()
		{
			sx_rom::address_t word;
			word = (w + (sx_rom::address_t(m) << 8)) & 0xfff;
			word = rom( word);
			m = word >> 8; // assume only bits 8-11 are filled.
			w = word & 0xff;
			set_nop_delay( 3);
		}

		void  mov_w_m()
		{
			w = m;
		}

		void  mov_m_w()
		{
			m = w;
		}

		void  mov_m_lit(int arg_lit4)
		{
			m = arg_lit4;
		}

	private:

		typedef sx_ram::register_t register_t;
		typedef unsigned long time_counter_t;
		typedef std::stack< sx_rom::address_t> stack_t;
		struct stored_interrupt_state 
		{
			register_t program_counter;
			register_t w;
			register_t status;
			register_t fsr;
		};


		register_t port_options[5][32];

		static const size_t sx_stack_size = 8;
		//static const address_t PORTA = 0;
		//static const address_t PORTB = 0;
		//static const address_t PORTC = 0;

		// skip is not an assembly instruction
		void  skip()
		{
			++program_counter;
			update_pc();
		}

		void  sleep()
		{
			sleeping = true;
			set_nop_delay( std::numeric_limits< time_counter_t>::max());
		}

		int bitmask( int bit)
		{
			return (1<<bit);
		}


		//
		// some instructions take more than one cycle.
		// set a delay for such instructions
		//
		void set_nop_delay( time_counter_t delay)
		{
			nop_delay = delay;
		}

		void update_pc()
		{
			pc_register = program_counter;
		}

		address_t pop()
		{
			address_t value = stack.top();
			stack.pop();
			return value;
		}

		void push( address_t value)
		{
			stack.push( value);
		}

		register_t real_w;
		register_t &w;
		register_t m;
		register_t wdt;
		register_t &flags;
		bool in_interrupt;
		bool sleeping;
		unsigned long nop_delay;

		stored_interrupt_state interrupt_state;
		int program_counter; // program_counter is not a single register.
		register_t &pc_register;
		sx_ram ram;
		sx_rom rom;
		stack_t stack;
	};
}
#undef MEM
#undef IMPLEMENT_INSTRUCTION
#endif //SX_CONTROLLER_INCLUDED