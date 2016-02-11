//  Copyright 2015 Stephan Menzel. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#define MOOSE_LICSRV_DLL

// see https://gcc.gnu.org/wiki/Visibility
// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
	#define MOOSE_LICSRV_DLL_IMPORT __declspec(dllimport)
	#define MOOSE_LICSRV_DLL_EXPORT __declspec(dllexport)
	#define MOOSE_LICSRV_DLL_LOCAL
#else
	#if __GNUC__ >= 4
		#define MOOSE_LICSRV_DLL_IMPORT __attribute__ ((visibility ("default")))
		#define MOOSE_LICSRV_DLL_EXPORT __attribute__ ((visibility ("default")))
		#define MOOSE_LICSRV_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
	#else
		#define MOOSE_LICSRV_DLL_IMPORT
		#define MOOSE_LICSRV_DLL_EXPORT
		#define MOOSE_LICSRV_DLL_LOCAL
	#endif
#endif

#ifdef MOOSE_LICSRV_DLL // defined if is compiled as a DLL
	#ifdef moose_licsrv_EXPORTS // defined if we are building the DLL (instead of using it)
		#define MOOSE_LICSRV_API MOOSE_LICSRV_DLL_EXPORT
	#else
		#define MOOSE_LICSRV_API MOOSE_LICSRV_DLL_IMPORT
	#endif // moose_liclib_EXPORTS
	#define MOOSE_LICSRV_LOCAL MOOSE_LICLIB_DLL_LOCAL
#else // MOOSE_LICSRV_DLL is not defined: this means it is a static lib.
	#define MOOSE_LICSRV_API
	#define MOOSE_LICSRV_LOCAL
#endif // MOOSE_LICSRV_DLL



