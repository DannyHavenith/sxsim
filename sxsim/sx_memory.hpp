#if !defined( SX_MEMORY_INCLUDED)
#define SX_MEMORY_INCLUDED

namespace micro_emulator 
{

	class sx_memory
	{
	public:
		// typedefs
		typedef unsigned char register_t;
		typedef usigned short address_t;

		// public constants
		static const address_t memory_size = 256;
		static const address_t FSR = 4;

		void set_bank( register_t bank)
		{
			bank_ = bank;
		}

		void set( address_t address, register_t value)
		{
			cell( address) = value;
		}

		register_t get( address_t address) const
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
				return memory[ address + bank_ * (address & 0xf0)];
			}
		}

		register_t memory[ memory_size];

	};

}

#endif //SX_MEMORY_INCLUDED