#if !defined( SX_SIMULATOR_HPP)
#define SX_SIMULATOR_HPP

#include "sx_memory.hpp"
#include "sx_state.hpp"

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
    typedef sx_state state;

	bool load_rom( const unsigned short (&instructions)[4096]);
	unsigned short run( unsigned long tick_count);
	state get_state() const;
	void set_state( const state &new_state);
	void set_breakpoint( address_type address, bool do_set = true);
	void reset();
private:
	micro_emulator::sx_controller *emulator;
};
#endif //SX_SIMULATOR_HPP
