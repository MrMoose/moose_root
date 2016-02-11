// own
#include "FileData.hpp"

// booost
#include "boost/algorithm/string/trim.hpp"

// STL
#include <iostream>
#include <string>

namespace moose {
namespace licsrv {

FileData::FileData(const char *filename)
		: m_mtime(0)
		, m_size(0) {

	if (filename) {
		std::string fname(filename);
		boost::algorithm::trim(fname);
		m_filename = fname;
	} else {
		// I won't throw up here anymore ;-)
		//throw bad_alloc();
	}
}

FileData::~FileData() noexcept {

}

/*
 * returns 1 - file exists and needs update
 *         0 - file exists but doesn't need updating
 *        -1 - file doesn't exist
 */
int FileData::gottaUpdate(void) const noexcept {

	if (m_filename.empty())
		return 0;

	if (!boost::filesystem::exists(m_filename))
		return -1;

        try {
		if (boost::filesystem::last_write_time(m_filename) != m_mtime || boost::filesystem::file_size(m_filename) != m_size)
			return 1;
		else
			return 0;
        } catch (...) {
		std::cout << "couldn't stat file " << m_filename.string() << " in FileDataStruct::needsUpdate" << std::endl;
		return 0;
	}
}

} // ns licsrv
} // ns moose



