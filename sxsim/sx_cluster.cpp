
//          Copyright Danny Havenith 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// Implementation of the methods of the sx_cluster class.
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include "sx_cluster.hpp"
#include "sx_controller.hpp"

namespace sx_emulator
{

sx_simulator sx_cluster::new_controller()
{
	controller_ptr controller( new controller_type);
	controllers.push_back( controller);
	return sx_simulator( controller);
}

size_t sx_cluster::run( size_t count)
{
	if (count)
	{
		bool breakpoint_hit = false;
		// first instruction is always executed
		do_forced_tick();
		// other instructions may be breakpoints.
		while (--count && !breakpoint_hit)
		{
			breakpoint_hit = do_tick();
		}
	}

	return count;
}

bool sx_cluster::do_tick()
{
	bool result = false;
	for ( controllers_type::iterator i = controllers.begin();
		i != controllers.end();
		++i)
	{
		result = (*i)->synchronized_tick() || result ;
	}
	return result;
}

bool sx_cluster::do_forced_tick()
{
	bool result = false;
	for ( controllers_type::iterator i = controllers.begin();
		i != controllers.end();
		++i)
	{
		result = (*i)->tick() || result ;
	}
	return result;
}
}
