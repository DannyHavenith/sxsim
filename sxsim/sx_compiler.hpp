
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
	virtual void execute(unsigned short arg1, unsigned short arg2, implementation *imp ) const = 0;
};

template< typename implementation, typename tag>
struct ins0 : instruction_interface<implementation>
{
	virtual void execute( unsigned short,unsigned short, implementation *imp) const
	{
		imp->execute( tag());
	}

	static instruction_interface<implementation> *get_instance()
	{
		static ins0< implementation, tag> instance;
		return &instance;
	}

};

template< typename implementation, typename tag>
struct ins1 : instruction_interface<implementation>
{
	virtual void execute( unsigned short arg1, unsigned short, implementation *imp) const
	{
		imp->execute( tag(), arg1);
	}

	static instruction_interface<implementation> *get_instance()
	{
		static ins1< implementation, tag> instance;
		return &instance;
	}
};

template< typename implementation, typename tag>
struct ins2 : instruction_interface<implementation>
{
	virtual void execute( unsigned short arg1, unsigned short arg2, implementation *imp) const
	{
		imp->execute( tag(), arg1, arg2);
	}

	static instruction_interface<implementation> *get_instance()
	{
		static ins2< implementation, tag> instance;
		return &instance;
	}

};

///
/// special instance of the instruction interface.
/// This one can point to any nullary member function of the implementation
/// class.
template< typename implementation>
struct ins_notag : instruction_interface< implementation>
{
	typedef void (implementation::*fptr)();
	ins_notag( fptr f_)
		: f( f_)
	{
	}

	virtual void execute( unsigned short, unsigned short, implementation *imp) const
	{
		(imp->*f)();
	}
private:
	const fptr f;
};

template< typename implementation>
struct compiled_instruction
{
	typedef instruction_interface<implementation> interface_type;

	compiled_instruction()
		:i(0),a1(0), a2(0)
	{
	}
	explicit compiled_instruction( const interface_type *interface_, unsigned short arg1 = 0, unsigned short arg2 = 0)
		:i(interface_),a1(arg1), a2( arg2)
	{
	}

	void execute( implementation *imp) const
	{
		i->execute( a1, a2, imp);
	}

private:
	const interface_type *i;
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
	typedef instruction_interface< implementation> interface_type;
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
		location = slot_type( ins::get_instance());
	}

	template<typename tag>
	void execute( const tag &, int arg1)
	{
		typedef ins1< implementation, tag> ins;
		location = slot_type( ins::get_instance(), arg1);
	}

	template<typename tag>
	void execute( const tag &, int arg1, int arg2)
	{
		typedef ins2< implementation, tag> ins;
		location = slot_type( ins::get_instance(), arg1, arg2);
	}
private:
	slot_type &location;
};

#endif /* SX_COMPILER_HPP_ */
