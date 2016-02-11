#pragma once

// own stuff
#include "LicSrvConfig.hpp"
#include "License.hpp"

// booost
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/cstdint.hpp>

// STL
#include <ctime>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

/*
 * This class represents one client connection to the licenser server.
 * It uses a challenge response method of authentication
 */
namespace moose {
namespace licsrv {

class Server;

class Client : public boost::enable_shared_from_this<Client> {

	public:
		/*
		 * A license can have several conditions, depending mostly on
		 * the timestamp but also if it is known at all
		 */
		 enum MOOSE_LICSRV_API State {
			OK = 0,
			LICENSE_EXPIRED,
			LICENSE_INVALID
		};

		typedef boost::shared_ptr<Client> pointer;

		// a new connection cometh in
		MOOSE_LICSRV_API static pointer create(boost::asio::io_service &n_iosrv, Server &n_parent);
		MOOSE_LICSRV_API tcp::socket&   socket();

		MOOSE_LICSRV_API void           start();

	private:
		Client(boost::asio::io_service &n_iosrv, Server &n_parent);

		// due to asio we must split the protocol handling in
		// a number of callback functions.
		// The first one is called right after the connection starts
		// to read the clients command, usually "getlicense"
		void processCommand(const boost::system::error_code &n_error, std::size_t n_bytes_transferred);

		// after reading this, parse
		void handle_read_command(const boost::system::error_code &n_error, std::size_t n_bytes_transferred);

		// after we read and parsed the command (getlicense)
		// we gotta assemble and send the challenge
		void handle_write_challenge(const boost::system::error_code &n_error, std::size_t n_bytes_transferred);

		// after we've sent the challenge to the client we read it's answer
		void handle_read_response(const boost::system::error_code &n_error, std::size_t n_bytes_transferred);

		// and send information about whether we allow or refuse the license
		void handle_write_answer(const boost::system::error_code &n_error, std::size_t n_bytes_transferred);


		boost::asio::streambuf          m_buf;             //!< the reading buffer
		boost::uint64_t                 m_challenge_rand;  //!< a random number, the challenge
		License                         m_license;         //!< the license requested
		Server                         &m_parent;          //!< the parent server instance
		boost::asio::ip::tcp::socket    m_socket;          //!< connection socket
		State                           m_state;           //!< the state in which the requested
		                                                   //!< license is
};

} // ns licsrv
} // ns moose

