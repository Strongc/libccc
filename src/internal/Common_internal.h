#ifndef __CCC_COMMON_INTERNAL_H__
#define __CCC_COMMON_INTERNAL_H__

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#endif

namespace ccc {

#ifdef _WIN32
	typedef HANDLE handle_t;
	typedef handle_t thread_t;
	typedef unsigned thread_ret_t;
	typedef handle_t semaphore_t;
#else
	typedef void* handle_t;
	typedef pthread_t thread_t;
	typedef void* thread_ret_t;
	typedef sem_t semaphore_t;
#endif

}

#ifndef CCCAPI
#ifdef _WIN32
#define CCCAPI __stdcall
#else
#define CCCAPI
#endif // _WIN32
#endif // CCCAPI

#endif // __CCC_COMMON_INTERNAL_H__