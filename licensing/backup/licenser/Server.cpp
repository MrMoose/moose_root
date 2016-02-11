extern "C" {
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <errno.h>
}

#include "Server.hpp"

using namespace std;

namespace Licenser {

Server::Server(asio::io_service& iosrv) :
		_acceptor(iosrv, tcp::endpoint(tcp::v4(), 42023)),
		_licenses("/etc/licenser/licenses.conf"),
		_timer(iosrv) {
	
	_timer.expires_from_now(boost::posix_time::seconds(1));
	_timer.async_wait(boost::bind(&Server::handle_timer, this, asio::placeholders::error));

	srand(time(0));

	_acceptor.set_option(tcp::acceptor::reuse_address(true));
	_acceptor.listen(128);

	start_accept();
}


void Server::handle_timer(const asio::error& error) {

	if (!error) {
		if (_licenses.gottaUpdate()) {
			_licenses.init();
		}
		_timer.expires_from_now(boost::posix_time::seconds(2));
		_timer.async_wait(boost::bind(&Server::handle_timer, this, asio::placeholders::error));
	} else {
		std::cerr << "Timer error!" << std::endl;
	}
}

void Server::start_accept() {

	Client::pointer new_connection =
		Client::create(_acceptor.io_service(), *this);

	_acceptor.async_accept(new_connection->socket(),
			boost::bind(&Server::handle_accept, this, new_connection,
				asio::placeholders::error));
}

void Server::handle_accept(Client::pointer new_client,
				const asio::error& error) {

	if (!error) {
		new_client->start();
		start_accept();
	}
}

}

