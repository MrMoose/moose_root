#ifndef DEFINE_LICENSER_LICENSES_HH
#define DEFINE_LICENSER_LICENSES_HH

// STL
#include <stdexcept>
#include <map>
#include <string>

// booost
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/exception.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"

// own
#include "FileData.hpp"

namespace Licenser {

/*
 * All configured licenses are in here. Dupped by a file
 */	
class Licenses : public std::map<std::string, boost::gregorian::date>, public FileData {

public:
	typedef std::map<std::string, boost::gregorian::date>::iterator iterator;
        typedef std::map<std::string, boost::gregorian::date>::const_iterator const_iterator;
	typedef std::map<std::string, boost::gregorian::date>::reverse_iterator reverse_iterator;
	typedef std::map<std::string, boost::gregorian::date>::const_reverse_iterator const_reverse_iterator;
	typedef std::map<std::string, boost::gregorian::date>::size_type size_type;
								    

	     Licenses(const char *filename) throw (std::bad_alloc);

	// read config file
	void init() throw (boost::filesystem::filesystem_error, std::bad_alloc);

};

}

#endif

