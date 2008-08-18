#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include "listing_parser.hpp"
#include "sx_instruction_list.hpp"
#include "instruction_decoder.hpp"
#include "sx_controller.hpp"

#include "sx_simulator.hpp"


sx_simulator::sx_simulator()
{
	emulator = new sx_emulator::sx_controller();
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
}

