# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.0)
project(moose_licsrv)

set(CMAKE_CXX_FLAGS "-std=c++11 -fvisibility=hidden")

find_package(Boost COMPONENTS random chrono thread system REQUIRED)

set(MOOSE_LICSRV_SRC
	Client.cpp
	Server.cpp
	FileData.cpp
	License.cpp
	Licenses.cpp
	)

set(MOOSE_LICSRV_HDR
	LicSrvConfig.hpp
	Client.hpp
	Server.hpp
	FileData.hpp
	License.hpp
	Licenses.hpp
	)

include_directories(
	${moose_liclib_SOURCE_DIR}/../
	${CMAKE_SOURCE_DIR}
	${Boost_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} SHARED ${MOOSE_LICSRV_SRC})
target_link_libraries(${PROJECT_NAME}
	moose_liclib
	moose_tools
	${Boost_LIBRARIES}
)

add_executable(license_server main.cpp)

target_link_libraries(license_server
	${PROJECT_NAME}
	${Boost_LIBRARIES}
)


add_subdirectory(test)
enable_testing()
add_test(NAME Coder COMMAND test/TestCoder)



