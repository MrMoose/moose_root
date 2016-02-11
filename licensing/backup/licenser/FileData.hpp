#ifndef DEFINE_LICENSER_FILEDATA_HH
#define DEFINE_LICENSER_FILEDATA_HH

#include <stdexcept>

// booost
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/exception.hpp>

namespace Licenser {

/*
 * This is basically a simple functionality wrapper to provide a class
 * that can instantiate and maintain itself from a file.
 * When the file is changed, the class will know and you may call init()
 * again. This is quite useless by itself but meant to be drived from.
 * This must however be done from the outside. So every now and again
 * check if the class derived from this needs updating by asking
 * needsUpdate() and, if true, reset _mtime and _size yourself in 
 * your own init().
 */	
class FileData {	
public:
	             FileData(const char *filename) throw (std::bad_alloc);
	virtual     ~FileData();
	
	// if true, the underlying file was changed
	int          gottaUpdate(void) const throw ();

protected:
	boost::filesystem::path   _filename;
	time_t                    _mtime;
	off_t                     _size;
} ;

}

#endif

