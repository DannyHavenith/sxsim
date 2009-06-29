
##          Copyright Danny Havenith 2006 - 2009.
## Distributed under the Boost Software License, Version 1.0.
##    (See accompanying file LICENSE_1_0.txt or copy at
##          http://www.boost.org/LICENSE_1_0.txt)

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

ADD_LIBRARY(pysix SHARED
	python_wrapper.cpp
	)

# make sure the library is called "pysix" and not "libpysix".
SET_TARGET_PROPERTIES(pysix PROPERTIES PREFIX "")

#on windows, the library really needs to have a ".pyd" extension
IF(WIN32)
	SET_TARGET_PROPERTIES(pysix PROPERTIES SUFFIX  ".pyd")
ENDIF(WIN32)

# Make sure the linker can find the sxsim library once it is built.
link_directories (${sxgo_BINARY_DIR}/sxsim)
include_directories( ${sxgo_SOURCE_DIR}/sxsim)

target_link_libraries (pysix sxsim ${PYTHON_LIBRARY} ${Boost_PYTHON_LIBRARY})

INSTALL(TARGETS 
	pysix
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
	