
# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.0)
project(moose_parsers)

find_package(Boost COMPONENTS random chrono thread system REQUIRED)

set(MOOSE_TOOLS_SRC
	Rfc2616.cpp
	)

set(MOOSE_TOOLS_HDR
	MooseParsersConfig.hpp
	Rfc2616.cpp
	)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${Boost_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} SHARED ${MOOSE_TOOLS_SRC})

target_link_libraries(${PROJECT_NAME}
	${Boost_LIBRARIES}
)

add_subdirectory(test)
enable_testing()
add_test(NAME Parsers    COMMAND test/TestParsers )


