// STL
#include <iostream>
#include <vector>
#include <string>

// booost
#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string/classification.hpp>

// own
#include "Licenses.hpp"

namespace Licenser {

using namespace std;

Licenses::Licenses(const char *filename) throw (bad_alloc) : FileData(filename) {

	try {
		init();
	} catch (boost::filesystem::filesystem_error &fex) {
		cerr << "couldn't read license file: " << filename << " : " << fex.what() << endl;
	}
}

// read config file
void Licenses::init() throw (boost::filesystem::filesystem_error, bad_alloc) {

	string                   line;
	vector<string>           strvec;
	vector<string>::iterator strit;

	if (!boost::filesystem::exists(_filename)) {
		clear();
		return;
	}

	_size = boost::filesystem::file_size(_filename);
	_mtime = boost::filesystem::last_write_time(_filename);
	clear();

	boost::filesystem::ifstream infile(_filename, std::ios::in);

	// read the file
	while (getline(infile, line)) {
		// skip comments and stuff we don't know
		if (!(line[0] == '#') && !(line[0] ==':') && !(line[0] =='$') && line.size() > 2) {
		        // split on colons
			boost::algorithm::split(strvec, line, boost::algorithm::is_any_of(":"));

			try {
				if (strvec.size() > 1) {
					strit = strvec.begin();
					string license = *strit++;
					string expiry_date = *strit;
					boost::algorithm::trim(license);
					boost::algorithm::trim(expiry_date);
					insert(make_pair(license, boost::gregorian::from_simple_string(expiry_date)));
					cout << "licenser read license " << license << " expiring on " << expiry_date << endl;
				}
			} catch (const exception &ex) {
				cerr << "couldn't parse license line: " << line << " : " << ex.what() << endl;
			}
		}
	}

}         

}


