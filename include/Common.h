#ifndef __CCC_COMMON_H__
#define __CCC_COMMON_H__

#include <assert.h>
#include <exception>

namespace ccc {

#ifdef _WIN32
	typedef unsigned thread_ret_t;
#else
	typedef void* thread_ret_t;
#endif
}

#ifndef CCCAPI
#ifdef _WIN32
#define CCCAPI __stdcall
#else
#define CCCAPI
#endif // _WIN32
#endif // CCCAPI

#ifndef CCC_NONCOPYLABLE
#define CCC_NONCOPYLABLE(type) \
	private:\
		type(const type&) {}\
		void operator = (const type&) {}
#endif // CCC_NONCOPYLABLE

#endif // __CCC_COMMON_H__