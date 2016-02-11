#ifndef DEFINE_LICENSE_SERVER
#define DEFINE_LICENSE_SERVER

// STL
#include <string>

// booost
#include <asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

// own
#include "Licenses.hpp"
#include "Client.hpp"

using asio::ip::tcp;

namespace Licenser {

//class Client;

class Server : public boost::noncopyable {

	public:
		                Server(asio::io_service& iosrv);
		Licenses &getLicenses() throw () { return _licenses; };

	private:
		void            start_accept();
		void            handle_accept(Client::pointer new_client, const asio::error& error);
		void            handle_timer(const asio::error& error);

		tcp::acceptor        _acceptor;
		Licenses             _licenses;
		asio::deadline_timer _timer;
};

}

#endif

