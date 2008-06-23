#if !defined( AS_BINARY_HPP_INCLUDED)
#define AS_BINARY_HPP_INCLUDED
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/fold.hpp>

namespace binary_numbers
{
	namespace mpl = boost::mpl;
template< unsigned long long number, unsigned long source_radix>
struct to_digits_impl
{
	typedef typename to_digits_impl< number/source_radix, source_radix>::type prefix;
	typedef typename mpl::push_back< prefix, mpl::int_< number % source_radix> >::type type;
};

template< unsigned long source_radix>
struct to_digits_impl< 0, source_radix>
{
	typedef mpl::vector_c<int> type;
};

template< unsigned long long number, unsigned long source_radix = 10>
struct to_digits : 
	mpl::eval_if_c< 
		number == 0,
		mpl::vector_c< int, 0>,
		to_digits_impl< number, source_radix>
		> 
{};

struct binary_advance
{
	template< typename left, typename right>
	struct apply : mpl::int_< left::value * 2 + right::value> {};
};

struct is_binary_digit_advance
{
	template< typename left, typename right>
	struct apply : 
		mpl::bool_< left::value && (right::value == 1 || right::value == 0)> {};
};

template< typename sequence>
struct is_binary_digits : 
	mpl::fold< 
		sequence,
		mpl::bool_<true>,
		is_binary_digit_advance
	>
{};

template< unsigned long long number>
struct to_binary_digits:
	mpl::eval_if<
		typename is_binary_digits< typename to_digits< number, 10>::type>::type,
		to_digits< number, 10>,
		to_digits< number, 8>
	>
{};

template< unsigned long long number, unsigned long source_radix = 10>
struct as_binary_impl : 
	mpl::fold< 
		typename to_digits< number, source_radix>::type,
		mpl::int_<0>,
		binary_advance
	>
{};

template< unsigned long long number>
struct as_binary: 	
	mpl::fold< 
		typename to_binary_digits< number>::type,
		mpl::int_<0>,
		binary_advance
	>
{
	static const int value = type::value;
	operator unsigned int() {return value;}
};
}

#endif //AS_BINARY_HPP_INCLUDED