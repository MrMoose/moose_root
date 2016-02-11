#define BOOST_TEST_MODULE CoderTests

#include <boost/test/unit_test.hpp>

#include "liclib/Coder.hpp"

#include "licsrv/Server.hpp"

#include <boost/thread.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>

#include <iostream>


struct server_fixture {

	server_fixture()
		: m_server(m_iosrv) {

		m_thread = new boost::thread( [&](){ m_iosrv.run(); });

		boost::this_thread::sleep_for(boost::chrono::seconds(1));
	}

	~server_fixture() {

		m_iosrv.stop();
		m_thread->join();
		delete m_thread;
		boost::this_thread::sleep_for(boost::chrono::seconds(1));
	}

	boost::asio::io_service m_iosrv;
	moose::licsrv::Server   m_server;
	boost::thread          *m_thread;
};



BOOST_FIXTURE_TEST_CASE( test_case1, server_fixture ) {

	std::string customers_name;
	int         max_queries;
	std::string expiry_date;
	std::string yahoo_id;

	try {
		if (moose::liclib::verifyLicense("127.0.0.1", "TestKundenID", max_queries, yahoo_id, customers_name, expiry_date)) {
			std::cout << "yep!\n\n";
		} else {
			std::cout << "nope!\n\n";
		}
		std::cout << "Values are:\n";
		std::cout << "Max queries    : " << max_queries << std::endl;
		std::cout << "Yahoo ID       : " << yahoo_id << std::endl;
		std::cout << "Customers Name : " << customers_name << std::endl;
		std::cout << "Expiry Date    : " << expiry_date << std::endl;

	} catch (const std::exception &sex) {
		std::cout << "exception:   " << sex.what() << std::endl;
	}
}

