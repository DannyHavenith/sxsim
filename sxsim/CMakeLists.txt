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

# when building for 64bit processors we need to 
# add -fPIC to the compiler options to be able to use
# this lib in shared libraries
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(sxsim PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

	
add_executable( sxsimulator sxsim.cpp)

target_link_libraries( sxsimulator sxsim) 