#include "Server.hpp"

#include <boost/asio.hpp>

int main(int argc, char **argv) {

	try {

		boost::asio::io_service iosrv;
		moose::licsrv::Server server(iosrv);
		iosrv.run();

	} catch (const std::exception &sex) {
		std::cerr << "Exception: " << sex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

