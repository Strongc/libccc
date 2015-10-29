#include "Lock.h"
#include "internal/Common_internal.h"

namespace ccc {

namespace internal_ {

	struct MutexData {
#ifdef _WIN32
		CRITICAL_SECTION cs;
#else
		pthread_mutex_t mtx;
#endif
	};
}

Mutex::Mutex() {
	pd_ = new internal_::MutexData;
#ifdef _WIN32
	InitializeCriticalSection(&(pd_->cs));
#else
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&(pd_->mtx), &attr);
	pthread_mutexattr_destroy(&attr);
#endif
}

Mutex::~Mutex() {
#ifdef _WIN32
	DeleteCriticalSection(&(pd_->cs));
#else
	pthread_mutex_destroy(&(pd_->mtx));
#endif
	delete pd_;
}

void Mutex::enter() {
#ifdef _WIN32
	EnterCriticalSection(&(pd_->cs));
#else
	pthread_mutex_lock(&(pd_->mtx));
#endif
}

void Mutex::leave() {
#ifdef _WIN32
	LeaveCriticalSection(&(pd_->cs));
#else
	pthread_mutex_unlock(&(pd_->mtx));
#endif
}

}