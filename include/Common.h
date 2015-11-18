#ifndef __CCC_COMMON_H__
#define __CCC_COMMON_H__

#include <assert.h>
#include <exception>

namespace ccc {

	typedef struct {} Void;

	static void dummy_printf(...) {}

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

// FIXME
#ifndef CCC_DBG_PRINT
#ifdef ENABLE_DEBUG_PRINT
#define CCC_DBG_PRINT printf
#else
#define CCC_DBG_PRINT ccc::dummy_printf
#endif // ENABLE_DEBUG_PRINT
#endif // CCC_DBG_PRINT

#ifndef CCC_ASSERT
#ifdef ENABLE_ASSERT
#define CCC_ASSERT assert
#else
#define CCC_ASSERT(expr)
#endif // ENABLE_ASSERT
#endif // CCC_ASSERT

#ifndef CCC_NONCOPYLABLE
#define CCC_NONCOPYLABLE(type) \
	private:\
		type(const type&) {}\
		void operator = (const type&) {}
#endif // CCC_NONCOPYLABLE

#ifndef CCC_INCOMPARABLE
#define CCC_INCOMPARABLE(type) \
	private:\
		void operator == (const type&) {}\
		void operator != (const type&) {}
#endif // CCC_INCOMPARABLE

#endif // __CCC_COMMON_H__