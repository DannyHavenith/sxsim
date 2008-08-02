#if !defined( SX_STATE_HPP)
#define SX_STATE_HPP

#include "sx_memory.hpp"

	struct sx_light_state 
	{
		typedef sx_emulator::sx_ram::register_t register_type;		
		register_type m;
		register_type w;
		unsigned short pc;
		bool in_interrupt;
	};

	struct sx_state : public sx_light_state
	{
		sx_emulator::sx_ram ram;
	};

#endif //SX_STATE_HPP