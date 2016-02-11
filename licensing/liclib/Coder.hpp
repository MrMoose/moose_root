#pragma once

#include "LicLibConfig.hpp"

// boost
#include <boost/format.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/asio.hpp>

#define KEYWORD "DetzoldIstDoof"

#include <string>


namespace moose {
namespace liclib {


	// throw std::exception
	// throw asio error
	MOOSE_LICLIB_API bool verifyLicense(const std::string &n_ip, const std::string &n_license, int &n_max_queries, std::string &n_yahoo_id, std::string &n_customer_name, std::string &n_expiry_date);


	MOOSE_LICLIB_API void encodeBF(const std::string &n_input, std::string &n_output);
	MOOSE_LICLIB_API void decodeBF(const std::string &n_input, std::string &n_output);
} // ns liclib
} // ns moose


