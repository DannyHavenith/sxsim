import os
from pyplusplus import module_builder

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( ["../sxsim/sx_simulator.hpp", "../sxsim/listing_parser.hpp"]
                                      
                                      , working_directory=r"."
                                      , include_paths=[  u'../sxsim']
                                      , define_symbols=[] )


#Well, don't you want to see what is going on?
mb.print_declarations()

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='pysix' )

#Writing code to file.
mb.write_module( './bindings.cpp' )

