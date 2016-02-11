
//  Copyright 2015 Stephan Menzel. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "UnTestConfig.hpp"

#include "tools/Pimpled.hpp"

namespace moose {
namespace untest {

struct UnTestMembers;

//! all right, let's start slow.
//! an instance of this class will start counting upon start() and then 
//! current() you the current value, increasing by 1 each second, overflowing at 255
class UnTest : private tools::Pimpled<UnTestMembers> {

	public:
		MOOSE_UNTEST_API UnTest();
		MOOSE_UNTEST_API virtual ~UnTest();

		//! specify the amount of milliseconds to sleep before increasing current value
		MOOSE_UNTEST_API void start(const unsigned int n_millis = 1000);

		// going from 0 to 255 steady 
		MOOSE_UNTEST_API std::uint8_t current_int() const;

		// going from 0 to 1.0 once every 3 seconds
		MOOSE_UNTEST_API float current_float() const;
};



}
}

