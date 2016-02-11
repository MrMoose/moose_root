#include "Server.hpp"


// MOEP remove includes
extern "C" {
#include <stdlib.h>
#include <sys/types.h>
#include <linux/unistd.h>
}

namespace moose {
namespace licsrv {

namespace asio = boost::asio;
using boost::asio::ip::tcp;

Server::Server(boost::asio::io_service &n_iosrv) :
		m_acceptor(n_iosrv, tcp::endpoint(tcp::v4(), 42023)),
		m_licenses("licenses.conf"),
		m_timer(n_iosrv) {


	m_timer.expires_from_now(boost::posix_time::seconds(1));
	m_timer.async_wait(boost::bind(&Server::handle_timer, this, boost::asio::placeholders::error));

	m_acceptor.set_option(tcp::acceptor::reuse_address(true));
	m_acceptor.listen(128);

	start_accept();
}


void Server::handle_timer(const boost::system::error_code &n_err) {

	if (!n_err) {
		if (m_licenses.gottaUpdate()) {
			m_licenses.init();
		}
		m_timer.expires_from_now(boost::posix_time::seconds(2));
		m_timer.async_wait(boost::bind(&Server::handle_timer, this, boost::asio::placeholders::error));
	} else {
		std::cerr << "Timer error!" << n_err << std::endl;
	}
}

void Server::start_accept() {

	Client::pointer new_connection = Client::create(m_io_service, *this);


	m_acceptor.async_accept(new_connection->socket(),
			boost::bind(&Server::handle_accept, this, new_connection,
				asio::placeholders::error));
}

void Server::handle_accept(Client::pointer new_client,
				const boost::system::error_code &n_err) {

	if (!n_err) {
		new_client->start();
		start_accept();
	}
}

Licenses &Server::getLicenses() noexcept {

	return m_licenses;
}

} // ns licsrv
} // ns moose

