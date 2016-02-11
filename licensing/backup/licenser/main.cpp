#include "Server.hpp"
#include <asio.hpp>

using asio::ip::tcp;
using namespace std;

int main() {

	try {
		asio::io_service iosrv;
		Licenser::Server server(iosrv);
		iosrv.run();
	} catch (const std::exception& e) {
		cerr << "Exception: " << e.what() << endl;
	}

	return 0;
}

