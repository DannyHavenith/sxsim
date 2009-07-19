
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// implementation file for the sx_controller wrapper.
/// This is the source file that will trigger most templates to be instantiated.

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50

#include <iostream>
#include <fstream>
#include "listing_parser.hpp"
#include "sx_instruction_list.hpp"
#include "instruction_decoder.hpp"
#include "sx_controller.hpp"

#include "sx_simulator.hpp"

#ifdef GENERATE_GRAPHVIZ_INSTRUCTION_TREE
#include "sx_instruction_names.hpp"
struct graphviz_printer
{
	typedef micro_emulator::instruction_decoder< sx_emulator::sx_instruction_list, sx_emulator::sx_controller> decoder;

private:
	static void declare_branch_node( std::ostream &strm, int id)
	{
		strm << "  node_" << id << "[label=\"\", shape=diamond];\n";
	}

	static void declare_edge( std::ostream &strm, int source, const std::string &label, int dest)
	{
		strm << " node_" << source << " -> node_" << dest << " [label =\"" << label << "\"];\n";
	}

	static void declare_leaf_node( std::ostream &strm, int id, const std::string &name)
	{
		strm << " node_" << id << " [ label=\"" << name << "\"];\n";
	}

	template< int bit, typename on_zero, typename on_one>
	static int print( std::ostream &stream, int &id, int parent_bit,
			const decoder::decide_node< bit, on_zero, on_one> &)
	{
		int my_id = ++id; // create our own unique id

		// if there's a gap between the bit that our parent tests
		// and the gap that we test, insert extra 'don't care' nodes.
		declare_branch_node( stream, my_id);
		int return_id = my_id;
		while( parent_bit > bit + 1)
		{
			int extra_node_id =  ++id;
			declare_branch_node( stream, extra_node_id);
			declare_edge( stream, extra_node_id, "x", return_id);
			return_id = extra_node_id;
			--parent_bit;
		}
		int left_id = print( stream, id, bit, on_zero());
		int right_id = print( stream, id, bit, on_one());
		declare_edge( stream, my_id, "0", left_id);
		declare_edge( stream, my_id, "1", right_id);
		return return_id;

	}

	template< typename tag>
	static int print( std::ostream &stream, int &id, int /*ignored*/,
			const decoder::call_node< tag> &)
	{
		int my_id = ++id;
		declare_leaf_node( stream, id, sx_emulator::instruction_name< tag>::value());
		return my_id;
	}

public:
	static void print( std::ostream &strm)
	{
		int id_generator = 0;
		strm << "digraph G {\n graph [rankdir=\"LR\"]\n";
		print( strm, id_generator, 12, decoder::instruction_tree());
		strm << "}\n";
	}
};
#endif

sx_simulator::sx_simulator()
:emulator( new emulator_type())
{
//	std::ofstream graphviz_file("instruction_tree.txt");
//	graphviz_printer::print( graphviz_file);

}

sx_simulator::sx_simulator( sx_simulator::emulator_pointer_type emulator_)
:emulator( emulator_)
{
}

//
// to be replaced with a load_rom, in a future where
// rom contents can be grabbed from other sources, such as
// obj-files...
//
bool sx_simulator::load_rom( const unsigned short (&instructions)[4096])
{
	emulator->load_rom(	instructions);
	return true;
}

void sx_simulator::set_breakpoint( address_type address, bool do_set)
{
	if (do_set)
	{
		emulator->set_breakpoint( address);
	}
	else
	{
		emulator->remove_breakpoint( address);
	}
}

void  sx_simulator::on_memory_write(
	sx_simulator::address_type address,
	boost::function< void(  sx_simulator::address_type, sx_simulator::register_type )> handler
	)
{
	emulator->on_memory_access( address, handler);
}

/// get profiling information: a count of how often each instruction was run
const sx_simulator::histogram_type &sx_simulator::get_histogram() const
{
	return emulator->get_histogram();
}

/// set all statistics to zero
void sx_simulator::reset_histogram()
{
	emulator->reset_histogram();
}

unsigned long sx_simulator::run( unsigned long tick_count)
{
	return emulator->tick( tick_count);
}

void sx_simulator::reset()
{
	emulator->reset();
}

sx_simulator::state sx_simulator::get_state() const
{
	return emulator->get_state();
}

void sx_simulator::set_state( const sx_simulator::state &new_state)
{
	emulator->set_state( new_state);
}

