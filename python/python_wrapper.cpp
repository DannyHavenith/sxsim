#include <boost/python.hpp>
#include "listing_parser.hpp"
#include "sx_simulator.hpp"
#include "sx_state.hpp"

#include <string>
#include <iostream>
#include <fstream>

char const* greet()
{
   return "hello, world";
}

std::ostream &operator<<(std::ostream &strm, const sx_state &state)
{
	return strm;
}

std::ostream &operator<<(std::ostream &strm, const sx_emulator::sx_ram &ram)
{

	return strm;
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

/*    class_<sx_simulator>("Simulator")
		.def( "load_rom", &load_rom)
		.def( "run", &sx_simulator::run)
		.def( "reset", &sx_simulator::reset)
		.def( "set_breakpoint", &sx_simulator::set_breakpoint)
		.add_property( "state", &sx_simulator::get_state, &sx_simulator::set_state);
*/

    class_< sx_emulator::sx_ram >( "sx_ram", init< >() )
        .def(
            "get"
            , (unsigned char ( ::sx_emulator::sx_ram::* )( short unsigned int ) const)( &::sx_emulator::sx_ram::get )
            , ( arg("address") ) )
        .def(
            "get_absolute"
            , (unsigned char ( ::sx_emulator::sx_ram::* )( short unsigned int ) const)( &::sx_emulator::sx_ram::get_absolute )
            , ( arg("address") ) )
        .def(
            "get_internal"
            , (unsigned char ( ::sx_emulator::sx_ram::* )( short unsigned int ) const)( &::sx_emulator::sx_ram::get_internal )
            , ( arg("address") ) )
        /*.def(
            "__call__"
            , (unsigned char & ( ::sx_emulator::sx_ram::* )( short unsigned int ) )( &::sx_emulator::sx_ram::operator() )
            , ( arg("address") )
                /* undefined call policies  )    */
        .def(
            "__call__"
            , (unsigned char const & ( ::sx_emulator::sx_ram::* )( short unsigned int ) const)( &::sx_emulator::sx_ram::operator() )
            , ( arg("address") )
            , return_value_policy< copy_const_reference >() )
        .def(
            "set"
            , (void ( ::sx_emulator::sx_ram::* )( short unsigned int,unsigned char ) )( &::sx_emulator::sx_ram::set )
            , ( arg("address"), arg("value") ) )
        .def(
            "set_absolute"
            , (void ( ::sx_emulator::sx_ram::* )( short unsigned int,unsigned char ) )( &::sx_emulator::sx_ram::set_absolute )
            , ( arg("address"), arg("value") ) )
        .def(
            "set_bank"
            , (void ( ::sx_emulator::sx_ram::* )( unsigned char ) )( &::sx_emulator::sx_ram::set_bank )
            , ( arg("bank") ) );
/*        .def_readonly( "FSR", sx_emulator::sx_ram::FSR )
        .def_readonly( "IND", sx_emulator::sx_ram::IND )
        .def_readonly( "PC", sx_emulator::sx_ram::PC )
        .def_readonly( "RA", sx_emulator::sx_ram::RA )
        .def_readonly( "RB", sx_emulator::sx_ram::RB )
        .def_readonly( "RC", sx_emulator::sx_ram::RC )
        .def_readonly( "RD", sx_emulator::sx_ram::RD )
        .def_readonly( "RE", sx_emulator::sx_ram::RE )
        .def_readonly( "RTCC", sx_emulator::sx_ram::RTCC )
        .def_readonly( "STATUS", sx_emulator::sx_ram::STATUS )
        .def_readonly( "memory_size", sx_emulator::sx_ram::memory_size );
*/
    class_< sx_emulator::sx_rom >( "sx_rom", init< >() )
        .def(
            "__call__"
            , (short unsigned int ( ::sx_emulator::sx_rom::* )( short unsigned int ) const)( &::sx_emulator::sx_rom::operator() )
            , ( arg("address") ) )
        .def(
            "set"
            , (void ( ::sx_emulator::sx_rom::* )( short unsigned int,short unsigned int ) )( &::sx_emulator::sx_rom::set )
            , ( arg("address"), arg("value") ) );

        //.def_readonly( "memory_size", sx_emulator::sx_rom::memory_size );

    class_< sx_light_state >( "sx_light_state" )
        .def_readwrite( "in_interrupt", &sx_light_state::in_interrupt )
        .def_readwrite( "m", &sx_light_state::m )
        .def_readwrite( "pc", &sx_light_state::pc )
        .def_readwrite( "stack", &sx_light_state::stack )
        .def_readwrite( "w", &sx_light_state::w );

    class_< sx_simulator >( "Simulator", init< >() )
        .def(
            "get_state"
            , (::sx_state ( ::sx_simulator::* )(  ) const)( &::sx_simulator::get_state ) )
        .def(
            "reset"
            , (void ( ::sx_simulator::* )(  ) )( &::sx_simulator::reset ) )
        .def(
            "run"
            , (long unsigned int ( ::sx_simulator::* )( long unsigned int ) )( &::sx_simulator::run )
            , ( arg("tick_count") ) )
        .def(
            "set_breakpoint"
            , (void ( ::sx_simulator::* )( short unsigned int,bool ) )( &::sx_simulator::set_breakpoint )
            , ( arg("address"), arg("do_set")=(bool)(true) ) )
        .def(
            "set_state"
            , (void ( ::sx_simulator::* )( ::sx_state const & ) )( &::sx_simulator::set_state )
            , ( arg("new_state") ) )
		.def( "load_rom", &load_rom);


    class_< sx_state, bases< sx_light_state > >( "sx_state" )
        .def_readwrite( "ram", &sx_state::ram );

}

