// own
#include "License.hpp"

// booost
#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string/classification.hpp>

// STL
#include <vector>
#include <stdexcept>

namespace moose {
namespace licsrv {

License::License() {

}

License::License(const std::string &n_line) {

	std::vector<std::string>           strvec;
	std::vector<std::string>::iterator strit;

	// split on colons
	boost::algorithm::split(strvec, n_line, boost::algorithm::is_any_of(":"));

	if (strvec.size() > 4) {
		strit = strvec.begin();
		m_id = *strit++;

		m_expiry_date = boost::gregorian::from_simple_string(*strit++);

		m_yahoo_id = *strit++;

		m_max_queries = atoi(strit->c_str());
		++strit;

		m_name = *strit;
	} else {
		throw std::bad_alloc();
	}

}

const std::string &License::id(void) noexcept {

	return m_id;
}

const boost::gregorian::date &License::expiry_date(void) noexcept {

	return m_expiry_date;
}

const std::string &License::yahoo_id(void) noexcept {

	return m_yahoo_id;
}

int License::max_queries(void) noexcept {

	return m_max_queries;
}

const std::string &License::name(void) noexcept {

	return m_name;

}

}
}


