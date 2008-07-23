#if !defined (LISTING_PARSER_INCLUDED)
#define LISTING_PARSER_INCLUDED
#include <string>
#include <vector>
#include <iosfwd>
#include <algorithm>

struct listing_info
{
	/* TODO: later
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
		std::fill( instructions, instructions + rom_size, 0x0fff);
	}

	static const size_t rom_size = 4 * 1024;
	int address_to_line[ rom_size];
	std::vector< std::string> lines;
	unsigned short instructions[rom_size];
};

listing_info ParseListingFile( std::istream &listing);

#endif
