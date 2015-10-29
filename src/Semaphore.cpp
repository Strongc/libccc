#include "../include/Semaphore.h"
#include "internal/Common_internal.h"
#include <limits>

namespace ccc {

namespace internal_ {

	struct SemaphoreData {
		ccc::semaphore_t sem;
	};
}

Semaphore::Semaphore() {
	pd_ = new internal_::SemaphoreData;
#ifdef _WIN32
	pd_->sem = CreateSemaphore(0, 0, 0xffffffff, 0); // FIXME: use numeric_limits
#else
	sem_init(&(pd_->sem), 0, 0);
#endif
}

Semaphore::~Semaphore() {
#ifdef _WIN32
	CloseHandle(pd_->sem);
#else
	sem_init(&(pd_->sem));
#endif
	delete pd_;
}

void Semaphore::wait() {
#ifdef _WIN32
	WaitForSingleObject(pd_->sem, INFINITE);
#else
	sem_wait(&(pd_->sem));
#endif
}

void Semaphore::post() {
#ifdef _WIN32
	ReleaseSemaphore(pd_->sem, 1, 0);
#else
	sem_post(&(pd_->sem));
#endif
}

}