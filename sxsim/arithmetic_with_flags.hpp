#if !defined( ARITHMETIC_WITH_FLAGS_INCLUDED)
#define ARITHMETIC_WITH_FLAGS_INCLUDED

#include "flags.hpp"

namespace arithmetic_with_flags
{

	template< int allowed_flags = sx_flags::Z | sx_flags::C | sx_flags::DC>
	class flagged : public sx_flags_definition // use Z, DC and C
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

			int result = data_ - rhs;

			set( Z, !(data_ = result & CARRY_MASK));
			set( C, result & ~CARRY_MASK);

			return *this;
		}

		flagged &operator|=( int rhs)
		{
			data_ |= rhs;
			set( Z, !data_);

			return *this;
		}

		flagged &operator&=( int rhs)
		{
			data_ &= rhs;
			set( Z, !data_);

			return *this;
		}

		flagged &operator^=( int rhs)
		{
			data_ ^= rhs;
			set( Z, !data_);

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
			set( Z, data_);

			return *this;
		}

		flagged &rr()
		{
			if (get( C))
			{
				data_ |= LEFT_OUTSIDE_CARRY_MASK;
			}
			set( C, data_&1);
			data_ = ((data_  >> 1) & CARRY_MASK);
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
			if (flag & allowed_flags)
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
		}

		bool get( int flag)
		{
			return (flags_ & ~flag) != 0;
		}


	};
}
#endif //ARITHMETIC_WITH_FLAGS_INCLUDED