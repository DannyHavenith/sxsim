#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include "listing_parser.hpp"
#include "sx_instruction_list.hpp"
#include "instruction_decoder.hpp"
#include "sx_controller.hpp"

#include "sx_simulator.hpp"


sx_simulator::sx_simulator()
{
	emulator = new micro_emulator::sx_controller();
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

unsigned short sx_simulator::run( unsigned long tick_count)
{
	while (tick_count--)
	{
			emulator->tick();
	}
	return emulator->get_pc();
}

void sx_simulator::reset()
{
	// TODO: emulator->reset
	emulator->set_pc( 0x7ff);
}

const sx_simulator::state sx_simulator::get_state() const
{
	// TODO: more state
	state s;
	s.ram = emulator->get_ram();
	return s;
}

void sx_simulator::set_state( const sx_simulator::state &new_state)
{
}

