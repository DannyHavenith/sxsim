#ifndef MEMORY_EVENTS_INCLUDED
#define MEMORY_EVENTS_INCLUDED
#include <bitset>
#include <boost/mpl/contains.hpp>
#include "sx_instruction_list.hpp" // for dest_reg_
#include "sx_memory.hpp"

namespace sx_emulator
{
	namespace mpl = boost::mpl;

	/// An implementation of an SX controller that does nothing more than fire memory events when ram is written to.
	/// This class is an SX controller implementation (that is, it implements an execute(...) method for all sx instruction tags),
	/// but the only thing that these execute implementations do is to call registered callbacks for those instructions that
	/// write to a RAM location (including the RA, RB, etc. registers).
	/// This implementation is intended as a mix-in with a 'real' implementation.
	/// Internally, object of this class contain an array with one callback for every ram location.
	class memory_events
	{
	public:

		typedef boost::function<void (sx_ram::address_t, sx_ram::register_t)> handler_type;

		memory_events( const sx_ram &ram_)
			:ram( ram_)
		{
		}

		/// meta function that returns true_ iff the arguments of an instruction
		/// include a dest_reg_ (or in other words: the instruction writes to a ram location).
		template< typename tag_type>
		struct changes_memory :
			mpl::contains<
				typename tag_type::args,
				dest_reg_
			> {};

		/// \brief This template is used to make a distinction between SX instructions that potentially change ram
		/// and those that don't.
		///
		/// the event dispatcher comes in two variations:
		/// - null dispatcher, which does nothing and is used for all instructions that
		///   do not change memory locations at all (e.g. "mov w, fr")
		/// - real dispatcher, which calls the trigger method on the event dispatcher.
		/// The primary template implements the null dispatcher. I.e. it does nothing.
		template<typename tag_type, typename memory_changed = typename changes_memory<tag_type>::type>
		struct event_dispatcher
		{
			static void dispatch( const memory_events &, sx_ram::address_t) {};
		};

		/// This specialization is used when changes_memory< tag> is true. It implements the event dispatching
		/// by merely calling the trigger method on the memory_events class. 
		template<typename tag_type>
		struct event_dispatcher< tag_type, boost::mpl::true_>
		{
			static void dispatch( const memory_events &ev, sx_ram::address_t address)
			{
				ev.trigger( address);
			}
		};

		/// Nullary operations never change ram, therefore this overload is empty
		template< typename tag_type>
		void execute( const tag_type &)
		{
		}

		/// Implementation for operations with one argument
		template< typename tag_type>
		void execute( const tag_type &, int arg1)
		{
			event_dispatcher< tag_type>::dispatch( *this, arg1);
		}


		/// Implementation for operations with two arguments
		template< typename tag_type>
		void execute( const tag_type &, int arg1, int)
		{
			// this function call uses the fact that fr addresses
			// are  always in arg1. If this wasn't sure, we'd need some
			// meta-programming to determine the right argument from the
			// tag_type
			event_dispatcher< tag_type>::dispatch( *this, arg1);
		}

		/// called by the dispatcher: trigger an event for this address, if a handler was registered.
		void trigger( sx_ram::address_t address) const
		{
			// first, do a quick test to see if there are handlers for this limited (0x00-0x1f) addres
			if (handler_flags[ address])
			{
				// if so, expand the addres to a real internal address and check again.
				sx_ram::address_t internal_address = ram.limited_to_internal( address);
				if (handlers[internal_address])
				{
					handlers[internal_address]( address, ram.get_internal(internal_address));
				}
			}
		}

		/// register a memory event handler
		void set( sx_ram::address_t address, handler_type handler)
		{
			if (address >= 0 && address < sx_ram::memory_size)
			{
				handlers[ address] = handler;
				update_handler_flags();
			}
		}

	private:

		/// make sure that the handler flags are consistent with the handlers
		void update_handler_flags()
		{
			std::fill( handler_flags, handler_flags + 32, false);
			for (int count = 0; count < sx_ram::memory_size; ++count)
			{
				if (handlers[count])
				{
					handler_flags[0] = true;
					handler_flags[ count & 0x1f] = true;
				}
			}
		}

		bool handler_flags[32];
		handler_type handlers[ sx_ram::memory_size];
		const sx_ram &ram;
	};
}
#endif //MEMORY_EVENTS_INCLUDED
