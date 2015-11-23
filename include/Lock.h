#ifndef __CCC_LOCK_H__
#define __CCC_LOCK_H__

#include "Common.h"

namespace ccc {

	namespace internal_ {
	
		struct MutexData;
	}

	/**
	 * @class Mutex
	 * @brief »¥³âËø
	 */
	class Mutex {
	public:
		Mutex();
		~Mutex();

		void enter();
		void leave();

	private:
		internal_::MutexData* pd_;
		
		CCC_NONCOPYABLE(Mutex);
		CCC_INCOMPARABLE(Mutex);
	};

	/**
	 * @class Lock
	 * @brief »¥³âËøraii
	 */
	struct Lock {
		explicit Lock(Mutex* pMtx) {
			CCC_ASSERT(pMtx);
			
			if (pMtx) {
				pMtx->enter();
			}

			pMtx_ = pMtx;
		}

		~Lock() {
			if (pMtx_) {
				pMtx_->leave();
			}
		}

		Mutex* pMtx_;
		
		CCC_NONCOPYABLE(Lock)
		CCC_INCOMPARABLE(Lock);
	};
	
	#define CCC_LOCK(mtx) ccc::Lock lock_##__LINE__(&(mtx));
	#define CCC_LOCK_P(pmtx) ccc::Lock lock_p_##__LINE__(pmtx);
}

#endif // __CCC_LOCK_H__