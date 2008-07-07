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
	
	const sregex e = repeat<8>( _) >> (s1 = repeat<4>( xdigit)) >> +_s >> ( s2 = repeat<4>( xdigit)) >> !(repeat<6>( _s) >> ( s3 = *_));
	smatch match;
	string last_source;
	while (getline( listing, buffer))
	{
		if (!buffer.empty() && regex_match( buffer, match, e))
		{
			unsigned short address = hex_to_int( match[1]);
			unsigned short instruction = hex_to_int( match[2]);
			result.instructions[ address] = instruction;
			if (match[3])
			{
				result.source_lines[ address] = match[3];
				last_source = match[3];
			}
			else
			{
				result.source_lines[ address] = last_source;
			}
		}
	}

	return result;
}
