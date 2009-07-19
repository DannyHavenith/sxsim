
//          Copyright Danny Havenith 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
#include "sx_simulator.hpp"

namespace sx_emulator
{
class sx_controller;

class sx_cluster
{
public:
	typedef sx_controller controller_type;
	typedef boost::shared_ptr< controller_type> controller_ptr;

	sx_simulator new_controller();
	size_t run( size_t count);

private:
	bool do_tick();
	bool do_forced_tick();
	typedef std::vector< controller_ptr> controllers_type;
	controllers_type controllers;
};

}

#endif /* SX_CLUSTER_HPP_ */
