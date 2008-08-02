#if !defined( SX_CONTROLLER_INCLUDED)
#define SX_CONTROLLER_INCLUDED

#include <limits>
#include <exception>
#include <stack>
#include <iostream>
#include <boost/utility.hpp>

#include "arithmetic_with_flags.hpp"
#include "sx_memory.hpp"
#include "sx_state.hpp"

namespace sx_emulator
{
	namespace // unnamed
	{
		//
		// very dedicated function to translate option register values 
		// to rtcc prescaler values.
		unsigned short quick_pow2( unsigned char val)
		{
			switch (val)
			{
			case 0: return 2; // actually pow2( val + 1)...
			case 1: return 4;
			case 2: return 8;
			case 3: return 16;
			case 4: return 32;
			case 5: return 64;
			case 6: return 128;
			case 7: return 256;
			default:
				throw std::logic_error( "unexpected value for quick_pow2");
				break;
			};
		}
	}

	using arithmetic_with_flags::flagged;
	using binary_numbers::as_binary;

#define MEM ram( arg_register)

	struct recoverable_error : public std::runtime_error
	{
		recoverable_error( const char *what, unsigned short program_counter)
			: std::runtime_error( what),
			pc( program_counter)
		{
		};

		unsigned short pc;
	};

	struct stack_overflow_exception : public recoverable_error
	{
		stack_overflow_exception( unsigned short pc)
			: recoverable_error( "stack overflow", pc)
		{
		}
	};

	struct stack_underflow_exception : public recoverable_error
	{
		stack_underflow_exception(unsigned short pc)
			: recoverable_error( "stack underflow", pc)
		{
		}
	};

	struct reti_outside_interrupt_exception : public recoverable_error
	{
		reti_outside_interrupt_exception(unsigned short pc)
			: recoverable_error( "reti outside interrupt", pc)
		{
		}
	};

	struct sx_controller_impl :  public sx_flags_definition, public boost::noncopyable
	{
		typedef sx_ram::address_t address_t;

		sx_controller_impl()
			:real_w(0), w( real_w), m(0), wdt(0),
			pc_register( ram( sx_ram::PC)),
			flags( ram( sx_ram::STATUS)),
			in_interrupt(false),
			sleeping( false),
			nop_delay( 0),
			enable_rtcc_interrupt(false),
			rtcc_on_cycle( false),
			rtcc_prescale( 256),
			rtcc_prescale_counter( 256)
		{
			reset();
		}

		void reset()
		{
			set_pc( 0x7ff);
			ram( sx_ram::STATUS) = 0x0f;
			m = 0x0f;
		}


		sx_state get_state() const
		{
			sx_state s;
			s.ram = ram;
			s.w = w;
			s.m = m;
			s.in_interrupt = in_interrupt;
			s.pc = get_pc();

			return s;
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

		address_t inc_pc()
		{
			address_t ret = program_counter++;
			update_pc();
			return ret;
		}

		void dec_pc()
		{
			--program_counter;
			update_pc();
		}

		const sx_rom &get_rom() const
		{
			return rom;
		}

		const sx_ram &get_ram() const
		{
			return ram;
		}

		bool get_rtcc_on_cycle()
		{
			return rtcc_on_cycle;
		}

		void do_rtcc()
		{
			if (!(--rtcc_prescale_counter))
			{
				rtcc_prescale_counter = rtcc_prescale;
				if (++ram( sx_ram::RTCC) == 0 && enable_rtcc_interrupt)
				{
					do_interrupt();
				}
			}
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
				throw stack_overflow_exception( stack.top());
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
			switch (arg_cregister)
			{
			case 2:
				port_options[arg_cregister][0] = w;
				set_option( w);
				break;
			case 3: 
				sleep();
				break;
			case 4:
				clear_wdt();
				break;
			case 0:
			case 1:
			case 5:
			case 6:
			case 7:
				port_options[arg_cregister][m] = w;
				break;
			};
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
			if (stack.empty())
			{
				throw stack_underflow_exception( get_pc());
			}
			set_pc( pop());
			set_nop_delay( 3);
		}

		void  retp()
		{
			if (stack.empty())
			{
				throw stack_underflow_exception( get_pc());
			}
			address_t return_address = pop();
			set_pc( return_address);
			// put bits 9-11 of the PC into bits 5-7 of status
			const address_t status_mask = as_binary< 11100000>();
			flags = (flags & ~status_mask) | ( (return_address >> 4) & status_mask);
			set_nop_delay( 3);
		}

		void  reti()
		{
			if (!in_interrupt)
			{
				throw reti_outside_interrupt_exception(get_pc());
			}

			in_interrupt = false;
			set_pc( interrupt_state.program_counter);
			w = interrupt_state.w;
			ram( sx_ram::FSR) = interrupt_state.fsr;
			ram( sx_ram::STATUS) = interrupt_state.status;
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

		void do_interrupt()
		{
			if (!in_interrupt)
			{
				in_interrupt = true;
				interrupt_state.program_counter = program_counter;
				interrupt_state.w = w;
				interrupt_state.fsr = ram( sx_ram::FSR);
				interrupt_state.status = ram( sx_ram::STATUS);
				ram( sx_ram::STATUS) &= 0x1f; // clear page bits.
				set_pc(0);
				set_nop_delay( 3);
			}
		}


	protected:
		typedef unsigned long time_counter_t;

		time_counter_t dec_nop_delay()
		{
			if (nop_delay)
			{
				return nop_delay--;
			}
			else
			{
				return nop_delay;
			}
		}

		void reset_nop_delay()
		{
			nop_delay = 0;
		}

	private:

		typedef sx_ram::register_t register_t;
		typedef std::stack< sx_rom::address_t> stack_t;
		struct stored_interrupt_state 
		{
			sx_rom::address_t program_counter;
			register_t w;
			register_t status;
			register_t fsr;
		};


		register_t port_options[8][32];

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

		void clear_wdt()
		{
			wdt = 0;
			// todo: actions on clear wdt
		}

		void set_option( int value)
		{
			// todo: implement bits 7, 
			enable_rtcc_interrupt = (value & (1<<6)) == 0;
			rtcc_on_cycle = (value & (1<<5)) == 0;
			if ((value & (1<<3)) == 0)
			{
				rtcc_prescale = quick_pow2( value & 0x07);
			}
			else
			{
				rtcc_prescale = 1;
			}
			rtcc_prescale_counter = rtcc_prescale;
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

		bool enable_rtcc_interrupt;
		bool rtcc_on_cycle;
		short rtcc_prescale;
		short rtcc_prescale_counter;

		stored_interrupt_state interrupt_state;
		int program_counter; // program_counter is not a single register.
		register_t &pc_register; // ...but is reflected in ram...
		sx_ram ram;
		sx_rom rom;
		stack_t stack;
	};

	class sx_controller : public sx_controller_impl
	{
		typedef micro_emulator::instruction_decoder<
			micro_emulator::sx_instruction_list< 
			sx_emulator::sx_controller_impl
			>
		> decoder_t;

		static const sx_rom::register_t BREAKPOINT = 0x4f;
		sx_rom shadow_rom;


	public:

		template< typename Range>
		void load_rom( const Range &r, sx_rom::address_t offset = 0)
		{
			sx_controller_impl::load_rom( r, offset);
			shadow_rom.load( r, offset);
		}

		void set_breakpoint( address_t address)
		{
			shadow_rom.set( address, BREAKPOINT);
		}

		void remove_breakpoint( address_t address)
		{
			shadow_rom.set( address, get_rom()( address));
		}

		size_t tick()
		{
			//
			// handle realtime clock, if enabled.
			if (get_rtcc_on_cycle()) do_rtcc();

			reset_nop_delay();
			sx_rom::register_t instruction = get_rom()( inc_pc());
			decoder_t::feed( 
				instruction, *this);

			return 0;
		}

		//
		// there are a few subtle differences between 'tick' and 'ticks'.
		// The former always executes an instructon, whereas 'ticks' may break on 
		// breakpoints.
		// That is also why this function retrieves instructions from the shadow rom, 
		// which may contain breakpoints.
		//
		size_t tick( size_t count)
		{
			// first instruction is always executed
			if (count)
			{
				tick();

				// other instructions may be breakpoints.
				while (--count)
				{

					//
					// handle realtime clock, if enabled.
					if (get_rtcc_on_cycle()) do_rtcc();



					if (!dec_nop_delay())
					{
						sx_rom::register_t instruction = shadow_rom( inc_pc());
						if (instruction == BREAKPOINT)
						{
							dec_pc();
							break;
						}

						decoder_t::feed( 
							instruction, *this
							);
					}
				}
			}

			return count;
		}
	};
}
#undef MEM
#undef IMPLEMENT_INSTRUCTION
#endif //SX_CONTROLLER_INCLUDED
