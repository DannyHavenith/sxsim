
##          Copyright Danny Havenith 2006 - 2009.
## Distributed under the Boost Software License, Version 1.0.
##    (See accompanying file LICENSE_1_0.txt or copy at
##          http://www.boost.org/LICENSE_1_0.txt)

## This is the main CMakeLists file for the sxgo project. It includes
## sxsim, the library, 'ui', the "sxgo!"-ui to sxsim and 'python' the "pysix" python module.


cmake_minimum_required(VERSION 2.6)
#include( configure.cmake)

project(sxgo)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)

##SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE( Boost COMPONENTS python)

IF(${Boost_MINOR_VERSION} GREATER 34 AND NOT WIN32)
	FIND_PACKAGE( Boost COMPONENTS python system)
ENDIF (${Boost_MINOR_VERSION} GREATER 34 AND NOT WIN32)

add_subdirectory (sxsim)
add_subdirectory (ui)
add_subdirectory (python) 

#INSTALL(TARGETS 
#	"${CMAKE_CURRENT_SOURCE_DIR}/test/blink.lst"
#	DESTINATION bin)


set(CPACK_PACKAGE_NAME "sxgo")
set(CPACK_PACKAGE_VENDOR "havenith.net")
set(CPACK_PACKAGE_CONTACT "dh.sxgo@havenith.net")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "sxgo, parallax sx28 emulator")
set(CPACK_PACKAGE_VERSION "0.1.3")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "5")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "sxgo")
set(CPACK_PACKAGE_EXECUTABLES "sxgoui;sxgo emulator")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE_1_0.txt")
include(InstallRequiredSystemLibraries)
include(CPack)
