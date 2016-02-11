#pragma once

// own
#include "LicSrvConfig.hpp"
#include "FileData.hpp"
#include "License.hpp"

// booost
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/exception.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"

// STL
#include <map>
#include <string>

namespace moose {
namespace licsrv {

/*
 * All configured licenses are in here. Dupped by a file
 */
class Licenses : public FileData {

	public:
		MOOSE_LICSRV_API Licenses(const char *filename);
		MOOSE_LICSRV_API ~Licenses() noexcept;

		//! read config file
		//! throw (boost::filesystem::filesystem_error, std::bad_alloc)
		MOOSE_LICSRV_API void init();

		//! @return copy until all this is made thread safe
		//! @throw if license could not be found
		MOOSE_LICSRV_API License get(const std::string &n_id);
		MOOSE_LICSRV_API const License get(const std::string &n_id) const;

	private:
		typedef std::map<std::string, License> licmaptype;

		licmaptype  m_licenses;

};

}
}

