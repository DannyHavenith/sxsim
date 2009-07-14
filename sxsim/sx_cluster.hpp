/*
 * sx_cluster.hpp
 *
 *  Created on: Jul 14, 2009
 *      Author: danny
 */

#ifndef SX_CLUSTER_HPP_
#define SX_CLUSTER_HPP_
#include <vector>
#include <boost/shared_ptr.hpp>
#include "sx_controller.hpp"

namespace sx_emulator
{

class sx_cluster
{
public:
	typedef sx_controller controller_type;
	typedef boost::shared_ptr< controller_type> controller_ptr;

	controller_ptr new_controller()
	{
		controller_ptr controller( new controller_type);
		controllers.push_back( controller);
		return controller;
	}

	size_t tick( size_t count)
	{
		// first instruction is always executed
		if (count)
		{
			bool breakpoint_hit = false;
			do_tick();
			// other instructions may be breakpoints.
			while (--count & !breakpoint_hit)
			{
				breakpoint_hit = do_tick();
			}
		}

		return count;
	}

private:

	bool do_tick()
	{
		bool result = false;
		for ( controllers_type::iterator i = controllers.begin();
			i != controllers.end();
			++i)
		{
			bool this_result = (*i)->synchronized_tick();
			result = result || this_result;
		}
		return result;
	}

	typedef std::vector< controller_type> controllers_type;
	controllers_type controllers;
};

}

#endif /* SX_CLUSTER_HPP_ */
