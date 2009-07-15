
##          Copyright Danny Havenith 2009.
## Distributed under the Boost Software License, Version 1.0.
##    (See accompanying file LICENSE_1_0.txt or copy at
##          http://www.boost.org/LICENSE_1_0.txt)

add_library(
	sxsim 
	
	listing_parser.cpp
	sx_simulator.cpp
	sx_cluster.cpp
	
	memory_events.hpp
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
	sx_compiler.hpp
	sx_state.hpp
	sx_cluster.hpp
	targetver.h
	)

# when building for 64bit processors on linux we need to 
# add -fPIC to the compiler options to be able to use
# this lib in shared libraries
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(sxsim PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

