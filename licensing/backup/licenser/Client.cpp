// STL
#include <vector>
#include <string>

// booost
#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string/classification.hpp>

// own stuff
#include "Client.hpp"
#include "Licenses.hpp"
#include "Server.hpp"
#include "Paddy.hpp"

namespace Licenser {

using asio::ip::tcp;
using namespace std;

Client::pointer Client::create(asio::io_service& iosrv, Server &parent) {

	return pointer(new Client(iosrv, parent));
}

tcp::socket& Client::socket() {

	return _socket;
}

void Client::start() {

	asio::error err;
	size_t bytes_transferred = 0;

	processCommand(err, bytes_transferred);
}

Client::Client(asio::io_service& iosrv, Server &parent) :
		_buf(1024),
		_parent(parent),
		_socket(iosrv) {

}

void Client::processCommand(const asio::error& error, size_t bytes_transferred) {

	asio::async_read_until(_socket, _buf, '\n',
			boost::bind(&Client::handle_read_command, shared_from_this(),
					asio::placeholders::error,
					asio::placeholders::bytes_transferred));

}

void Client::handle_write_challenge(const asio::error& error, size_t bytes_transferred) {

	if (_state == OK) {
		// read response
		asio::async_read_until(_socket, _buf, '\n',
				boost::bind(&Client::handle_read_response, shared_from_this(),
						asio::placeholders::error,
						asio::placeholders::bytes_transferred));
	} else {
		// close socket
		_socket.close();
	}

}

void Client::handle_write_answer(const asio::error& error, size_t bytes_transferred) {

	// nothing
	_socket.close();
}

void Client::handle_read_command(const asio::error& error, size_t bytes_transferred) {

	vector<string> tokens;
	string         challenge;
	string         reply;

	if (!error) {
		// read one command line such as "getlicense <ID>"
		istream is(&_buf);
		string line;
		getline(is, line);

		boost::algorithm::split(tokens, line, boost::algorithm::is_space());

		if (tokens.size() > 1) {
			if (tokens[0] == "getlicense") {		// TODO: error or broken protocol handling
				_id = tokens[1];
				boost::algorithm::trim(_id);
			}
		}

		cout << "Client requested license \"" << _id << "\" ";
		
		// is this license still valid?
		Licenses::iterator lit = _parent.getLicenses().find(_id);
		if (lit != _parent.getLicenses().end()) {
			if (lit->second > boost::gregorian::day_clock::local_day()) {
				// OK, let's callenge!
				// First we gotta construct a 64 bit random number out of
				// two 32 bit ones. I know, it's weird but easy enough for me
				cout << "which is valid" << endl;
				_state = OK;
				int64_t tmp;
				int32_t *ptr = (int32_t *) &tmp;
				*ptr++ = rand();
				*ptr = rand();
				_challenge_rand = tmp;

				Paddy::assignf(challenge, "%lli\n", _challenge_rand);
			} else {
				// booohooo, the license has expired
				cout << "which has expired on " << lit->second << endl;
				_state = LICENSE_EXPIRED;
				challenge = "ERR -1\n";
			}
		
		} else {
			// apparently not. Chuck Norris will come and get ya!
			cout << "which is unknown" << endl;
			_state = LICENSE_INVALID;
			challenge = "ERR -2\n";
		}

		// write challenge
		asio::async_write(_socket, asio::buffer(challenge),
			boost::bind(&Client::handle_write_challenge, shared_from_this(),
					asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	}
}

void Client::handle_read_response(const asio::error& error, size_t bytes_transferred) {

	vector<string> tokens;
	string         answer;
	string         response;
	string         reply;

	if (!error) {
		// read base64 encoded response
		istream is(&_buf);
		string line;
		getline(is, line);

		try {
			// decode Base64
			Coder::decodeBF(line, response);

			cout << "read and decoded response: " << response << endl;

			// split at colons
			boost::algorithm::split(tokens, response, boost::algorithm::is_any_of(":"));
			if (tokens.size() == 3) {
				int64_t arand = atoll(tokens[0].c_str());
				string aid = tokens[1];
				if ((aid == _id) && (arand == _challenge_rand)) {
					// License OK
					Paddy::assignf(answer, "%lli:-)", atoll(tokens[2].c_str()));
				} else {
					// Not OK. I'm sending Chuck out...
					Paddy::assignf(answer, "%lli:-)", atoll(tokens[2].c_str()));
				}
			} else {
				// protocol error or hack attempt
				answer = "ERR -23";
			}
		} catch (const exception &ex) {
			answer = "ERR -23";
		}

		// encode Base64
		try {
			Coder::encodeBF(answer, reply);

		} catch (const exception &ex) {
			reply = "ERR -100";
		}

		reply.append("\n");

		// write answer. We're done
		asio::async_write(_socket, asio::buffer(reply),
				boost::bind(&Client::handle_write_answer, shared_from_this(),
					asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	}
}

}

