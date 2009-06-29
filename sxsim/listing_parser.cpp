
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// implementation of the listing parser 

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

	const sregex labels =
		repeat<24>( _)
		>>	!( s1 = as_xpr(':'))
		>> (s2 = +(~(xpressive::set = ';', ' ', '\t', '\n', '\r')))
		>> *_s
		>> ( s3 = *_);

	//
	// keywords that, if they follow a label, will make the parser ignore the label as a "jump-to" address.
	const sregex data_labels =
		icase( as_xpr( "equ") | "ds"|"macro"|"dw"|"=")
		>> *_;

	const sregex ignored_labels =
			repeat<4>( xdigit)
		|	icase(
					as_xpr("ifdef")
				|	"ifndef"
				|	"endif"
				|	"else"
			);

	smatch match;
	int last_source = 0;
	int current_source = 0;
	listing_info::major_rom_label *current_major_label = 0;

	while (getline( listing, buffer))
	{
		// if we find a regular line with assembly code,
		// add the code to the 'instructions' array and relate the address
		// to the current line in the listing.
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
			result.data_labels[ match[2]]= hex_to_int( match[1]);
		}

		// if we find a label of any sorts, add it to the label information
		if ( regex_match( buffer, match, labels))
		{
			if ((!match[3] || !regex_match( match[3].str(), data_labels)) && !regex_match( match[2].str(), ignored_labels))
			{
				if (match[1] && current_major_label)
				{
					// minor label (starting with ':')
					current_major_label->minor_labels.push_back( listing_info::rom_label(match[2], current_source));
				}
				else
				{
					result.jump_labels.push_back(listing_info::major_rom_label( match[2], current_source));
					current_major_label = &result.jump_labels.back();
				}
			}
		}

		result.lines.push_back( buffer);
		++current_source;
	}

	return result;
}
