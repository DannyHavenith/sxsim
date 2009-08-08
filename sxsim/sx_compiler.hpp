
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

/// meta-function that returns a function pointer type (to a free function), given an implementation type
template< typename implementation>
struct instruction_interface
{
	typedef void (type)( unsigned short, unsigned short, implementation *);
};

template< typename implementation, typename tag>
struct ins0
{
	static void execute( unsigned short,unsigned short, implementation *imp)
	{
		imp->execute( tag());
	}
};

template< typename implementation, typename tag>
struct ins1
{
	static void execute( unsigned short arg1, unsigned short, implementation *imp)
	{
		imp->execute( tag(), arg1);
	}
};

template< typename implementation, typename tag>
struct ins2
{
	static void execute( unsigned short arg1, unsigned short arg2, implementation *imp)
	{
		imp->execute( tag(), arg1, arg2);
	}
};

template< typename implementation>
struct compiled_instruction
{
    // typedef for the function interface of the 'precompiled' functions.
    typedef void (function_type)( unsigned short, unsigned short, implementation *);

	compiled_instruction()
		:f(0),a1(0), a2(0)
	{
	}

	explicit compiled_instruction( function_type *interface_, unsigned short arg1 = 0, unsigned short arg2 = 0)
		:f(interface_),a1(arg1), a2( arg2)
	{
	}

	void execute( implementation *imp) const
	{
		f( a1, a2, imp);
	}

    bool points_to( function_type *comp) const
    {
        return f == comp;
    }

private:
	function_type *f;
	unsigned short a1;
	unsigned short a2;
};

/// \brief class template that can translate sx-instructions to
/// instances that will call a handler with the right arguments.
///
/// This class template is used to translate a rom full of sx-instructions into an array full
/// of member function pointers each member function points to the implementation of the corresponding
/// instruction.
template< typename implementation>
class sx_compiler
{
public:
	typedef compiled_instruction<implementation> slot_type;

	sx_compiler(
			slot_type &location_)
	:location( location_)
	{
	}

	template<typename tag>
	void execute( const tag &)
	{
		typedef ins0< implementation, tag> ins;
		location = slot_type( &ins::execute);
	}

	template<typename tag>
	void execute( const tag &, int arg1)
	{
		typedef ins1< implementation, tag> ins;
		location = slot_type( &ins::execute, arg1);
	}

	template<typename tag>
	void execute( const tag &, int arg1, int arg2)
	{
		typedef ins2< implementation, tag> ins;
		location = slot_type( &ins::execute, arg1, arg2);
	}
private:
	slot_type &location;
};

#endif /* SX_COMPILER_HPP_ */
