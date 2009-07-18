
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// an implementation of the sx instruction set that will only print the instructions 
#if !defined( SX_PRINT_INCLUDED)
#define SX_PRINT_INCLUDED

struct sx_print_base
{
public:
	sx_print_base()
		: stream( std::cout)
	{
	};

	void set_stream( std::ostream &stream_)
	{
		stream = &stream_;
	}
protected:
	struct instruction_printer
	{
		instruction_printer( std::ostream &stream_, const char *instruction)
			: name( instruction), stream(stream_)
		{}

		template< typename tag>
		void print( const tag &, int arg1 = -1 , int arg2 = -1)
		{
			stream << name << " ";
			if (arg1 != -1)
			{
				stream << arg1 << " ";
				if (arg2 != -1)
				{
					stream << arg2;
				}
			}
			stream << std::endl;
		}
	private:
		const char * const name;
		std::ostream &stream;
	};

private:
	std::ostream *stream;
};

#define GENERIC_SX_IMPLEMENT_INSTRUCTION( fname, args_in, args_out) \
	void execute args_in	\
	{											\
		instruction_printer(*stream, #fname).print args_out;	\
	}

#define GENERIC_SX_CLASS_HEADER sx_print : private sx_print_base

#include "generic_sx_implementation.hpp"

#undef GENERIC_SX_IMPLEMENT_INSTRUCTION
#undef GENERIC_SX_CLASS_HEADER

#undef IMPLEMENT_INSTRUCTION
#endif //SX_PRINT_INCLUDED
