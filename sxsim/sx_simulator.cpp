
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// implementation file for the sx_controller wrapper.
/// This is the source file that will trigger most templates to be instantiated.

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include "listing_parser.hpp"
#include "sx_instruction_list.hpp"
#include "instruction_decoder.hpp"
#include "sx_controller.hpp"

#include "sx_simulator.hpp"


sx_simulator::sx_simulator()
{
	emulator = new emulator_type();
}

sx_simulator::~sx_simulator()
{
	delete emulator;
}

//
// to be replaced with a load_rom, in a future where
// rom contents can be grabbed from other sources, such as
// obj-files...
//
bool sx_simulator::load_rom( const unsigned short (&instructions)[4096])
{
	emulator->load_rom(	instructions);
	return true;
}

void sx_simulator::set_breakpoint( address_type address, bool do_set)
{
	if (do_set)
	{
		emulator->set_breakpoint( address);
	}
	else
	{
		emulator->remove_breakpoint( address);
	}
}

void  sx_simulator::on_memory_access(
	sx_simulator::address_type address,
	boost::function< void(  sx_simulator::address_type, sx_simulator::register_type )> handler
	)
{
	emulator->on_memory_access( address, handler);
}

/// get profiling information: a count of how often each instruction was run
const sx_simulator::histogram_type &sx_simulator::get_histogram() const
{
	return emulator->get_histogram();
}

/// set all statistics to zero
void sx_simulator::reset_histogram()
{
	emulator->reset_histogram();
}

unsigned long sx_simulator::run( unsigned long tick_count)
{
	return emulator->tick( tick_count);
}

void sx_simulator::reset()
{
	emulator->reset();
}

sx_simulator::state sx_simulator::get_state() const
{
	return emulator->get_state();
}

void sx_simulator::set_state( const sx_simulator::state &new_state)
{
	emulator->set_state( new_state);
}

