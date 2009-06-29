
##          Copyright Danny Havenith 2006 - 2009.
## Distributed under the Boost Software License, Version 1.0.
##    (See accompanying file LICENSE_1_0.txt or copy at
##          http://www.boost.org/LICENSE_1_0.txt)

## This is the main CMakeLists file for the sxgo project. It includes
## sxsim, the library, 'ui', the "sxgo!"-ui to sxsim and 'python' the "pysix" python module.


cmake_minimum_required(VERSION 2.6)
#include( configure.cmake)

project(sxgo)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

##SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE( Boost COMPONENTS python)


IF (Boost_FOUND)
    include_directories( ${Boost_INCLUDE_DIRS})
    MESSAGE( "Boost found")
ELSE (Boost_FOUND)
    MESSAGE( "could not find boost, using manual configuration")
    include_directories( ${Boost_ROOT_DIR} )
    link_directories( ${Boost_LIBRARY_DIR})
ENDIF (Boost_FOUND)

add_subdirectory (sxsim)
add_subdirectory (ui)
add_subdirectory (python) 


set(CPACK_PACKAGE_NAME "sxgo")
set(CPACK_PACKAGE_VENDOR "havenith.net")
set(CPACK_PACKAGE_CONTACT "dh.sxgo@havenith.net")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "sxgo, parallax sx28 emulator")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "sxgo")

include(InstallRequiredSystemLibraries)
include(CPack)
