
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// class that gathers all relevant SX state

#if !defined( SX_STATE_HPP)
#define SX_STATE_HPP

#include <stack>
#include "sx_memory.hpp"

	struct sx_light_state 
	{
		typedef sx_emulator::sx_ram::register_t register_type;
		typedef sx_emulator::sx_rom::address_t  address_type;
		typedef std::stack< address_type> stack_type;

		register_type m;
		register_type w;
		stack_type stack;
		unsigned short pc;
		bool in_interrupt;
	};

	struct sx_state : public sx_light_state
	{
		sx_emulator::sx_ram ram;
	};

#endif //SX_STATE_HPP