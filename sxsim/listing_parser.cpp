#include <boost/xpressive/xpressive.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>
#include <sstream>

#include "listing_parser.hpp"

using namespace std;
using namespace boost;
using namespace boost::xpressive;

unsigned short hex_to_int(const string &str)
{
	unsigned short num;
	istringstream i(str);
	i >> hex >> num;
	return num;
}


listing_info ParseListingFile( istream &listing)
{
	listing_info result;
	string buffer;

	//
	// regular expressions that matches lines of the form:
	// linenr hex-address hex-codes assembly-source
	const sregex e = 
				repeat<8>( _)						// ignore first 8 characters (linenumers)
			>>	(s1 = repeat<4>( xdigit)) >> +_s	// hex address
			>>  (	s2 = repeat<4>( xdigit))		// hex assembly code
			>> !(+_s >> ( s3 = repeat<4>( xdigit))) // optionally, more assembly codes (DW directive)
			>> !(+_s >> ( s4 = repeat<4>( xdigit))) 
			>> !(+_s >> ( s5 = repeat<4>( xdigit))) 
			>> !(repeat<6>( _) >> ( s6 = *_))		// source code line.
			>> *_;									// eat anything that's left (newlines, etc)

	// regular expression that finds DS directives (and their address)
	const sregex ds = 
				repeat<8>( _)						// ignore first 8 characters (linenumers)
			>>  ('=' >> (s1 = +xdigit))
			>>  +_s >> ( s2 = +(~_s))
			>>  +_s >> icase("ds")
			>>  *_;

	smatch match;
	int last_source = 0;
	int current_source = 0;
	while (getline( listing, buffer))
	{
		if (!buffer.empty() && regex_match( buffer, match, e))
		{
			if (match[1])
			{
				unsigned short address = hex_to_int( match[1]);
				result.address_to_line[ address] = current_source;
				last_source = current_source;

				int n = 2;
				while (n<6 && match[n])
				{
					unsigned short instruction = hex_to_int( match[n]);
					result.instructions[ address] = instruction;
					result.address_to_line[ address] = last_source;
					++address;
					++n;
				}
			}
		}

		// if we find a data label (DS directive) add it to the info.
		if ( regex_match( buffer, match, ds))
		{
			result.data_labels[ match[2]]= hex_to_int( string( match[1]));
		}

		result.lines.push_back( buffer);
		++current_source;
	}

	return result;
}
