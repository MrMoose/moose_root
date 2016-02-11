#pragma once

#include "LicSrvConfig.hpp"

// booost
#include <boost/date_time/gregorian/gregorian.hpp>

// STL
#include <string>

namespace moose {
namespace licsrv {

/*!
 * This is one license
 */
class License {

	public:
		// the empty defaul c'tor is not really usable but we need
		// it to have a License object in the Client to be initialized later
		License(void);

		// this creates one of those License objects out of a line in the cfg file
		License(const std::string &n_line);

		const std::string &id(void) noexcept;
		const boost::gregorian::date &expiry_date(void) noexcept;
		const std::string &yahoo_id(void) noexcept;
		int max_queries(void) noexcept;
		const std::string &name(void) noexcept;

	private:
		std::string              m_id;
		boost::gregorian::date   m_expiry_date;
		std::string              m_yahoo_id;
		int                      m_max_queries;
		std::string              m_name;
};

} // ns licsrv
} // ns moose


