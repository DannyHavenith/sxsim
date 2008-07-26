#if !defined( ARITHMETIC_WITH_FLAGS_INCLUDED)
#define ARITHMETIC_WITH_FLAGS_INCLUDED

#include "flags.hpp"

namespace arithmetic_with_flags
{

	//
	// implementation of some basic arithmetic that sets/resets
	// flags.
	//
	// Objects of his class are typically instantiated on the fly, when some
	// calculation needs to be performed and discarded right after that.
	// An optimizing compiler will discard the object creation and only implement the 
	// actual arithmetic.
	//
	// This implementation can modify Z (zero), C (carry) and DC (digit carry) flags.
	//
	// typical usage example:
	//	flagged( some_value, my_flags) += 10; // add 10 to some value and set my flags.
	//
	class flagged : public sx_flags_definition // use Z, DC and C symbols
	{

	public:

		typedef unsigned char register_t;

		operator register_t() const
		{
			return data_;
		}

		operator bool() const
		{
			return data_ != 0;
		}

		flagged( register_t &data, register_t &flags)
			:data_(data), flags_(flags)
		{ //nop
		}

		flagged operator=( register_t data)
		{
			set( Z,  0 == (data_ = data));
			return *this;
		}

		flagged &operator+=( int rhs)
		{
			set( DC, ((data_ & DC_MASK) + (rhs & DC_MASK)) & ~DC_MASK);

			int result = data_ + rhs;

			set( Z, !(data_ = result & CARRY_MASK));
			set( C, result & ~CARRY_MASK);

			return *this;
		}

		flagged &operator-=( int rhs)
		{
			set( DC, ((data_ & DC_MASK) - (rhs & DC_MASK)) & ~DC_MASK);

			unsigned int result = data_ - rhs;

			set( Z, !(data_ = result & CARRY_MASK));
			set( C, result & ~CARRY_MASK);

			return *this;
		}

		flagged &operator|=( int rhs)
		{
			set( Z, !(data_ |= rhs));

			return *this;
		}

		flagged &operator&=( int rhs)
		{
			set( Z, !(data_ &= rhs));

			return *this;
		}

		flagged &operator^=( int rhs)
		{
			set( Z, !(data_ ^= rhs));

			return *this;
		}

		flagged &operator++()
		{
			set( Z, !(++data_));

			return *this;
		}

		flagged &operator--()
		{
			set( Z, !(data_ = (data_ - 1) & CARRY_MASK));

			return *this;
		}

		flagged &test()
		{
			set( Z, data_ == 0);

			return *this;
		}

		flagged &rr()
		{
			unsigned int result = data_;
			if (get( C))
			{
				result |= LEFT_OUTSIDE_CARRY_MASK;
			}
			set( C, result&1);
			data_ = ((result >> 1) & CARRY_MASK);
			return *this;
		}

		flagged &rl()
		{
			int result = data_ << 1;
			if (get( C))
			{
				result |= 1;
			}
			set( C, result & LEFT_OUTSIDE_CARRY_MASK);
			data_ = (result & CARRY_MASK);
			return *this;
		}
	private:

		register_t &data_;
		register_t &flags_;

		void set( int flag, int do_)
		{
			if (do_)
			{
				flags_ |= flag;
			}
			else
			{
				flags_ &= ~flag;
			}
		}

		bool get( int flag)
		{
			return (flags_ & flag) != 0;
		}


	};
}
#endif //ARITHMETIC_WITH_FLAGS_INCLUDED
