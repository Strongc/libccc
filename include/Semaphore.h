#ifndef __CCC_SEMAPHORE_H__
#define __CCC_SEMAPHORE_H__

#include "Common.h"

namespace ccc {

	namespace internal_ {
		struct SemaphoreData;
	}

	class Semaphore {
		CCC_NONCOPYLABLE(Semaphore)

	public:
		Semaphore();
		~Semaphore();

		void wait();
		void post();

	private:
		internal_::SemaphoreData* pd_;
	};
}

#endif // __CCC_SEMAPHORE_H__