
# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.0)
project(moose_root)

set(UNREAL TRUE CACHE BOOL "Configure for Unreal usage")

if (${UNREAL})
	set(THRID_PARTY_ROOT $ENV{THIRD_PARTY_UNREAL})
	# those are all Unreal specific settings
	set(Boost_USE_STATIC_LIBS           ON) # only find static libs
	set(Boost_USE_MULTITHREADED         ON)
	set(Boost_USE_STATIC_RUNTIME       OFF)
	set(Boost_ADDITIONAL_VERSIONS "1.60.0")

	set(BOOST_ROOT "${THRID_PARTY_ROOT}/boost-1.60.0"
			CACHE PATH "set root of boost install")
	set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include"
			CACHE PATH "set boost includes")
	set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib"
			CACHE PATH "set boost library dir")
	set(UNREAL_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install"
			CACHE PATH "Prefix prepended to install directories")
	set(CMAKE_INSTALL_PREFIX "${UNREAL_INSTALL_PREFIX}"
			CACHE INTERNAL "Prefix prepended to install directories" FORCE)

	find_package(Threads)

else()
	set(THRID_PARTY ENV{THIRD_PARTY})
endif()


if (MSVC)

else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -DPIC -pthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DPIC -pthread -std=c++11 -fvisibility=hidden")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)



find_package(Boost COMPONENTS random chrono thread system unit_test_framework REQUIRED)


add_subdirectory(tools)
#add_subdirectory(parsers)
#add_subdirectory(hpx)
#add_subdirectory(licensing)
#add_subdirectory(monty)

add_subdirectory(untest)


