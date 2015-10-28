#include "Lock.h"
#include "internal/Common_internal.h"

namespace ccc {

namespace internal_ {

	struct MutexData {
#ifdef _WIN32
		CRITICAL_SECTION cs;
#endif
	};
}

Mutex::Mutex() {
	pd_ = new internal_::MutexData;
#ifdef _WIN32
	InitializeCriticalSection(&(pd_->cs));
#endif
}

Mutex::~Mutex() {
#ifdef _WIN32
	DeleteCriticalSection(&(pd_->cs));
#endif
	delete pd_;
}

void Mutex::enter() {
#ifdef _WIN32
	EnterCriticalSection(&(pd_->cs));
#endif
}

void Mutex::leave() {
#ifdef _WIN32
	LeaveCriticalSection(&(pd_->cs));
#endif
}

}