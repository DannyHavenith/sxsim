#include <boost/python.hpp>
#include "listing_parser.hpp"
#include "sx_simulator.hpp"

#include <string>
#include <iostream>
#include <fstream>

char const* greet()
{
   return "hello, world";
}

listing_info ParseListingFromFilename( const std::string &name)
{
	std::ifstream file( name.c_str());
	if (!file)
	{
		throw std::runtime_error("file does not exist");
	}

	return ParseListingFile( file);
}

bool load_rom( sx_simulator &sim, const listing_info &listing)
{
	return sim.load_rom( listing.instructions);
}

BOOST_PYTHON_MODULE(pysix)
{
    using namespace boost::python;
    class_<listing_info>("ListingInfo");

    def( "ParseListingFile", ParseListingFromFilename);

    class_<sx_simulator>("Simulator")
		.def( "load_rom", &load_rom)
		.def( "run", &sx_simulator::run)
		.def( "reset", &sx_simulator::reset);
}

