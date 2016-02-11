#pragma once

#include "LicSrvConfig.hpp"

// booost
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>

#include <ctime>

namespace moose {
namespace licsrv {

/*
 * This is basically a simple functionality wrapper to provide a class
 * that can instantiate and maintain itself from a file.
 * When the file is changed, the class will know and you may call init()
 * again. This is quite useless by itself but meant to be derived from.
 * This must however be done from the outside. So every now and again
 * check if the class derived from this needs updating by asking
 * needsUpdate() and, if true, reset _mtime and _size yourself in
 * your own init().

 MOEP! This is bullshit. Remove or replace by portable version

 */
class FileData {

	public:
		MOOSE_LICSRV_API FileData(const char *filename);
		MOOSE_LICSRV_API virtual ~FileData() noexcept;

		// if true, the underlying file was changed
		MOOSE_LICSRV_API int gottaUpdate(void) const noexcept;

	protected:
		boost::filesystem::path   m_filename;
		time_t                    m_mtime;
		off_t                     m_size;
} ;

} // ns licsrv
} // ns moose

