#ifndef DEFINE_LICENSER_CLIENT
#define DEFINE_LICENSER_CLIENT

// STL
#include <ctime>
#include <iostream>
#include <string>

// booost
#include <asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

// own stuff
#include "Coder.hpp"

using asio::ip::tcp;

namespace Licenser {

class Server;

class Client : public boost::enable_shared_from_this<Client> {

	public:
		enum State {
			OK = 0,
			LICENSE_EXPIRED,
			LICENSE_INVALID
		};

		typedef boost::shared_ptr<Client> pointer;

		static pointer create(asio::io_service& iosrv, Server &parent);
		tcp::socket&   socket();

		void           start();

	private:
		Client(asio::io_service& iosrv, Server &parent);
		void           handle_write_challenge(const asio::error& error, size_t bytes_transferred);
		void           handle_write_answer(const asio::error& error, size_t bytes_transferred);
		void           handle_read_command(const asio::error& error, size_t bytes_transferred);
		void           handle_read_response(const asio::error& error, size_t bytes_transferred);

		void           processCommand(const asio::error& error, size_t bytes_transferred);

		asio::streambuf  _buf;
		int64_t          _challenge_rand;
		std::string      _id;
		Server          &_parent;
		tcp::socket      _socket;
		State            _state;
};

}

#endif

