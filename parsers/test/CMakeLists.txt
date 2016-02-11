
# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

find_package (Boost COMPONENTS unit_test_framework REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(TestRfc2616 TestRfc2616.cpp)
target_link_libraries(TestRfc2616 moose_tools ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

