// STL
#include <iostream>
#include <string>

// booost
#include "boost/algorithm/string/trim.hpp"

// own
#include "FileData.hpp"

namespace Licenser {
	
using namespace std;

FileData::FileData(const char *filename) throw (bad_alloc) : _mtime(0), _size(0) {

	if (filename) {
		string fname(filename);
		boost::algorithm::trim(fname);
		_filename = fname;
	} else {
		// I won't throw up here anymore ;-)
		//throw bad_alloc();
	}
}

FileData::~FileData() {

}

/*
 * returns 1 - file exists and needs update
 *         0 - file exists but doesn't need updating
 *        -1 - file doesn't exist
 */
int FileData::gottaUpdate(void) const throw () {

	if (_filename.empty())
		return 0;

	if (!boost::filesystem::exists(_filename))
		return -1;
	
        try {
		if (boost::filesystem::last_write_time(_filename) != _mtime || boost::filesystem::file_size(_filename) != _size)
			return 1;
		else
			return 0;
        } catch (...) {
		std::cout << "couldn't stat file " << _filename.string() << " in FileDataStruct::needsUpdate" << std::endl;
		return 0;
	}
}

}


