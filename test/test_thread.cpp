#include "ut.h"
#include "ccc.h"
#include <stdio.h>

using namespace ccc;
using namespace std;

int flag1 = 0;

static void threadProc() {
	msleep(1000);
	flag1 = 9;
	msleep(1000);
}

static void threadProcExpc() {
	msleep(1000);
	throw 0;
	msleep(1000);
}

int main() {
	UT_APP(ccc::Thread);
	
	UT(ThreadBase subclass) {
		class MyThread : public ThreadBase {
		protected:
			virtual void exec() {
				printf("thread start\n");
				msleep(1000);
				str = "dirty";
				printf("thread end\n");
			}

		public:
			string str;
		};
		
		MyThread thd;
		
		thd.run();
		thd.join();
		
		UT_ASSERT(thd.str == "dirty", "");
	} UT_END
	
	UT(join before run) {
		class MyThread : public ThreadBase {
		protected:
			virtual void exec() {}
		};
		
		MyThread thd;

		thd.join();
	} UT_END
	
	UT(run after join) {
		class MyThread : public ThreadBase {
		protected:
			virtual void exec() {
				i = 9;
			}
		public:
			MyThread() : i(0) {}
			int i;
		};
		
		MyThread thd;

		thd.join();
		thd.run();
		msleep(1000);
		thd.join();
		
		UT_ASSERT(thd.i == 9, "");
	} UT_END
	
	UT(exception in Thread) {	
		Thread thd(threadProcExpc);

		thd.run();
		msleep(1000);
		thd.join();
	} UT_END
	
	UT(c function in Thread) {
		Thread thd(threadProc);
		thd.run();
		thd.join();
		
		UT_ASSERT(flag1 == 9, "");
	} UT_END
	
	UT(c++ functor in Thread) {
		class MyClass {
		public:
			MyClass() : i(0) {}
			void threadProc() {
				msleep(1000);
				i = 9;
				msleep(1000);
			}
			int i;
		};
		
		MyClass my;
		
		Thread thd(bind0(&my, &MyClass::threadProc));
		thd.run();
		thd.join();

		UT_ASSERT(my.i == 9, "");
	} UT_END

	UT(null in Thread) {
		Thread thd(0);
		thd.run();
		thd.join();
	} UT_END

	UT_RUN;
	UT_APP_END;

	UT_RETURN;
}