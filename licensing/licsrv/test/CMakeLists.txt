# Copyright 2015 Stephan Menzel. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

find_package (Boost COMPONENTS unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(TestCoder TestCoder.cpp)
target_link_libraries(TestCoder moose_liclib moose_licsrv ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

