
# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.0)
project(moose_liclib)

set(CMAKE_CXX_FLAGS "-std=c++11 -fvisibility=hidden")

find_package(Boost COMPONENTS random chrono thread system REQUIRED)
find_package(OpenSSL REQUIRED)

set(MOOSE_LICLIB_SRC
	Coder.cpp
	)

set(MOOSE_LICLIB_HDR
	LicLibConfig.hpp
	Coder.hpp
	)

include_directories(
	${CMAKE_SOURCE_DIR}
	${Boost_INCLUDE_DIRS}
	${OpenSSL_INCLUDE_DIR}
)

add_library(${PROJECT_NAME} SHARED ${MOOSE_LICLIB_SRC})

target_link_libraries(${PROJECT_NAME}
	${Boost_LIBRARIES}
	${OPENSSL_CRYPTO_LIBRARY}
)

