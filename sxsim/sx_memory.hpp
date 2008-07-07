#if !defined( SX_MEMORY_INCLUDED)
#define SX_MEMORY_INCLUDED

#include <algorithm>
#include <boost/range.hpp>

namespace micro_emulator 
{

	class sx_rom
	{
	public:
		typedef unsigned short register_t;
		typedef unsigned short address_t;
		static const size_t memory_size = 4 * 1024;

		template< typename Range>
		explicit sx_rom( const Range &r, address_t offset = 0)
		{
			load( r, offset);
		};

		sx_rom()
		{
			std::fill_n( memory, memory_size, 0);
		}

		//
		// load some data (in 'Range') into rom
		//
		template< typename Range>
		void load( const Range &r, address_t offset = 0)
		{
			std::copy( boost::begin(r), boost::end( r), memory + offset);
		}

		//
		// retrieve data word
		//
		register_t operator()( address_t address) const
		{
			return memory[address];
		}

		//
		// set a value in the 'rom'
		// usefull to replace instructions with breakpoints.
		//
		void set( address_t address, register_t value)
		{
			memory[address] = value;
		}

	private:
		register_t memory[memory_size];
	};

	class sx_ram
	{
	public:
		sx_ram()
			:bank_(0)
		{
		}

		// typedefs
		typedef unsigned char register_t;
		typedef unsigned short address_t;

		// public constants
		static const address_t memory_size = 256;
		static const address_t IND	= 0;
		static const address_t RTCC = 1;
		static const address_t PC	= 2;
		static const address_t STATUS = 3;
		static const address_t FSR	= 4;
		static const address_t RA	= 5;
		static const address_t RB	= 6;
		static const address_t RC	= 7;
		static const address_t RD	= 8;
		static const address_t RE	= 9;

		void set_bank( register_t bank)
		{
			// set both FSR (for correctness) and
			// the bank_ member (for speed)
			bank_ = bank * 32;
			memory[FSR] = (memory[FSR] & 0x1f) | (bank << 5);
		}

		void set( address_t address, register_t value)
		{
			cell( address) = value;
		}

		register_t get( address_t address) const
		{
			return cell( address);
		}

		register_t &operator()( address_t address)
		{
			return cell( address);
		}

	private:
		register_t &cell( address_t address)
		{
			if (address == 0)
			{
				return memory[ memory[FSR]];
			}
			else
			{
				return memory[ address + ((address & 0x10)?bank_:0)];
			}
		}

		const register_t &cell( address_t address) const
		{
			if (address == 0)
			{
				return memory[ memory[FSR]];
			}
			else
			{
				return memory[ address + ((address & 0x10)?bank_:0)];
			}
		}

		int bank_;
		register_t memory[ memory_size];

	};

}

#endif //SX_MEMORY_INCLUDED