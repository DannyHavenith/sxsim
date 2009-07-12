
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
#include <boost/scoped_ptr.hpp>

template< typename implementation>
struct instruction_interface
{
	virtual void execute( implementation *) const = 0;
};

template< typename implementation, typename tag>
struct ins0 : instruction_interface<implementation>
{
	ins0( )
	{}

	virtual void execute( implementation *imp) const
	{
		imp->execute( tag());
	}
};

template< typename implementation, typename tag>
struct ins1 : instruction_interface<implementation>
{
	ins1( short arg1_)
		:arg1( arg1_)
	{}

	virtual void execute( implementation *imp) const
	{
		imp->execute( tag(), arg1);
	}

private:
	const short arg1;
};

template< typename implementation, typename tag>
struct ins2 : instruction_interface<implementation>
{
	ins2( short arg1_, short arg2_)
		:arg1( arg1_), arg2( arg2_)
	{}

	virtual void execute( implementation *imp) const
	{
		imp->execute( tag(), arg1, arg2);
	}

private:
	const short arg1;
	const short arg2;
};

template< typename implementation>
struct ins_notag : instruction_interface< implementation>
{
	typedef void (implementation::*fptr)();
	ins_notag( fptr f_)
		: f( f_)
	{
	}

	virtual void execute( implementation *imp) const
	{
		(imp->*f)();
	}
private:
	const fptr f;
};

/// \brief class template that can translate sx-instructions to
/// boost::function instances that will call a handler with the right arguments.
/// This class template is used to translate a rom full of sx-instructions into an array full
/// of function pointers to the right functions.
template< typename implementation>
class sx_compiler
{
public:
	typedef instruction_interface< implementation> interface_type;
	typedef boost::scoped_ptr< interface_type> slot_type;

	sx_compiler(
			slot_type &location_)
	:location( location_)
	{
	}

	template<typename tag>
	void execute( const tag &)
	{
		typedef ins0< implementation, tag> ins;
		location.reset( new ins());
	}

	template<typename tag>
	void execute( const tag &, int arg1)
	{
		typedef ins1< implementation, tag> ins;
		location.reset( new ins(arg1));
	}

	template<typename tag>
	void execute( const tag &, int arg1, int arg2)
	{
		typedef ins2< implementation, tag> ins;
		location.reset( new ins( arg1, arg2));
	}
private:
	slot_type &location;
};

#endif /* SX_COMPILER_HPP_ */
