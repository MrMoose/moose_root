
# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# add_definitions(-DBOOST_TEST_MAIN)
add_executable(TestUnTest TestUnTest.cpp)

target_link_libraries(TestUnTest moose_untest moose_tools ${Boost_LIBRARIES})

add_dependencies(TestUnTest moose_untest moose_tools)
