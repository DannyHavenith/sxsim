#if !defined( FLAGS_INCLUDED)
#define FLAGS_INCLUDED

// definition of sx flags
struct sx_flags
{
	static const int DC_MASK = 0x0f;
	static const int CARRY_MASK = 0xff;
	static const int LEFT_OUTSIDE_CARRY_MASK = 
		(CARRY_MASK | (CARRY_MASK << 1)) ^ CARRY_MASK;

	static const int NO_FLAGS = 0;
	static const int Z  = (1<<2);
	static const int DC = (1<<1);
	static const int C  = (1<<0);
};

typedef sx_flags sx_flags_definition;

#endif //FLAGS_INCLUDED