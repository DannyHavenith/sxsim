#if !defined (LISTING_PARSER_INCLUDED)
#define LISTING_PARSER_INCLUDED
#include <string>
#include <iosfwd>
#include <algorithm>

struct listing_info
{
	listing_info()
	{
		std::fill( instructions, instructions + rom_size, 0x0fff);
	}

	static const size_t rom_size = 4 * 1024;
	std::string source_lines[ rom_size];
	unsigned short instructions[rom_size];
};

listing_info ParseListingFile( std::istream &listing);

#endif