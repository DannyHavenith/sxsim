
//          Copyright Danny Havenith 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// This file contains a small python wrapper around the sx_simulator class

#include <boost/python.hpp>
#include <boost/python/object.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "listing_parser.hpp"
#include "sx_simulator.hpp"
#include "sx_state.hpp"
#include "sx_cluster.hpp"
#include "simulated_devices.hpp"

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>



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

void save_state(sx_state &state, const std::string &filename)
{
	using namespace boost::archive;
	std::ofstream file( filename.c_str());
	xml_oarchive arch( file);
	arch << BOOST_SERIALIZATION_NVP( state);
}

void load_state(sx_state &state, const std::string &filename)
{
	using namespace boost::archive;
	std::ifstream file( filename.c_str());
	xml_iarchive arch( file);
	arch >> BOOST_SERIALIZATION_NVP( state);
}

void set_memory_handler( sx_simulator &sim, int address, boost::python::object function_object)
{
	sim.on_memory_write( address, function_object);
}

BOOST_PYTHON_MODULE(pysix)
{
    using namespace boost::python;
	using namespace sx_emulator;

    class_<listing_info>("ListingInfo")
		.def( "GetLabelAddress",			&::listing_info::GetLabelAddress,
			arg("label"))

    ;

    def( "ParseListingFile", &ParseListingFromFilename);

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
        .def_readwrite( "w",			&sx_light_state::w )
        ;

    class_< sx_state, bases< sx_light_state > >( "sx_state" )
        .def_readwrite( "ram", &sx_state::ram )
        .def( "save", &save_state)
        .def( "load", &load_state)
		;

	class_< sx_simulator >( "Emulator", init< >() )
		.add_property( "state", &sx_simulator::get_state, &sx_simulator::set_state)
        .def( "reset",			&::sx_simulator::reset)
        .def( "run",			&::sx_simulator::run,  arg("tick_count"))
        .def( "set_breakpoint", &::sx_simulator::set_breakpoint,
			(arg("address"), arg("do_set")=(bool)(true) ) )
		.def( "on_memory_write",&set_memory_handler,
			(arg("address"), arg("handler")))
		.def( "load_rom",		&load_rom)
		;

	class_<sx_cluster>( "Cluster")
		.def( "new_controller", &sx_cluster::new_controller)
		.def( "run", &sx_cluster::run)
		;

    class_<sx_device, boost::shared_ptr<sx_device> >( "Device")
        .def( "set_byte_value", (void (sx_device::*)( const std::string &, unsigned char)) &sx_device::set_value)
        .def( "set_bit_value", (void (sx_device::*)( const std::string &, bool)) &sx_device::set_value)
        .def( self_ns::str(self))
        ;

    def( "CreateDevice", &create_device);

}

