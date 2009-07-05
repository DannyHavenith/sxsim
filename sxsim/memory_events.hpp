#ifndef MEMORY_EVENTS_INCLUDED
#define MEMORY_EVENTS_INCLUDED
#include <bitset>
#include <boost/mpl/contains.hpp>
#include "sx_instruction_list.hpp" // for dest_reg_
#include "sx_memory.hpp"

namespace sx_emulator
{
	namespace mpl = boost::mpl;

	/// This class keeps an array of event handlers that are invoked when the sx changes 
	/// a memory location.
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

		/// the event dispatcher comes in two variations:
		/// - null dispatcher, which does nothing and is used for all instructions that 
		///   do not change memory locations at all (e.g. "mov w, fr")
		/// - real dispatcher, which calls the trigger method on the event dispatcher.
		template<typename tag_type, typename memory_changed = changes_memory<tag_type>::type>
		struct event_dispatcher
		{
			static void dispatch( const memory_events &, sx_ram::address_t) {};
		};

		// special case: this specialization is used when changes_memory< tag> is true.
		template<typename tag_type>
		struct event_dispatcher< tag_type, boost::mpl::true_>
		{
			static void dispatch( const memory_events &ev, sx_ram::address_t address)
			{
				ev.trigger( address);
			}
		};

		template< typename tag_type>
		void execute( const tag_type &)
		{
		}

		template< typename tag_type>
		void execute( const tag_type &, int arg1)
		{
			// this function call uses the fact that fr addresses
			// are  always in arg1. If this wasn't sure, we'd need some
			// meta-programming to determine the right argument from the
			// tag_type
			event_dispatcher< tag_type>::dispatch( *this, arg1);
		}


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
				// if so, expand the addres to a real internal address. 
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