#if !defined( SX_SIMULATOR_HPP)
#define SX_SIMULATOR_HPP

#include "sx_memory.hpp"

namespace micro_emulator
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

	struct light_state 
	{
		register_type w;
		register_type flags;
		register_type rtcc;
		bool in_interrupt;
	};

	struct state : public light_state
	{
		micro_emulator::sx_ram ram;
	};

	bool load_rom( const unsigned short (&instructions)[4096]);
	unsigned short run( unsigned long tick_count);
	const state get_state() const;
	void set_state( const state &new_state);
	void set_breakpoint( address_type address, bool do_set = true);
	void reset();
private:
	micro_emulator::sx_controller *emulator;
};
#endif //SX_SIMULATOR_HPP
