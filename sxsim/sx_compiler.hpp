
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


/*
 * sx_compiler.hpp
 *
 *  Created on: Jul 9, 2009
 *      Author: danny
 */

#ifndef SX_COMPILER_HPP_
#define SX_COMPILER_HPP_
#include <boost/bind.hpp>
#include <boost/function.hpp>

/// \brief class template that can translate sx-instructions to
/// boost.function instances that will call a handler with the right arguments

template< typename implementation>
class sx_compiler
{
public:
	sx_compiler(
			boost::function<void()> &location_,
			implementation *impl_)
	:location( location_),
	impl( impl_)
	{
	}

	template<typename tag>
	void execute( const tag &)
	{
		typedef void (implementation::*member_function_type)( const tag &);
		location =
			boost::bind(
					(member_function_type)&implementation::execute,
					impl,
					tag()
					);
	}

	template<typename tag>
	void execute( const tag &, int arg1)
	{
		typedef void (implementation::*member_function_type)( const tag &, int);
		location =
			boost::bind(
					(member_function_type)&implementation::execute,
					impl,
					tag(),
					arg1);
	}

	template<typename tag>
	void execute( const tag &, int arg1, int arg2)
	{
		typedef void (implementation::*member_function_type)( const tag &, int, int);
		location =
			boost::bind(
					(member_function_type)&implementation::execute,
					impl,
					tag(),
					arg1,
					arg2);
	}
private:
	boost::function<void()> &location;
	implementation *impl;
};

#endif /* SX_COMPILER_HPP_ */
