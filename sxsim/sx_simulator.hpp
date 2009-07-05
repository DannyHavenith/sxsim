
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// implementation of a wrapper around the real sx emulator.

#if !defined( SX_SIMULATOR_HPP)
#define SX_SIMULATOR_HPP
#include <boost/function.hpp> // for the function-template
#include "sx_memory.hpp"
#include "sx_state.hpp"

namespace sx_emulator
{
	class sx_controller; // pimpl
}

class sx_simulator
{
public:
	sx_simulator();
	~sx_simulator();
	typedef unsigned short address_type;
	typedef unsigned short pc_type;
	typedef unsigned char register_type;
    typedef sx_state state;

	bool load_rom( const unsigned short (&instructions)[4096]);
	unsigned long run( unsigned long tick_count);
	state get_state() const;
	void set_state( const state &new_state);
	void set_breakpoint( address_type address, bool do_set = true);
	void on_memory_access( address_type address, boost::function< void( address_type, register_type )> handler);
	void reset();
private:
	sx_emulator::sx_controller *emulator;
};
#endif //SX_SIMULATOR_HPP
