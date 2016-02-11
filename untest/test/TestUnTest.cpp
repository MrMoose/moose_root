
//  Copyright 2015 Stephan Menzel. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE UnrealTests
#include <boost/test/unit_test.hpp>

#include "../UnTest.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

using namespace boost::accumulators;

BOOST_AUTO_TEST_CASE(RunAndStart) {
	
	moose::untest::UnTest untest;
	boost::uint64_t c = 0;

	// see starting point
	BOOST_CHECK(untest.current_int() == 0);
	BOOST_CHECK_CLOSE(untest.current_float(), 0, 0.1);


	// say start
	BOOST_CHECK_NO_THROW(untest.start(1000));

	boost::this_thread::sleep_for(boost::chrono::milliseconds(300));
	
	// another start shall not hurt at all
	BOOST_CHECK_NO_THROW(untest.start());

	// now let's sleep a little. Value should still be 0
	boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
	BOOST_CHECK_NO_THROW(c = untest.current_int());
	BOOST_CHECK(c == 1);

	boost::this_thread::sleep_for(boost::chrono::seconds(1));
	BOOST_CHECK_NO_THROW(c = untest.current_int());
	BOOST_CHECK(c == 2);

	boost::this_thread::sleep_for(boost::chrono::seconds(1));
	BOOST_CHECK_NO_THROW(c = untest.current_int());
	BOOST_CHECK(c == 3);

	boost::this_thread::sleep_for(boost::chrono::seconds(4));
}
