#ifndef DEFINE_LICENSER_CODER_HPP
#define DEFINE_LICENSER_CODER_HPP

// STL
#include <stdexcept>
#include <string>

namespace Licenser {

class Coder {
public:
	static void encodeBF(const std::string &input, std::string &output) throw (std::exception);
	static void decodeBF(const std::string &input, std::string &output) throw (std::exception);
	
};

}

#endif

