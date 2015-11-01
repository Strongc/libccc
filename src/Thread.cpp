#include "Thread.h"
#include "internal/Common_internal.h"

namespace ccc {

namespace internal_ {

	struct ThreadBaseData {
		typedef ccc::thread_ret_t (CCCAPI *thread_proc_type)(void*);

		ThreadBaseData() : running(false) {
#ifdef _WIN32
			handle = INVALID_HANDLE_VALUE;
#endif
		}
		
		virtual ~ThreadBaseData() {
			clear();
		}
		
		void run(thread_proc_type func, void* param) {
			if (running) return;
#ifdef _WIN32
			unsigned id;
			handle = (thread_t)_beginthreadex(NULL, 0, func, param, 0, &id);
			running = (handle != INVALID_HANDLE_VALUE);
#else
			int ret = pthread_create(&handle, NULL, func, param);
			running = (ret == 0);
#endif
		}
		
		void join() {
			if (!running) return;
#ifdef _WIN32
			WaitForSingleObject(handle, INFINITE);
#else
			void* pRet;
			pthread_join(&handle, &pRet);
#endif
			clear();
		}
		
		void clear() {
#ifdef _WIN32
			if (handle != INVALID_HANDLE_VALUE) {
				CloseHandle(handle);
				handle = INVALID_HANDLE_VALUE;
			}
#else
			pthread_detach(&handle);
#endif
			running = false;
		}
		
		thread_t handle;
		bool running;
	};
	
	struct ThreadData : public ThreadBaseData {
		explicit ThreadData(ccc::Proc0 f) {
			func = f;
		}

		Proc0 func;
	};
}

thread_ret_t CCCAPI ThreadBase::threadProc(void* p) {
	ThreadBase* that = reinterpret_cast<ThreadBase*>(p);

	if (!that) return 0;
	that->proc();

	return 0;
}

ThreadBase::ThreadBase() {
	pd_ = new internal_::ThreadBaseData;
}

ThreadBase::ThreadBase(internal_::ThreadBaseData* pd) {
	CCC_ASSERT(pd);
	pd_ = pd;
}

ThreadBase::~ThreadBase() {
	pd_->clear();
	delete pd_;
}

void ThreadBase::run() {
	pd_->run(&ThreadBase::threadProc, this);
}

void ThreadBase::join() {
	pd_->join();
}

Thread::Thread(Proc0 f)
	: ThreadBase(new internal_::ThreadData(f)) {}

Thread::Thread(c_func_types f)
	: ThreadBase(new internal_::ThreadData(bind0(f))) {}

void Thread::proc() {
	try {
		internal_::ThreadData* p = dynamic_cast<internal_::ThreadData*>(pd_);
	
		if (p) {
			p->func();
		}
	} catch (...) {
		CCC_ASSERT(0);
		return;
	}
}

}