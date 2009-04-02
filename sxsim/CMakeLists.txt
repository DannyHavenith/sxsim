add_library(
	sxsim 
	
	listing_parser.cpp
	sx_simulator.cpp
	
	arithmetic_with_flags.hpp
	as_binary.hpp
	flags.hpp
	generic_sx_implementation.hpp
	instruction_decoder.hpp
	instruction_list.hpp
	listing_parser.hpp
	sx_controller.hpp
	sx_instruction_list.hpp
	sx_memory.hpp
	sx_nop.hpp
	sx_print.hpp
	sx_simulator.hpp
	sx_state.hpp
	targetver.h
	)
	
add_executable( sxsimulator sxsim.cpp)

target_link_libraries( sxsimulator sxsim) 