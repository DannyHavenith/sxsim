
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// class that gathers all relevant SX state

#if !defined( SX_STATE_HPP)
#define SX_STATE_HPP

#include <deque>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/deque.hpp>
#include "sx_memory.hpp"

	struct stored_interrupt_state
	{
		sx_emulator::sx_rom::address_t pc;
		register_t w;
		register_t status;
		register_t fsr;

		template< typename Archive>
		void serialize( Archive &ar, unsigned int)
		{
		 	ar & BOOST_SERIALIZATION_NVP( w);
		 	ar & BOOST_SERIALIZATION_NVP( pc);
		 	ar & BOOST_SERIALIZATION_NVP( status);
		 	ar & BOOST_SERIALIZATION_NVP( fsr);
		}
	};




	struct sx_light_state
	{
		typedef sx_emulator::sx_ram::register_t register_type;
		typedef sx_emulator::sx_rom::address_t  address_type;
		// deque instead of stack to allow easy serialization
		typedef std::deque< address_type> stack_type;

		register_type m;
		register_type w;
		stack_type stack;
		unsigned short pc;
		bool in_interrupt;

		unsigned long cycle_counter;
	};

	struct sx_state : public sx_light_state
	{
		sx_emulator::sx_ram ram;
		stored_interrupt_state interrupt_state;

		template< typename Archive>
		void serialize( Archive &ar, unsigned int)
		{
		 	ar & BOOST_SERIALIZATION_NVP( m);
		 	ar & BOOST_SERIALIZATION_NVP( w);
		 	ar & BOOST_SERIALIZATION_NVP( stack);
		 	ar & BOOST_SERIALIZATION_NVP( pc);
		 	ar & BOOST_SERIALIZATION_NVP( in_interrupt);
		 	ar & BOOST_SERIALIZATION_NVP( interrupt_state);
		 	ar & BOOST_SERIALIZATION_NVP( cycle_counter);
		 	ar & BOOST_SERIALIZATION_NVP( ram);
		}
	};


#endif //SX_STATE_HPP
