#if !defined (LISTING_PARSER_INCLUDED)
#define LISTING_PARSER_INCLUDED
#include <string>
#include <vector>
#include <iosfwd>
#include <algorithm>
#include <map>

struct listing_info
{
	/* TODO: extend the line information at some time.
	struct line
	{
		line(
			const std::string &a,
			const std::string &c,
			const std::string &s)
			:address(a), code(c), source(s)
		{
		}
		std::string address;
		std::string code;
		std::string source;
	}
	*/

	listing_info()
	{
		std::fill( instructions, instructions + rom_size, 0x04f);
		std::fill( address_to_line, address_to_line + rom_size, 0);
	}

	unsigned short GetNearestAddress( int line) const
	{
		unsigned short nearest_address = 0;
		int nearest_line = 0;
		// simple implementation for now:
		// linear search.
		for ( int index = 0; index < rom_size; ++index)
		{
			int current_line = address_to_line[index];
			if ( current_line <= line && current_line > nearest_line)
			{
				nearest_line = current_line;
				nearest_address = index;
			}
		}

		return nearest_address;
	}

	int GetLine( unsigned short address) const
	{
		// todo: assert.
		if (address >= rom_size) return 0;

		return address_to_line[address];
	}

	static const size_t rom_size = 4 * 1024;
	typedef std::map< std::string, int> label_container_type;
	typedef std::pair< std::string, int> rom_label;
	typedef std::vector< rom_label> rom_label_container_type;
	struct major_rom_label
	{
		major_rom_label()
			:line(0)
		{
		}

		major_rom_label( const std::string &name_, int line_)
			:name( name_), line( line_)
		{
		}

		/*
		major_rom_label( const major_rom_label &rhs)
			: name( rhs.name), line( rhs.line)
		{
		}

		major_rom_label &operator=( const major_rom_label &rhs)
		{
			name = rhs.name;
			line = rhs.line;
		}
*/
		std::string name;
		int line;

		rom_label_container_type minor_labels;
	};
	typedef std::vector< major_rom_label> jumplabel_container_type;

	label_container_type	 data_labels;
	jumplabel_container_type jump_labels;
	int address_to_line[ rom_size];
	unsigned short instructions[rom_size];
	std::vector< std::string> lines;
};

listing_info ParseListingFile( std::istream &listing);

#endif
