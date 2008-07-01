#if !defined( SX_PRINT_INCLUDED)
#define SX_PRINT_INCLUDED

struct sx_print_base
{
protected:
	struct instruction_printer
	{
		instruction_printer( const char *instruction)
			: name( instruction)
		{}

		void print( int arg1 = -1 , int arg2 = -1)
		{
			std::cout << name << " ";
			if (arg1 != -1)
			{
				std::cout << arg1 << " ";
				if (arg2 != -1)
				{
					std::cout << arg2;
				}
			}
			std::cout << std::endl;
		}
	private:
		const char * const name;
	};
};

#define GENERIC_SX_IMPLEMENT_INSTRUCTION( fname, args_in, args_out) \
	void fname args_in	\
	{											\
		instruction_printer(#fname).print args_out;	\
	}

#define GENERIC_SX_CLASS_HEADER sx_print : private sx_print_base

#include "generic_sx_implementation.hpp"

#undef GENERIC_SX_IMPLEMENT_INSTRUCTION
#undef GENERIC_SX_CLASS_HEADER

#undef IMPLEMENT_INSTRUCTION
#endif //SX_PRINT_INCLUDED