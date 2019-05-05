
# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)
cmake_policy(SET CMP0074 NEW)

project(moose_root)

enable_testing()

# Some global setups for all platforms
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(MOOSE_STATIC_BUILD TRUE CACHE BOOL "Configure for static linkage. Mostly, except for Qt")

# Set globally now
set(CMAKE_CXX_STANDARD 14)

set(TP $ENV{THIRD_PARTY_ROOT})
if (NOT TP)
    message(FATAL_ERROR "When using this configuration you must specify 
                         the THIRD_PARTY_ROOT environment variable")
endif()

set(THIRD_PARTY_ROOT $ENV{THIRD_PARTY_ROOT}
        CACHE PATH "Root path of 3rd party libraries")
file(TO_CMAKE_PATH ${THIRD_PARTY_ROOT} THIRD_PARTY_ROOT)

if (MOOSE_STATIC_BUILD)
	set(BUILD_SHARED_LIBS OFF)
else()
	set(BUILD_SHARED_LIBS ON)
endif()

####################################################################
#
# Inital setup done, start setting up generals
#
####################################################################

# Setup Boost Directories. 
# The CMake Module appears to require that
set(BOOST_ROOT ${THIRD_PARTY_ROOT}/boost-1.70.0 CACHE PATH "set root of boost install")
set(BOOST_INCLUDEDIR ${BOOST_ROOT}/include CACHE PATH "set boost includes")
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib CACHE PATH "set boost library dir")
set(Boost_ADDITIONAL_VERSIONS "1.70.0")

# Boost specific settings for fake debug DLL runtime static libs
set(Boost_USE_STATIC_LIBS           ON) # only find static libs
set(Boost_USE_MULTITHREADED         ON)
set(Boost_USE_STATIC_RUNTIME       OFF)
set(Boost_USE_DEBUG_LIBS           OFF) # we are fake debug
set(Boost_USE_DEBUG_RUNTIME        OFF) # what he said


# We always use Release runtime for Debug builds
# as this is what Epic does and expects. AKA Fake Debug
if (MSVC)
    set(CFLAGS ${CMAKE_C_FLAGS_DEBUG})
    set(CXXFLAGS ${CMAKE_CXX_FLAGS_DEBUG})

    string(REPLACE "MDd" "MD" CFLAGS "${CFLAGS}")
    string(REPLACE "MDd" "MD" CXXFLAGS "${CXXFLAGS}")

    string(REPLACE "/D_DEBUG" "" CFLAGS "${CFLAGS}")
    string(REPLACE "/D_DEBUG" "" CXXFLAGS "${CXXFLAGS}")

    string(APPEND CFLAGS   " /DMOOSE_DEBUG")
    string(APPEND CXXFLAGS " /DMOOSE_DEBUG")

    set(CMAKE_C_FLAGS_DEBUG "${CFLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CXXFLAGS}")

	# Add some more switches to try to speed up compile times
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG:FASTLINK /INCREMENTAL")
	set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /DEBUG:FASTLINK /INCREMENTAL")

	# Use multiple cores. You can reduce the number in Tools -> Options -> Project Settings
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /DNOMINMAX /DWIN32_LEAN_AND_MEAN")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /DNOMINMAX /DWIN32_LEAN_AND_MEAN")
	
    # Turn off Boost's auto linking in order to link the now 'fake' debug against release boost
    add_definitions(-DBOOST_ALL_NO_LIB)
else()
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMOOSE_DEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMOOSE_DEBUG")
endif()



####################################################################
#
# General setup done, start pulling in dependencies
#
####################################################################

find_package(Boost REQUIRED
    COMPONENTS
    atomic
    filesystem
    random
    chrono
    log
    date_time
    regex
    thread
    system
    exception
    program_options
    unit_test_framework
)

# Yeah, threads too. I guess this is not needed on Linux        
find_package(Threads)




####################################################################
#
# Dependencies done, set some compiler definitions
#
####################################################################
add_definitions(-DBOOST_MAJOR=${Boost_MAJOR_VERSION})
add_definitions(-DBOOST_MINOR=${Boost_MINOR_VERSION})
add_definitions(-DMOOSE_TOOLS_LOG_FILE_DIR="/var/log/moose")
add_definitions(-DMOOSE_TOOLS_LOG_FILE_NAME="moose_tools.log")

if (MSVC)
    add_definitions(-DWINVER=0x0A00)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -pthread -std=c++14 -fvisibility=hidden")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(CMAKE_BUNDLE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bundle/$<CONFIG>)




add_subdirectory(tools)
#add_subdirectory(monty)
add_subdirectory(mredis)
add_subdirectory(rescue)



