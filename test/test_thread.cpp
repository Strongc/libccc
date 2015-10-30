#include "ut.h"
#include "ccc.h"
#include <stdio.h>

using namespace ccc;
using namespace std;

int main() {
	UT_APP(ccc::Thread);
	
	UT(ThreadBase Subclass) {
		class MyThread : public ThreadBase {
		protected:
			virtual void proc() {
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

	UT_RUN;
	UT_APP_END;

	UT_RETURN;
}