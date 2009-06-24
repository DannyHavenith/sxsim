import os
from pyplusplus import module_builder

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( ["../sxsim/sx_simulator.hpp", "../sxsim/listing_parser.hpp"]
                                      , gccxml_path=r"C:/Program Files (x86)/gccxml 0.9/bin/gccxml.exe" 
                                      , working_directory=r"."
                                      , include_paths=[ u'D:/Programming/boost_1_39_0', u'../sxsim']
                                      , define_symbols=[] )


#Well, don't you want to see what is going on?
mb.print_declarations()

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='pysix' )

#Writing code to file.
mb.write_module( './bindings.cpp' )

