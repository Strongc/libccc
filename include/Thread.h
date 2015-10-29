#ifndef __CCC_THREAD_H__
#define __CCC_THREAD_H__

#include "Common.h"
#include "Bind.h"

namespace ccc {

	namespace internal_ {

		struct ThreadBaseData;
		struct ThreadData;
	}

	/**
	 * @class 线程基类
	 */
	class ThreadBase {
		CCC_NONCOPYLABLE(ThreadBase)

	public:
		ThreadBase();
		ThreadBase(internal_::ThreadBaseData* pd);
		virtual ~ThreadBase();

		void run();
		void join();

	protected:
		virtual void proc() = 0;

		internal_::ThreadBaseData* pd_;
		
	private:
		static thread_ret_t CCCAPI threadProc(void* p);
	};

	/**
	 * @class 线程类
	 * @brief 接收一个函数对象并在新开启的线程中运行
	 */
	class Thread : public ThreadBase {
	public:
		explicit Thread(Binded0 bnd);
		virtual ~Thread() {}

	protected:
		virtual void proc();
	};
}

#endif // __CCC_THREAD_H__