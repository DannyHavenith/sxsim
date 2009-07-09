
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// implementation of SX rom and ram behaviour.

#if !defined( SX_MEMORY_INCLUDED)
#define SX_MEMORY_INCLUDED

#include <algorithm>
#include <boost/range.hpp>
#include <boost/function.hpp>
namespace sx_emulator 
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
		// useful to replace instructions with breakpoints.
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
		{
			reset();
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
			memory[FSR] = (memory[FSR] & 0x1f) | (bank << 5);
		}

		void reset() 
		{
			std::fill_n(memory, memory_size, 0x5A);
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

		const register_t &operator()( address_t address) const
		{
			return cell( address);
		}

		register_t get_internal( address_t address) const
		{
			return memory[address];
		}

		register_t get_absolute( address_t address) const
		{
			return memory[ absolute_to_internal( address)]; 
		}

		void set_absolute( address_t address, register_t value)
		{
			memory[ absolute_to_internal( address)] = value;
		}
		
		/// translate from 0x00-0x1F to an actual memory location.
		/// internal memory locations are 0x00-0x1F, 0x30-0x3f, 0x50-0x5f etc.
		address_t limited_to_internal( address_t limited) const
		{
			return limited == 0?
					absolute_to_internal(memory[FSR])
				:	(limited + ((limited & 0x10)?(0xe0 & memory[FSR]):0));
		}
	private:

		static address_t absolute_to_internal( address_t maddress) 
		{
			return (maddress&0x10)?maddress:(maddress & 0x0f);
		}

		register_t &cell( address_t address)
		{
			return memory[limited_to_internal( address)];
		}

		const register_t &cell( address_t address) const
		{
			if (address == 0)
			{
				return memory[ memory[FSR]];
			}
			else
			{
				return memory[ address + ((address & 0x10)?(0xe0 & memory[FSR]):0)];
			}
		}

		register_t memory[ memory_size];
		boost::function<void( address_t, register_t)> write_handler[memory_size];
	};

}

#endif //SX_MEMORY_INCLUDED
