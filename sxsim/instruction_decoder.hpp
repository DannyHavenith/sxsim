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
	namespace mpl = boost::mpl;
	namespace fusion = boost::fusion;

	template<typename instruction_list>
	struct instruction_decoder : public instruction_list::impl
	{

	private:
		typedef int instruction_type;
		template< int bit, typename on_zero, typename on_one>
		struct decide_node {};

		template <typename instruction>
		struct call_node {};


		template<class tree>
		struct decode_and_dispatch
		{
		};

/* partial specialization didn't work. using overloads instead.
		template< class instruction>
		struct decode_and_dispatch< call_node< instruction> >
		{
			static void call( int word, typename instruction_list::impl &implementation)
			{
				dispatch<instruction>( word, implementation);
			}
		};

		template< int bit, typename on_zero, typename on_one>
		struct decode_and_dispatch< decide_node< bit, on_zero, on_one> >
		{
			static void call( int word, typename instruction_list::impl &implementation)
			{
				if (word & (1<<bit))
				{
					decode_and_dispatch< on_one>::call( word, implementation);
				}
				else
				{
					decode_and_dispatch< on_zero>::call( word, implementation);
				}
			}
		};
*/
		template< class T>
		struct call_tag {};

		template< int bit, typename on_zero, typename on_one>
		static void call( 
			instruction_type word, 
			typename instruction_list::impl &implementation,
			const call_tag<decide_node< bit, on_zero, on_one> > &)
		{
			if (word & (1<<bit))
			{
				call( word, implementation, call_tag<on_one>());
			}
			else
			{
				call( word, implementation, call_tag<on_zero>());
			}
		}

		template< class instruction>
		static void call( 
			instruction_type word, 
			typename instruction_list::impl &implementation,
			const call_tag< call_node< instruction> > &)
		{
			dispatch<instruction>( word, implementation);
		}

		typedef instruction_decoder< instruction_list> this_type;
		typedef typename instruction_list::impl impl;

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

		template< typename instructions, int discriminating_bit, unsigned long long history, typename enable = void>
		struct make_tree
		{
			//typedef mpl::filter_view< instructions, has_at< discriminating_bit, 0> > zeros;
			//typedef mpl::filter_view< instructions, has_at< discriminating_bit, 1> > ones;
			typedef typename mpl::copy_if< instructions, has_at< discriminating_bit, 0>, mpl::front_inserter< mpl::list<> > >::type zeros;
			typedef typename mpl::copy_if< instructions, has_at< discriminating_bit, 1>, mpl::front_inserter< mpl::list<> >  >::type ones;
			//typedef mpl::filter_view< instructions, has_at< discriminating_bit, 2> > undefineds;
			//BOOST_MPL_ASSERT(( mpl::empty<undefineds>::type));

			template< typename left, typename right, typename enable = void>
			struct mtree
			{
				typedef typename decide_node< 
					discriminating_bit,
					typename make_tree< left, discriminating_bit - 1, history * 10 >::type,
					typename make_tree< right, discriminating_bit - 1, 1 + (history * 10)>::type
					>  type;
			};

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
		template< typename instructions, unsigned long long history>
		struct make_tree<instructions, -2, history, typename boost::enable_if< mpl::not_< mpl::empty< instructions> > >::type>
		{
			BOOST_MPL_ASSERT(( mpl::false_));
		};

		typedef typename make_tree< typename instruction_list::instructions, 11, 0>::type instruction_tree;

		public:
			void feed( instruction_type word)
			{
				call( word,	*this, call_tag< instruction_tree>());
			}

	};
}
#endif //INSTRUCTION_DECODER_INCLUDED