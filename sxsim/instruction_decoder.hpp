#if !defined( INSTRUCTION_DECODER_INCLUDED)
#define INSTRUCTION_DECODER_INCLUDED

#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/lower_bound.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>

namespace micro_emulator 
{
	namespace mpl = boost::mpl;
	namespace fusion = boost::fusion;

	template<typename instruction_list>
	struct instruction_decoder : public instruction_list::impl
	{
	public:
		void feed( int word)
		{
			decode_and_dispatch<instructions_sorted>::call( 
				word,
				word,
				*this
				);
		}

	private:
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

		struct operand_less 
		{
			template< typename instruction1, typename instruction2>
			struct apply :
				mpl::less< 
				typename instruction1::operand_size,
				typename instruction2::operand_size
				>
			{};
		};

		struct insert_sorted
		{
			template< typename sequence, typename instruction>
			struct apply:
				mpl::insert<
				sequence,
				typename mpl::lower_bound< sequence, instruction, operand_less>::type,
				instruction
				>
			{};
		};

		template <typename type>
		struct pair_first_is
		{
			template< typename subject>
			struct apply:
				boost::is_same< type, typename subject::first> {};
		};

		/*
		typedef typename mpl::fold< 
			typename instruction_list::instructions, 
			mpl::vector0<>, 
			insert_sorted 
		>::type instructions_sorted;
	*/
		typedef typename instruction_list::instructions instructions_sorted;

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

		template< 
			typename sorted_instruction_seq, 
			int previous_operand_size = 0>
		struct decode_and_dispatch
		{
			static void call( int instruction, int full_instruction, impl &implementation)
			{
				typedef mpl::front< sorted_instruction_seq>::type considered;
				typedef mpl::pop_front< sorted_instruction_seq>::type next_sequence;
				const int operand_size = considered::word::operand_size::value;

				if (previous_operand_size != considered::word::operand_size::value)
				{
					instruction &= ((~0) << operand_size);
				}
				if ( instruction == ((considered::word::opcode) << operand_size))
				{
					dispatch<considered>( full_instruction, implementation);
				}
				else
				{
					decode_and_dispatch< next_sequence, operand_size>::call( instruction, full_instruction, implementation);
				}
			}
		};

		template< int previous_operand_size>
		struct decode_and_dispatch< mpl::vector0<>, previous_operand_size>
		{
			static void call( int instruction, int full_instruction, impl &implementation)
			{
				implementation.unknow_instruction( instruction);
			}
		};

	};
}
#endif //INSTRUCTION_DECODER_INCLUDED