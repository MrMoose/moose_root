
# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#cmake_minimum_required(VERSION 3.0)
#project(moose_untest)

#find_package(Boost 1.56.0 REQUIRED COMPONENTS random chrono thread system)

set(MOOSE_UNTEST_SRC
	UnTest.cpp
	)

set(MOOSE_UNTEST_HDR
	UnTestConfig.hpp
	UnTest.hpp
	)

include_directories(
	${CMAKE_SOURCE_DIR}
	${Boost_INCLUDE_DIRS}
)

if (${UNREAL})
	add_library(moose_untest STATIC ${MOOSE_UNTEST_SRC})
else()
	add_library(moose_untest SHARED ${MOOSE_UNTEST_SRC})
endif()

target_link_libraries(moose_untest
	moose_tools
	${Boost_LIBRARIES}
)

add_dependencies(moose_untest moose_tools)

add_subdirectory(test)
enable_testing()
add_test(NAME UnTest COMMAND test/TestUnTest  )


