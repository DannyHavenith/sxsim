#if !defined( SX_NOP_INCLUDED)
#define SX_NOP_INCLUDED

// 
// this integer is used to make sure that the compiler does not
// optimize away all sx_nop instructions.
//
namespace {
	static volatile int sx_nop_secret_word = 0;
}

struct sx_nop_base
{
protected:
	void store_arg( int a1 = -1, int a2 = -1)
	{
		sx_nop_secret_word = a1;
	}
};

#define GENERIC_SX_IMPLEMENT_INSTRUCTION( fname, args_in, args_out) \
	void fname args_in	\
	{											\
		store_arg args_out;	\
	}

#define GENERIC_SX_CLASS_HEADER sx_nop: private sx_nop_base

#include "generic_sx_implementation.hpp"

#undef GENERIC_SX_IMPLEMENT_INSTRUCTION
#undef GENERIC_SX_CLASS_HEADER

#undef IMPLEMENT_INSTRUCTION
#endif //SX_NOP_INCLUDED
