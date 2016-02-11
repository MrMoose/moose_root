
// own
#include "Licenses.hpp"

// booost
#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string/classification.hpp>

// STL
#include <iostream>
#include <vector>
#include <string>

namespace moose {
namespace licsrv {

Licenses::Licenses(const char *filename)
		: FileData(filename) {

	try {
		init();

	} catch (boost::filesystem::filesystem_error &fex) {
		std::cerr << "couldn't read license file: " << filename << " : " << fex.what() << std::endl;
	}
}

Licenses::~Licenses() noexcept {

}

// read config file
// throw (boost::filesystem::filesystem_error, bad_alloc)
void Licenses::init() {

	std::string                        line;
	std::vector<std::string>           strvec;
	std::vector<std::string>::iterator strit;

	if (!boost::filesystem::exists(m_filename)) {
		m_licenses.clear();
		return;
	}

	m_size = boost::filesystem::file_size(m_filename);
	m_mtime = boost::filesystem::last_write_time(m_filename);
	m_licenses.clear();

	boost::filesystem::ifstream infile(m_filename, std::ios::in);

	// read the file
	while (std::getline(infile, line)) {
		// skip comments and stuff we don't know
		if (!(line[0] == '#') && !(line[0] ==':') && !(line[0] =='$') && line.size() > 2) {
			try {
				// create a License Info object out of this line and insert
				License lic(line);

				m_licenses.insert(std::make_pair(lic.id(), lic));

			} catch (const std::exception &ex) {
				std::cerr << "couldn't parse license line: " << line << " : " << ex.what() << std::endl;
			}
		}
	}
}

License Licenses::get(const std::string &n_id) {

	licmaptype::iterator i = m_licenses.find(n_id);
	if (i == m_licenses.end()) {
		throw std::runtime_error("unknown license " + n_id);
	} else {
		return i->second;
	}
}


const License Licenses::get(const std::string &n_id) const {

	licmaptype::const_iterator i = m_licenses.find(n_id);
	if (i == m_licenses.end()) {
		throw std::runtime_error("unknown license " + n_id);
	} else {
		return i->second;
	}
}



} // ns licsrv
} // ns moose


