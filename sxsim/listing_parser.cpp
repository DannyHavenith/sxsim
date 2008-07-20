#include <boost/xpressive/xpressive.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>
#include <sstream>

#include "listing_parser.hpp"

using namespace std;
using namespace boost;
using namespace boost::xpressive;

unsigned short hex_to_int(string str)
{
	unsigned short num;
	istringstream i(str);
	i >> hex >> num;
	return num;
}

listing_info ParseListingFile( std::istream &listing)
{
	listing_info result;
	string buffer;
	
	const sregex e = repeat<8>( _) >> (s1 = repeat<4>( xdigit)) >> +_s >> ( s2 = repeat<4>( xdigit)) >> !(+_s >> ( s3 = repeat<4>( xdigit))) >> !(+_s >> ( s4 = repeat<4>( xdigit))) >> !(+_s >> ( s5 = repeat<4>( xdigit))) >> !(repeat<6>( _s) >> ( s6 = *_));
	smatch match;
	int last_source = 0;
	int current_source = 0;
	while (getline( listing, buffer))
	{
		if (!buffer.empty() && regex_match( buffer, match, e))
		{
			unsigned short address = hex_to_int( match[1]);
			if (match[6])
			{
				result.source_lines[ address] = current_source;
				last_source = current_source;
			}
			int n = 2;
			while (n<6 && match[n])
			{
				unsigned short instruction = hex_to_int( match[n]);
				result.instructions[ address] = instruction;
				result.source_lines[ address] = last_source;
				++address;
				++n;
			}
		}
		++current_source;
	}

	return result;
}
