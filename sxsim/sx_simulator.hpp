
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// implementation of a wrapper around the real sx emulator.

#if !defined( SX_SIMULATOR_HPP)
#define SX_SIMULATOR_HPP
#include <vector>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "sx_memory.hpp"
#include "sx_state.hpp"

namespace sx_emulator
{
	// pimpl
	class sx_controller; 
	class precompiled_sx_controller;
}

class sx_simulator
{
public:
	static const int rom_size = 4096;

	// uncomment only one of the two following typedefs.
	// the precompiled_sx_controller is somewhat more complex, but is a lot 
	// faster.

	typedef sx_emulator::precompiled_sx_controller emulator_type;
	//typedef sx_emulator::sx_controller emulator_type;


	typedef boost::shared_ptr<emulator_type> emulator_pointer_type;
	typedef unsigned short address_type;
	typedef unsigned short pc_type;
	typedef unsigned char register_type;
    typedef sx_state state;
	typedef unsigned long histogram_type[rom_size];

	sx_simulator();
	sx_simulator( emulator_pointer_type emulator_);

	bool    load_rom( const unsigned short (&instructions)[rom_size]);
	unsigned long run( unsigned long tick_count);
	state   get_state() const;
	void    set_state( const state &new_state);
	void    set_breakpoint( address_type address, bool do_set = true);
    void    set_temporary_breakpoint( address_type address);
    void    set_temporary_breakpoint_after_call( address_type address);
	void    on_memory_write( address_type address,
	            boost::function< void( address_type, register_type )> handler);

	const histogram_type &get_histogram() const;
	void    reset_histogram();
	void    reset();

private:
    void add_todo( address_type address);
    void run_todos();

    typedef std::vector< address_type> todo_list;
    todo_list todo;
	emulator_pointer_type emulator;
};
#endif //SX_SIMULATOR_HPP
