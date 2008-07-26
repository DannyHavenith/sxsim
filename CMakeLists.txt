cmake_minimum_required(VERSION 2.6)
MESSAGE( "just checking")
project(sxgo)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE( Boost 1.34.0 COMPONENTS mpl)

IF (Boost_FOUND)
include_directories( Boost_INCLUDE_DIRS)
MESSAGE( "Boost found")
ELSE (Boost_FOUND)
MESSAGE( "could not find boost")
include_directories( "h:/no_backup/boost_src/boost_svn_head")
ENDIF (Boost_FOUND)

add_subdirectory (sxsim)
add_subdirectory (ui)

