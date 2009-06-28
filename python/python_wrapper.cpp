#include <boost/python.hpp>
#include "listing_parser.hpp"
#include "sx_simulator.hpp"
#include "sx_state.hpp"

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

char const* greet()
{
   return "hello, world";
}

std::ostream &operator<<(std::ostream &strm, const sx_state &state)
{
	return strm;
}

namespace sx_emulator
{

std::ostream &operator<<(std::ostream &strm, const sx_ram &ram)
{
	using namespace std;
	static const char *rowlabels[] =
	{ "0 ", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "A ", "B ", "C ", "D ", "E ", "F "};

	strm << "  0X 1x 3x 5x 7x 9x Bx Dx Fx\n";
	for (int row = 0; row < 16; ++row) {
		strm << rowlabels[row];
		strm << hex << setw(2) << int( ram.get_absolute( row)) << ' ';
		for (int col = 1; col < 16; col+=2) {
			strm << hex << setw(2) << int(ram.get_absolute( col * 16 + row)) << ' ';
		}
		strm << endl;
	}

	return strm;
}
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

    class_< sx_emulator::sx_ram >( "sx_ram", init< >() )
        .def( "get",			&::sx_emulator::sx_ram::get, 
				arg("address"))
        .def( "get_absolute",	&::sx_emulator::sx_ram::get_absolute, 
				arg("address"))
        .def( "get_internal",	&::sx_emulator::sx_ram::get_internal, 
				arg("address"))
        .def( "__call__",		(unsigned char const & ( ::sx_emulator::sx_ram::* )( short unsigned int ) const)&::sx_emulator::sx_ram::operator(),
				arg("address"),
				return_value_policy< copy_const_reference >() )
        .def( "set",			&::sx_emulator::sx_ram::set, 
				( arg("address"), arg("value") ) )
        .def( "set_absolute",	&::sx_emulator::sx_ram::set_absolute, 
				( arg("address"), arg("value") ) )
        .def( "set_bank",		&::sx_emulator::sx_ram::set_bank, 
				arg("bank"))
        .def( self_ns::str(self))
		;

    class_< sx_emulator::sx_rom >( "sx_rom", init< >() )
        .def( "__call__",		&::sx_emulator::sx_rom::operator(), arg("address") )
        .def( "set",			&::sx_emulator::sx_rom::set,		( arg("address"), arg("value") ) )
		;

    class_< sx_light_state >( "sx_light_state" )
        .def_readwrite( "in_interrupt", &sx_light_state::in_interrupt )
        .def_readwrite( "m",			&sx_light_state::m )
        .def_readwrite( "pc",			&sx_light_state::pc )
        .def_readwrite( "stack",		&sx_light_state::stack )
        .def_readwrite( "w",			&sx_light_state::w );

    class_< sx_state, bases< sx_light_state > >( "sx_state" )
        .def_readwrite( "ram", &sx_state::ram )
		;

	class_< sx_simulator >( "Simulator", init< >() )
		.add_property( "state", &sx_simulator::get_state, &sx_simulator::set_state)
        .def( "reset",			&::sx_simulator::reset)
        .def( "run",			&::sx_simulator::run,  arg("tick_count"))
        .def( "set_breakpoint", &::sx_simulator::set_breakpoint, (
				arg("address"), arg("do_set")=(bool)(true) ) )
		.def( "load_rom",		&load_rom)
		;


}

