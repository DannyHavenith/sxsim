#if !defined( INSTRUCTION_DECODER_INCLUDED)
#define INSTRUCTION_DECODER_INCLUDED

#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/front_inserter.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/lower_bound.hpp>
#include <boost/mpl/insert.hpp>
//#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
namespace micro_emulator 
{
	//
	// This is the file with most of the meta-programming in it.
	// an instruction decoder receives an instruction list. 
	// out of this instruction list, the decoder will create a binary 
	// tree of instructions. The left branch of the tree contains all
	// instructions who's opcode starts with a zero, the right branch all 
	// those who's opcode starts with one (left-most bit). 
	// One level below that, the instructions are split up based on their 
	// second bit (second-to-left-most bit), etc.
	namespace mpl = boost::mpl;
	namespace fusion = boost::fusion;

	template<typename instruction_list>
	struct instruction_decoder : public instruction_list::impl
	{

	private:
		typedef int instruction_type;

		// decide nodes are branches of the binary tree.
		template< int bit, typename on_zero, typename on_one>
		struct decide_node {};

		// a call node is a leaf of the tree. When we reach a call
		// node, we've finished decoding the instruction and we can
		// call the appropriate function.
		template <typename instruction>
		struct call_node {};

		template< class T>
		struct call_tag {};

		template< int bit, typename on_zero, typename on_one>
		static void decode_and_call( 
			instruction_type word, 
			typename instruction_list::impl &implementation,
			const call_tag<decide_node< bit, on_zero, on_one> > &)
		{
			if (word & (1<<bit))
			{
				decode_and_call( word, implementation, call_tag<on_one>());
			}
			else
			{
				decode_and_call( word, implementation, call_tag<on_zero>());
			}
		}

		template< class instruction>
		static void decode_and_call( 
			instruction_type word, 
			typename instruction_list::impl &implementation,
			const call_tag< call_node< instruction> > &)
		{
			dispatch<instruction>( word, implementation);
		}

		typedef instruction_decoder< instruction_list> this_type;
		typedef typename instruction_list::impl impl;

		// 
		// the argument harvester takes an instruction word and 
		// extracts the operands from it, essentially by masking the
		// instruction word and, if needed, shifting the masked bits.
		//
		// The result is a fusion::tuple with operand values. This tuple 
		// can then be used to call a function with fusion::invoke.
		//
		template< typename argument_vector>
		struct argument_harvester
		{
			typedef typename mpl::pop_back<argument_vector>::type next_vector;
			typedef typename fusion::result_of::push_back<
				typename const argument_harvester< next_vector>::result_type, 
				int
			>::type  result_type;

			typedef typename mpl::back<argument_vector>::type last_argument;

			static result_type harvest( unsigned int instruction)
			{
				return fusion::push_back(
					argument_harvester<next_vector>::harvest( instruction),
					static_cast<int>((instruction & last_argument::mask) >> last_argument::mask_shift)
					);
			}
		};

		template<>
		struct argument_harvester< mpl::vector0<> >
		{
			typedef fusion::tuple<> result_type;
			static result_type harvest( unsigned int )
			{
				return result_type();
			}
		};

		template< typename instruction>
		static void dispatch( int code, impl &obj)
		{
			fusion::invoke( 
				instruction::function_ptr(), 
				fusion::push_front( 
				argument_harvester< instruction::word::args>::harvest(code),
				obj
				)
				);
		}

		//
		// for an instruction with a given opcode (say "pppp") and 
		// a given operand size (say 3), this metafunction returns the 
		// opcode bit at a given position in an instruction word, as if 
		// the opcode was defined as "000...000ppp222".
		template< typename instruction, int bit>
		struct opcode_bit_at :
			mpl::if_c< 
				(bit < instruction::operand_size::value),
				mpl::int_<2>,
				typename mpl::eval_if_c<
					(bit >= instruction::operand_size::value + mpl::size<typename instruction::bits>::type::value),
					mpl::identity< mpl::int_<0> >,
					mpl::at< 
						typename instruction::bits,
						mpl::int_< mpl::size< typename instruction::bits>::value - (bit - instruction::operand_size::value) - 1>
					>
				>::type
			>
		{};


		template< int discriminating_bit, int value>
		struct has_at 
		{
			template< typename instruction>
			struct apply:
				boost::is_same< 
					typename opcode_bit_at< instruction, discriminating_bit>::type,
					mpl::int_<value>
				>
			{};
		};

		//
		// meta-function that creates a binary tree out of the instruction
		// list.
		//
		template< typename instructions, int discriminating_bit, unsigned long long history, typename enable = void>
		struct make_tree
		{
			// split the instructions into two sets: the ones that have a zero at bit 'discriminating_bit' and the
			// ones that have a 1 in that position.
			typedef typename mpl::copy_if< instructions, has_at< discriminating_bit, 0>, mpl::front_inserter< mpl::list<> > >::type zeros;
			typedef typename mpl::copy_if< instructions, has_at< discriminating_bit, 1>, mpl::front_inserter< mpl::list<> > >::type ones;

			// if you uncomment the typedef and the assert:
			// 1. your compilation time will go up with about 50%
			// 2. you'll get an assertion if there are ambiguous instructions in your set.
			// An ambiguous instruction exists when two instructions can not be discriminated based on their leftmost
			// N bits. If you, for instance define and instruction 1010xxxx and an instruction 101yyyyy, then the decoder
			// does not know whether to choose the first one or the second after finding an instruction that starts 
			// with 101...

			//typedef typename mpl::copy_if< instructions, has_at< discriminating_bit, 2>, mpl::front_inserter< mpl::list<> > >::type undefineds;
			//BOOST_MPL_ASSERT(( mpl::empty<undefineds>::type));

			// the general case, were both sets are nonempty means we have to 
			// analyse more bits to find which instruction we have.
			// this means we enter a 'decide node' at this position.
			template< typename left, typename right, typename enable = void>
			struct mtree
			{
				typedef typename decide_node< 
					discriminating_bit,
					typename make_tree< left, discriminating_bit - 1, history * 10 >::type,
					typename make_tree< right, discriminating_bit - 1, 1 + (history * 10)>::type
					>  type;
			};

			//
			// both sets empty means we started with an empty instruction list
			// that's an error.
			//
			template< typename left, typename right>
			struct mtree< left, right, 
				typename boost::enable_if< 
					mpl::and_< 
						mpl::empty< left>, 
						mpl::empty< right> 
					> 
				>::type 
			> 
			{
				BOOST_MPL_ASSERT(( mpl::false_));
			}; 

			//
			// if one of the sets is empty, this means that we don't have to examine
			// this bit (the discriminating_bit) to decode the instruction.
			//
			template< typename left, typename right>
			struct mtree< left, right, 
				typename boost::enable_if< 
					mpl::and_< 
						mpl::empty< left>, 
						mpl::not_< mpl::empty< right> >
					> 
				>::type 
			> : make_tree< right, discriminating_bit - 1, history * 10 + 1>{}; 

			template< typename left, typename right>
			struct mtree< left, right, 
				typename boost::enable_if< 
					mpl::and_< 
						mpl::empty< right>, 
						mpl::not_< mpl::empty< left> >
					> 
				>::type 
			> : make_tree< left, discriminating_bit - 1, history * 10>{}; 
			
			typedef typename mtree< zeros, ones>::type type;
		};

		// if there's 1 instruction left in the set, just call it without testing any more bits.
		template< typename instructions, int discriminating_bit, unsigned long long history>
		struct make_tree<instructions, discriminating_bit, history, typename boost::enable_if_c< mpl::size< instructions>::value == 1>::type>
		{
			typedef call_node< typename mpl::front<instructions>::type> type;
		};

		// we shouldn't be here, there are no instructions in the set.
		template< typename instructions, int discriminating_bit, unsigned long long history>
		struct make_tree<instructions, discriminating_bit, history, typename boost::enable_if< mpl::empty< instructions> >::type>
		{
			BOOST_MPL_ASSERT(( mpl::false_));
		};

		// we shouldn't be here, there are no bits left to test on
		// If this assert triggers, there are two identical non-argument expressions in the 
		// instruction list.
		template< typename instructions, unsigned long long history>
		struct make_tree<instructions, -2, history, typename boost::enable_if< mpl::not_< mpl::empty< instructions> > >::type>
		{
			BOOST_MPL_ASSERT(( mpl::false_));
		};

		// make a decision tree. I really should make this '11' a template argument (the number of relevant
		// instruction bits)
		typedef typename make_tree< typename instruction_list::instructions, 11, 0>::type instruction_tree;

		public:
			// feed one instruction. 
			// this instructin will be dispatched to the right member
			// function
			void feed( instruction_type word)
			{
				decode_and_call( word,	*this, call_tag< instruction_tree>());
			}
	};
}
#endif //INSTRUCTION_DECODER_INCLUDED