
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// This file contains the implementation of the actual SX instruction set.

#if !defined( SX_CONTROLLER_INCLUDED)
#define SX_CONTROLLER_INCLUDED

#include <limits>
#include <exception>
#include <stdexcept>
#include <stack>
#include <iostream>
#include <boost/utility.hpp>
#include <boost/bind/placeholders.hpp>

#include "arithmetic_with_flags.hpp"
#include "sx_memory.hpp"
#include "sx_state.hpp"
#include "memory_events.hpp"
#include "sx_compiler.hpp"
#include "sx_instruction_list.hpp"
#include "instruction_decoder.hpp"

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

		unsigned long prescale_mask( unsigned char val)
		{
			return 255 >> (7 - val);
		}
	}

	using arithmetic_with_flags::flagged;
	using binary_numbers::as_binary;

#define MEM ram( arg_register)

	///
	/// base class for all recoverable exceptions thrown by the emulator
	struct recoverable_error : public std::runtime_error
	{
		recoverable_error( const char *what, unsigned short program_counter)
			: std::runtime_error( what),
			pc( program_counter)
		{
		};

		unsigned short pc;
	};

	/// exception that is thrown when the call depth exceeds the microcontrollers limits
	struct stack_overflow_exception : public recoverable_error
	{
		stack_overflow_exception( unsigned short pc)
			: recoverable_error( "stack overflow", pc)
		{
		}
	};

	/// exception that is thrown when a ret, retw instruction is encountered with no return address on the stack
	struct stack_underflow_exception : public recoverable_error
	{
		stack_underflow_exception(unsigned short pc)
			: recoverable_error( "stack underflow", pc)
		{
		}
	};

	/// exception that is thrown when an reti or retiw instruction is encountered outside an interrupt service routine
	struct reti_outside_interrupt_exception : public recoverable_error
	{
		reti_outside_interrupt_exception(unsigned short pc)
			: recoverable_error( "reti outside interrupt", pc)
		{
		}
	};

	/// exception that is thrown when an rtcc interrupt occurs while still processing the previous one.
	struct rtcc_overflow_exception : public recoverable_error
	{
		rtcc_overflow_exception( unsigned short pc)
			: recoverable_error( "rtcc interrupt while in interrupt", pc)
		{
		}

	};

	//
	/// This class implements the actual sx instruction set.
	using namespace micro_emulator;
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
			throw_on_rtcc_overflow( true),
			enable_rtcc_interrupt(false),
			rtcc_on_cycle( false),
			rtcc_prescale_mask( 255),
			cycle_counter( 0)
		{
			reset();
		}

		void reset()
		{
			set_pc( 0x7ff);
			ram( sx_ram::STATUS) = ram( sx_ram::STATUS) & 0x1f;
			m = 0x0f;
			in_interrupt = false;
			stack = stack_t();
			cycle_counter = 0;
		}

		sx_state get_state() const
		{
			sx_state s;
			s.ram = ram;
			s.w = w;
			s.m = m;
			s.in_interrupt = in_interrupt;
			s.pc = get_pc();
			s.stack = stack;

			return s;
		}

		void set_state( const sx_state &s)
		{
			ram = s.ram;
			w = s.w;
			m = s.m;
			in_interrupt = s.in_interrupt;
			set_pc( s.pc);
			stack = s.stack;
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

		bool get_rtcc_on_cycle() const
		{
			return rtcc_on_cycle;
		}

		void do_rtcc()
		{
			if (!(++cycle_counter & rtcc_prescale_mask))
			{
				if (++ram( sx_ram::RTCC) == 0 && enable_rtcc_interrupt)
				{
					do_interrupt();
				}
			}
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
			else
			{
				if (throw_on_rtcc_overflow)
				{
					throw rtcc_overflow_exception(get_pc());
				}
			}
		}

		void execute( const add_w_fr &, int arg_register)
		{
			flagged(w, flags) += MEM;
		}

		void execute( const add_fr_w &, int arg_register)
		{
			// TODO: find out if wrap-around occurs at page boundaries
			if (arg_register == sx_ram::PC)
			{
				program_counter += w;
			}
			flagged( MEM, flags) += w;
		}

		void execute( const and_w_lit &, int arg_lit8)
		{
			flagged( w, flags) &= arg_lit8;
		}

		void execute( const and_w_fr &, int arg_register)
		{
			flagged( w, flags) &= MEM;
		}

		void execute( const and_fr_w &, int arg_register)
		{
			flagged( MEM, flags) &= w;
		}

		void execute( const clrb_fr_bit &, int arg_register, int bit)
		{
			MEM &= ~bitmask(bit);
		}

		void execute( const setb_fr_bit &, int arg_register, int bit)
		{
			MEM |= bitmask(bit);
		}

		void execute( const snb_fr_bit &, int arg_register, int bit)
		{
			if (!(MEM&bitmask(bit)))
			{
				skip();
			}
		}

		void execute( const sb_fr_bit &, int arg_register, int bit)
		{
			if ((MEM&bitmask(bit)))
			{
				skip();
			}
		}

		void execute( const call &, int arg_addr8)
		{
			push( program_counter);
			set_pc( arg_addr8 | ((address_t( ram( sx_ram::STATUS)) & 0x00e0) << 4));

			if (stack.size() > sx_stack_size)
			{
				throw stack_overflow_exception( stack.top());
			}
		}

		void execute( const clr_fr &, int arg_register)
		{
			flagged( MEM, flags) = 0;
		}

		void execute( const clr_w &)
		{
			flagged( w, flags) = 0;
		}

		void execute( const mov_w_not_fr &, int arg_register)
		{
			flagged( w, flags) = ~MEM;
		}

		void execute( const not_fr &, int arg_register)
		{
			flagged( MEM, flags) = ~MEM;
		}

		void execute( const mov_w_dec_fr &, int arg_register)
		{
			flagged( w, flags) = MEM - 1;
		}

		void execute( const dec_fr &, int arg_register)
		{
			--flagged( MEM, flags);
		}

		void execute( const movsz_w_dec_fr &, int arg_register)
		{
			if (!( w = MEM - 1))
			{
				skip();
			}
		}

		void execute( const decsz_fr &, int arg_register)
		{
			// no flags
			if (!( --MEM))
			{
				skip();
			}
		}

		void execute( const jmp &, int addr9_)
		{
			set_pc( addr9_ | ((address_t( ram( sx_ram::STATUS)) & 0x00e0) << 4));
		}

		void execute( const mov_w_inc_fr &, int arg_register)
		{
			flagged( w, flags) = MEM + 1;
		}

		void execute( const inc_fr &, int arg_register)
		{
			++flagged( MEM, flags);
		}

		void execute( const movsz_w_inc_fr &, int arg_register)
		{
			if (!(w = MEM + 1))
			{
				skip();
			}
		}

		void execute( const incsz_fr &, int arg_register)
		{
			if (!(++MEM))
			{
				skip();
			}
		}

		void execute( const or_w_lit &, int arg_lit8)
		{
			flagged(w, flags) |= arg_lit8;
		}

		void execute( const or_w_fr &, int arg_register)
		{
			flagged(w, flags) |= MEM;
		}

		void execute( const or_fr_w &, int arg_register)
		{
			flagged( MEM, flags) |= w;
		}

		void execute( const mov_w_fr &, int arg_register)
		{
			flagged( w, flags) = MEM;
		}

		void execute( const test_fr &, int arg_register)
		{
			flagged( MEM, flags).test();
		}

		void execute( const mov_w_lit &, int arg_lit8)
		{
			w = arg_lit8;
		}

		void execute( const mov_fr_w &, int arg_register)
		{
			MEM = w; // no flags
			if (arg_register == sx_ram::PC)
			{
				execute( jmp(), w);
			}
		}

		void execute( const retw &, int arg_lit8)
		{
			// investigate:
			// set only bottom 8 bits with return address.
			w = arg_lit8;
			execute( ret());
		}

		void execute( const mov_w_shiftleft_fr &, int arg_register)
		{
			register_t temp = MEM;
			w = flagged( temp, flags).rl();
		}

		void execute( const rl_fr &, int arg_register)
		{
			flagged( MEM, flags).rl();
		}

		void execute( const mov_w_shiftright_fr &, int arg_register)
		{
			register_t temp = MEM;
			w = flagged( temp, flags).rr();
		}

		void execute( const rr_fr &, int arg_register)
		{
			flagged( MEM, flags).rr();
		}

		void execute( const mov_w_fr_minus_w &, int arg_register)
		{
			register_t temp = MEM;
			w = (flagged( temp, flags) -= w);
		}

		void execute( const sub_fr_w &, int arg_register)
		{
			flagged( MEM, flags)-= w;
		}

		void execute( const mov_w_swap_fr &, int arg_register)
		{
			w = ((MEM & 0xf0) >> 4) | ((MEM & 0x0f) << 4);
		}

		void execute( const swap_fr &, int arg_register)
		{
			MEM = ((MEM & 0xf0) >> 4) | ((MEM & 0x0f) << 4);
		}

		void execute( const mov_special_rx_w &, int arg_cregister)
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

		void execute( const xor_w_lit &, int arg_lit8)
		{
			flagged( w, flags) ^= arg_lit8;
		}

		void execute( const xor_w_fr &, int arg_register)
		{
			flagged( w, flags) ^= MEM;
		}

		void execute( const xor_fr_w &, int arg_register)
		{
			flagged( MEM, flags) ^= w;
		}

		void execute( const ret &)
		{
			if (stack.empty())
			{
				throw stack_underflow_exception( get_pc());
			}
			set_pc( pop());
			set_nop_delay( 3);
		}

		void execute( const retp &)
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

		void execute( const reti &)
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

		void execute( const retiw &)
		{
			ram( sx_ram::RTCC) += w;
			execute( reti() );
		}

		void execute( const page &, int lit3)
		{
			ram(sx_ram::STATUS) = ram(sx_ram::STATUS) & 0x1f | lit3 << 5;
		}

		void execute( const bank &, int lit3)
		{
			ram.set_bank( lit3);
		}

		void execute( const iread &)
		{
			sx_rom::address_t word;
			word = (w + (sx_rom::address_t(m) << 8)) & 0xfff;
			word = rom( word);
			m = word >> 8; // assume only bits 8-11 are filled.
			w = word & 0xff;
			set_nop_delay( 3);
		}

		void execute( const mov_w_m &)
		{
			w = m;
		}

		void execute( const mov_m_w &)
		{
			m = w;
		}

		void execute( const mov_m_lit &, int arg_lit4)
		{
			m = arg_lit4;
		}

		void execute( const breakpoint &)
		{
			// do nothing. others do sane things here.
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
			// if we're at a page instruction, perform one extra skip
			if ((get_rom()( program_counter) & 0xff8) == 0x010)
			{
				++program_counter;
				set_nop_delay( 1);
			}

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
			// set bits TO and PD
			ram( sx_ram::STATUS) |= 0x18;
		}

		void set_option( int value)
		{
			// todo: implement bits 7,
			enable_rtcc_interrupt = (value & (1<<6)) == 0;
			rtcc_on_cycle = (value & (1<<5)) == 0;
			if ((value & (1<<3)) == 0)
			{
				rtcc_prescale_mask = prescale_mask( value & 0x07);
			}
			else
			{
				rtcc_prescale_mask = 0;
			}
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
		bool throw_on_rtcc_overflow;
		unsigned long nop_delay;

		bool enable_rtcc_interrupt;
		bool rtcc_on_cycle;
		unsigned long rtcc_prescale_mask;
		unsigned long cycle_counter;

		stored_interrupt_state interrupt_state;
		int program_counter; // program_counter is not a single register.
		register_t &pc_register; // ...but is reflected in ram...
		sx_ram ram;
		sx_rom rom;
		stack_t stack;
	};

	class sx_controller : public sx_controller_impl, private memory_events
	{
		typedef micro_emulator::instruction_decoder<
			sx_instruction_list,
			sx_controller
		> decoder_t;

		static const sx_rom::register_t BREAKPOINT = 0x4f;
		sx_rom shadow_rom;


	public:
		typedef unsigned long histogram_type[ sx_rom::memory_size];

		sx_controller()
			: memory_events( sx_controller_impl::get_ram())
		{
		}

		/// execute a zero operand opcode
		template< typename tag>
		void execute( const tag &)
		{
			sx_controller_impl::execute( tag());
			memory_events::execute( tag());
		}

		/// execute a single operand opcode
		template< typename tag>
		void execute( const tag &, int arg1)
		{
			sx_controller_impl::execute( tag(), arg1);
			memory_events::execute( tag(), arg1);
		}

		/// executa a three operand opcode.
		template< typename tag>
		void execute( const tag &, int arg1, int arg2)
		{
			sx_controller_impl::execute( tag(), arg1, arg2);
			memory_events::execute( tag(), arg1, arg2);
		}

		/// get profiling information: a count of how often each instruction was run
		const histogram_type &get_histogram() const
		{
			return histogram;
		}

		/// set all statistics to zero
		void reset_histogram()
		{
			std::fill( histogram, histogram + (sizeof( histogram)/sizeof( histogram[0])), 0);
		}

		/// register a handler that will be called if a certain ram address is accessed.
		void on_memory_access( sx_ram::address_t address, memory_events::handler_type handler)
		{
			memory_events::set( address, handler);
		}

		/// load a range of instruction words into rom.
		template< typename Range>
		void load_rom( const Range &r, sx_rom::address_t offset = 0)
		{
			sx_controller_impl::load_rom( r, offset);
			shadow_rom.load( r, offset);
		}

		/// set a breakpoint at a given address
		void set_breakpoint( address_t address)
		{
			shadow_rom.set( address, BREAKPOINT);
		}

		/// remove the breakpoint at the given address
		void remove_breakpoint( address_t address)
		{
			shadow_rom.set( address, get_rom()( address));
		}

		/// keep track of how often a certain instruction was run
		address_t count_freq( address_t address)
		{
			++histogram[ address];
			return address;
		}

		/// run one clock cycle/instruction
		size_t tick()
		{
			//
			// handle realtime clock, if enabled.
			if (get_rtcc_on_cycle())
				do_rtcc();

			reset_nop_delay();
			sx_rom::register_t instruction = get_rom()( count_freq(inc_pc()));
			decoder_t::feed(
				instruction, *this);

			return 0;
		}


		/// tick variant that is to be called from some external clock
		/// returns true if a breakpoint was encountered
		bool synchronized_tick()
		{
			//
			// handle realtime clock, if enabled.
			if (get_rtcc_on_cycle()) do_rtcc();

			if (!dec_nop_delay())
			{
				// notice the count_freq call, this costs about 3% in performance,
				// but it delivers a nice profiling feature in return.
				sx_rom::register_t instruction = shadow_rom( count_freq(inc_pc()));
				if (instruction == BREAKPOINT)
				{
					dec_pc();
					return true;
				}

				decoder_t::feed(
					instruction, *this
					);
			}
			return false;
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
					// the body of this function is essentially a copy of
					// the synchronized_tick function, but calling that function
					// instead of just copying the contents
					// costs us about 25% performance on msvc (9)
					//

					// handle realtime clock, if enabled.
					if (get_rtcc_on_cycle()) do_rtcc();

					if (!dec_nop_delay())
					{
						// notice the count_freq call, this costs about 3% in execution time,
						// but it delivers a nice profiling feature in return.
						sx_rom::register_t instruction = shadow_rom( count_freq(inc_pc()));
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

	private:
		// keep a count of how often each instruction was run
		histogram_type histogram;

	};

	//
	// todo: refactor sx_controller into a non-leaf class.
	class precompiled_sx_controller : public sx_controller
	{

	public:
		template< typename Range>
		void load_rom( const Range &r, sx_rom::address_t offset = 0)
		{
			sx_controller_impl::load_rom( r, offset);
			compile();
		}

		bool synchronized_tick()
		{
			try
			{
				//
				// handle realtime clock, if enabled.
				if (get_rtcc_on_cycle()) do_rtcc();
				if (!dec_nop_delay())
				{
					precompiled[ count_freq(inc_pc())]->execute( this);
				}
			}
			catch( const breakpoint_exception &)
			{
				return false;
			}

			return true;
		}

		size_t tick()
		{
			return sx_controller::tick();
		}
		//
		// there are a few subtle differences between 'tick' and 'ticks'.
		// The former always executes an instruction, whereas 'ticks' may break on
		// breakpoints.
		// That is also why this function retrieves instructions from the shadow rom,
		// which may contain breakpoints.
		//
		size_t tick( size_t count)
		{
			try
			{
				// first instruction is always executed
				if (count)
				{
					sx_controller::tick();

					// other instructions may be breakpoints.
					while (--count)
					{
						//
						// handle realtime clock, if enabled.
						if (get_rtcc_on_cycle()) do_rtcc();
						if (!dec_nop_delay())
						{
							precompiled[ count_freq(inc_pc())]->execute( this);
						}
					}
				}
			}
			catch( const breakpoint_exception &)
			{
			}
			return count;
		}

		/// set a breakpoint at a given address
		void set_breakpoint( address_t address)
		{
			precompiled[address].reset( new ins_notag<precompiled_sx_controller>( &precompiled_sx_controller::throw_breakpoint));
		}

		/// remove the breakpoint at the given address
		void remove_breakpoint( address_t address)
		{
			compile( address, get_rom()(address));
		}

	private:
		typedef sx_compiler< precompiled_sx_controller> compiler_type;

		void throw_breakpoint()
		{
			throw breakpoint_exception();
		}

		/// compile an sx instruction into a function pointer
		void compile( address_t address, sx_rom::register_t instruction)
		{
			typedef micro_emulator::instruction_decoder<
				sx_instruction_list,
				compiler_type
			> compiler_decoder_type;

			compiler_type c( precompiled[ address]);
			compiler_decoder_type::feed( instruction, c);
		}

		/// compile all rom instructions into function pointers.
		void compile()
		{
			for (address_t address = 0; address < sx_rom::memory_size; ++address)
			{
				compile( address, get_rom()( address));
			}
		}

		compiler_type::slot_type precompiled[ sx_emulator::sx_rom::memory_size];
		struct breakpoint_exception {};

	};
}

#undef MEM
#undef IMPLEMENT_INSTRUCTION
#endif //SX_CONTROLLER_INCLUDED
