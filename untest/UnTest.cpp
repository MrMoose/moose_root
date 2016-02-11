//  Copyright 2015 Stephan Menzel. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "UnTest.hpp"

#include <boost/cstdint.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/ratio.hpp>

namespace moose {
namespace untest {

typedef boost::chrono::high_resolution_clock clck;
typedef boost::chrono::duration<float> seconds;

struct UnTestMembers : public moose::tools::Pimplee {

	UnTestMembers()
			: m_counterthread(nullptr)
			, m_current_int(0)
			, m_current_float(0.0) {
	}

	~UnTestMembers() throw () {
	
	}

	mutable boost::mutex                           m_mutex;
	boost::thread                                 *m_counterthread;
	seconds                                        m_delay;
	std::uint8_t                                   m_current_int;
	float                                          m_current_float;
};


UnTest::UnTest() {

}

UnTest::~UnTest() {

	if (d().m_counterthread) {
		d().m_counterthread->interrupt();
		d().m_counterthread->join();
		delete d().m_counterthread;
		d().m_counterthread = nullptr;
	}
}

void UnTest::start(const unsigned int n_millis /* = 1000 */) {
	
	boost::mutex::scoped_lock slock(d().m_mutex);
	if (d().m_counterthread) {
		return;
	} else {
		d().m_delay = boost::chrono::milliseconds(n_millis);
		d().m_counterthread = new boost::thread([&]() {

			// total length of increasing interval
			seconds interval(3);
			seconds slice(d().m_delay / interval);

			while (true) { // should have used Linux. It does the endless loop in less than 4 seconds.
				{
					boost::mutex::scoped_lock slock(d().m_mutex);
					d().m_current_int++;
					d().m_current_float += slice.count();
					if (d().m_current_float > 1.0) {
						d().m_current_float = 0.0f;
					}
				}
				boost::this_thread::sleep_for(d().m_delay);
			}
		});
	}
}

std::uint8_t UnTest::current_int() const {

	boost::mutex::scoped_lock slock(d().m_mutex);
	return d().m_current_int;
}

float UnTest::current_float() const {

	boost::mutex::scoped_lock slock(d().m_mutex);
	return d().m_current_float;
}

}
}

