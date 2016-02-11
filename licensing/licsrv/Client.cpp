// own stuff
#include "Client.hpp"
#include "Licenses.hpp"
#include "Server.hpp"

#include "liclib/Coder.hpp"

#include "tools/Random.hpp"

// booost
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/format.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

// STL
#include <vector>
#include <string>

namespace moose {
namespace licsrv {

namespace asio = boost::asio;
using asio::ip::tcp;

Client::pointer Client::create(asio::io_service &n_iosrv, Server &n_parent) {

	return pointer(new Client(n_iosrv, n_parent));
}

tcp::socket& Client::socket() {

	return m_socket;
}

void Client::start() {

	boost::system::error_code err;
	std::size_t bytes_transferred = 0;

	processCommand(err, bytes_transferred);
}

Client::Client(asio::io_service &n_iosrv, Server &n_parent)
		: m_buf(1024)
		, m_challenge_rand(0)
		, m_parent(n_parent)
		, m_socket(n_iosrv)
		, m_state(LICENSE_INVALID) {

}

// called right when the connection starts this reads in the Clients command
void Client::processCommand(const boost::system::error_code &n_error, std::size_t n_bytes_transferred) {

	// MOEP error handling

	asio::async_read_until(m_socket, m_buf, '\n',
			boost::bind(&Client::handle_read_command, shared_from_this(),
					asio::placeholders::error,
					asio::placeholders::bytes_transferred));

}

// the command parsing
void Client::handle_read_command(const boost::system::error_code &n_err, size_t n_bytes_transferred) {

	// MOEP error handling

	std::vector<std::string> tokens;
	std::string challenge;  // this will either be a challenge or an error message
	std::string reply;
	std::string id;

	if (!n_err) {
		// read one command line such as "getlicense <ID>"
		std::istream is(&m_buf);
		std::string line;
		std::getline(is, line);

		boost::algorithm::split(tokens, line, boost::algorithm::is_space());

		if (tokens.size() > 1) {
			if (tokens[0] == "getlicense") { // TODO: error or broken protocol handling
				id = tokens[1];
				boost::algorithm::trim(id);
			}
		}

		std::cout << "Client requested license \"" << id << "\" ";

		// is this license still valid?
		try {
			m_license = m_parent.getLicenses().get(id);
			if (m_license.expiry_date() > boost::gregorian::day_clock::local_day()) {
				// OK, let's callenge!
				std::cout << "which is valid" << std::endl;
				m_state = OK;
				m_challenge_rand = tools::urand();
				boost::format fmt("%lli\n");
				fmt % m_challenge_rand;
				challenge = fmt.str();
			} else {
				// booohooo, the license has expired
				std::cout << "which has expired on " << m_license.expiry_date() << std::endl;
				m_state = LICENSE_EXPIRED;
				challenge = "ERR -1\n";
			}
		} catch (const std::exception &sex) {
			// apparently not. Chuck Norris will come and get ya!
			std::cout << "which is unknown" << std::endl;
			m_state = LICENSE_INVALID;
			challenge = "ERR -2\n";
		}

		// write challenge
		asio::async_write(m_socket, asio::buffer(challenge),
			boost::bind(&Client::handle_write_challenge, shared_from_this(),
					asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	} else {
		// error handling
		std::cerr << "error reading command: " << n_err << std::endl;
	}
}
void Client::handle_write_challenge(const boost::system::error_code &n_err, std::size_t n_bytes_transferred) {

	if (m_state == OK) {
		// read response
		asio::async_read_until(m_socket, m_buf, '\n',
				boost::bind(&Client::handle_read_response, shared_from_this(),
						asio::placeholders::error,
						asio::placeholders::bytes_transferred));
	} else {
		// close socket
		m_socket.close();
	}

}

void Client::handle_write_answer(const boost::system::error_code &n_err, size_t bytes_transferred) {

	// nothing
	m_socket.close();
}


void Client::handle_read_response(const boost::system::error_code &n_err, size_t bytes_transferred) {

	std::vector<std::string> tokens;
	std::string answer;
	std::string response;
	std::string reply;

	if (!n_err) {
		// read base64 encoded response
		std::istream is(&m_buf);
		std::string line;
		std::getline(is, line);

		try {
			// decode Base64
			liclib::decodeBF(line, response);

			// split at colons
			boost::algorithm::split(tokens, response, boost::algorithm::is_any_of(":"));
			if (tokens.size() == 3) {
				int64_t arand = atoll(tokens[0].c_str());
				std::string aid = tokens[1];
				if ((aid == m_license.id()) && (arand == m_challenge_rand)) {
					// License OK
					boost::format fmt("%lli:-):%s:%i:%s:%s");
					fmt % atoll(tokens[2].c_str()) % m_license.yahoo_id() % m_license.max_queries() % m_license.name() % boost::gregorian::to_iso_extended_string(m_license.expiry_date());
					answer = fmt.str();
					std::cout << "I'm gonna grant it to " << m_license.id() << std::endl;
				} else {
					// Not OK. I'm sending Chuck out...
					boost::format fmt("%lli:-(:%s:%i:%s:%s");
					fmt % atoll(tokens[2].c_str()) % m_license.yahoo_id() % m_license.max_queries() % m_license.name() % boost::gregorian::to_iso_extended_string(m_license.expiry_date());
					std::cout << "I'm refusing " << m_license.id() << std::endl;
				}
			} else {
				// protocol error or hack attempt
				answer = "ERR -23";
			}
		} catch (const std::exception &sex) {
			std::cerr << "Caught exception: " << sex.what() << std::endl;
			answer = "ERR -23";
		}

		// encode Base64
		try {
			liclib::encodeBF(answer, reply);

		} catch (const std::exception &sex) {
			std::cerr << "Caught exception: " << sex.what() << std::endl;
			reply = "ERR -100";
		}

		reply.append("\n");

		// write answer. We're done
		asio::async_write(m_socket, asio::buffer(reply),
				boost::bind(&Client::handle_write_answer, shared_from_this(),
					asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	}
}

}
}

