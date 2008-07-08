#if !defined( INSTRUCTION_LIST_INCLUDED)
#define INSTRUCTION_LIST_INCLUDED

#include <boost/mpl/vector.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/max_element.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/empty.hpp>

#include "as_binary.hpp"

namespace micro_emulator 
{
	namespace mpl = boost::mpl;
	using boost::mpl::_;
	using binary_numbers::as_binary;
	using binary_numbers::to_binary_digits;

	template< typename sequence>
	struct trailing_zero_count;

	//
	// "unsafe", because we cannot apply this one
	// on empty sequences
	//
	template< typename sequence>
	struct unsafe_trailing_zero_count :
		mpl::eval_if_c<
		mpl::back< sequence>::type::value == 0,
		mpl::next< typename trailing_zero_count< typename mpl::pop_back< sequence>::type>::type >,
		mpl::int_<0>
		>
	{};

	template< typename sequence>
	struct trailing_zero_count:
		mpl::eval_if<
		typename mpl::empty< sequence>::type,
		mpl::int_<0>,
		unsafe_trailing_zero_count< sequence>
		>
	{};

	//
	// a masked argument is an argument (operand) that is part 
	// of the instruction word. It can be retrieved by masking the
	// instruction word and optionally shifting the result.
	// For example: an argument 'bit' that specifies a bit to be set or reset
	// that is always coded in bits 5-7 of an instruction word could be specified as
	//     struct bit_ : public masked_argument< 000011100000> {};
	//
	// other classes, like the argument_harvester class of the instruction decoder can
	// use these types to extract the data from an instruction word.
	template< unsigned long long bit_pattern>
	struct masked_argument 
	{
		typedef typename to_binary_digits< bit_pattern>::type bit_vector;
		static const int mask_shift = trailing_zero_count< bit_vector>::type::value;
		static const int mask = as_binary< bit_pattern>::value;
	};

	//
	// meta-meta function that is used to extract the size of 
	// an argument type.
	struct get_mask_size
	{
		template< typename argument>
		struct apply:
			mpl::size< typename argument::bit_vector>
		{};
	};

	//
	// determine the maximum mask size of a sequence of masked
	// arguments.
	template< typename argument_sequence>
	struct max_mask_size :
		mpl::eval_if<
			mpl::empty< argument_sequence>,
			mpl::int_<0>,
			typename mpl::deref< 
				typename mpl::max_element< 
					typename mpl::transform< argument_sequence, get_mask_size>::type  
				>::type
			>
		>
	{
	};


	//
	// the nary_member_func metafunction returns the 
	// type of a member function pointer with 'arg_count'
	// integer arguments. E.g. for arg_count == 2
	// it returns "void (object_type::*)( int, int)"
	//
	// implemented for 0 <= arg_count <= 2
	//
	template< typename object_type, int arg_count>
	struct nary_member_func
	{};

	template< typename object_type>
	struct nary_member_func< object_type, 0>
	{
		typedef void (object_type::*type)();
	};

	template< typename object_type>
	struct nary_member_func< object_type, 1>
	{
		typedef void (object_type::*type)( int);
	};
	template< typename object_type>
	struct nary_member_func< object_type, 2>
	{
		typedef void (object_type::*type)(int, int);
	};

	template< typename object_type, typename word_pattern> 
	struct pattern_member_func: nary_member_func< object_type, word_pattern::argument_count::value>
	{};

	template< typename implementation>
	struct instruction_list
	{
		typedef implementation impl;

		// an instruction word type encodes the bit pattern of
		// the opcode and any other arguments that are encoded 
		// in the word. Every word must be the same size.
		//
		// instantiations of this template are used together
		// with the 'instruction' template (see below).
		//
		// see also sx_instruction_list.hpp
		//
		template< 
			unsigned long long opcode_dcb, // decimal coded binary opcode
			typename arg1_type = mpl::void_,
			typename arg2_type = mpl::void_,
			typename arg3_type = mpl::void_
		>
		struct word 
		{
			// all argument types
			typedef mpl::vector< arg1_type, arg2_type, arg3_type> full_args;

			// the non-void argument types
			typedef typename mpl::iterator_range<
				typename mpl::begin< full_args>::type,
				typename mpl::find_if< full_args, boost::is_same< mpl::void_, _> >::type
			>::type args_seq;

			// create a back-insertable version of the argument sequence
			typedef typename mpl::copy< 
				args_seq,
				mpl::back_inserter< mpl::vector0<> >
			>::type args;


			// argument count
			typedef typename mpl::size< args>::type argument_count;
			typedef typename max_mask_size< args>::type operand_size;
			typedef typename to_binary_digits< opcode_dcb>::type bits;
			static const int opcode = as_binary<opcode_dcb>::value;
		};

		//
		// an 'instruction' instantiation couples a 'word' definition (see above)
		// with a member function of some object to be called.
		// This is used by an instruction decoder, which uses the word pattern to
		// decode an instruction word and then calls the specified member function.
		//
		template< 
			typename word_pattern,
			typename pattern_member_func< impl, word_pattern>::type ptr
		> 
		struct instruction
		{
			typedef word_pattern word;

			typedef typename word::operand_size operand_size;
			typedef typename word::bits bits;
			typedef typename pattern_member_func< impl, word_pattern>::type func_ptr;
			static const func_ptr function_ptr(){ return ptr;}
		};
	};
}
#endif //INSTRUCTION_LIST_INCLUDED
