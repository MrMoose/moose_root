#pragma once

// own
#include "LicSrvConfig.hpp"
#include "Licenses.hpp"
#include "Client.hpp"

// booost
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

// STL
#include <string>


namespace moose {
namespace licsrv {

//class Client;

class Server : public boost::noncopyable {

	public:
		MOOSE_LICSRV_API Server(boost::asio::io_service &n_iosrv);
		MOOSE_LICSRV_API Licenses &getLicenses() noexcept;

	private:
		void start_accept();
		void handle_accept(Client::pointer n_new_client, const boost::system::error_code &n_error);
		void handle_timer(const boost::system::error_code &n_error);

		boost::asio::ip::tcp::acceptor   m_acceptor;
		boost::asio::io_service          m_io_service;
		Licenses                         m_licenses;
		boost::asio::deadline_timer      m_timer;
};

} // ns licsrv
} // ns moose

