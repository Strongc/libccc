#include "ut.h"
#include "ccc.h"
#include <stdio.h>

using namespace ccc;
using namespace std;

int main() {
	UT_APP(ccc::MessageLoop);

	UT(MessageLoopDemo) {
		class MessageLoopDemo {
		public:
			void start() {
				loop.registerDispatcher(this, bind1(this, &MessageLoopDemo::processMsg));
				loopThd.execFunc(bind0(&loop, &MessageLoop::exec));
				postThd.execFunc(bind0(this, &MessageLoopDemo::post));
			}

			void stop() {
				atmStopPost.set(1);
				postThd.join();
				loop.stop();
				loopThd.join();
			}

			void processMsg(Message msg) {
				printf("Message receive, type = %s, data = %d, to = %u, from = %u\n",
					msg.type.c_str(), msg.data.as<int>(), msg.to, msg.from);
			}

			void post() {
				int i = 0;
				while (!atmStopPost.get()) {
					Message msg("DemoMessage", Any(i++));
					loop.postTo(this, this, msg);

					msleep(1000);
				}
			}

			MessageLoop loop;
			Thread loopThd;
			Thread postThd;
			Atom atmStopPost;
		};

		MessageLoopDemo demo;
		demo.start();
		msleep(20 * 1000);
		demo.stop();
	} UT_END

	UT(registerDispatcher) {
		class Client {
		public:
			void start() {
				loop.registerDispatcher(this, bind1(this, &Client::processMsg));
				loopThd.execFunc(bind0(&loop, &MessageLoop::exec));
				postThd.execFunc(bind0(this, &Client::post));
			}

			void stop() {
				postThd.join();
				loop.stop();
				loopThd.join();
			}

			void processMsg(Message msg) {
				printf("Message receive, type = %s, data = %d, to = %u, from = %u\n",
					msg.type.c_str(), msg.data.as<int>(), msg.to, msg.from);

				atm.set(msg.data.as<int>());
			}

			void post() {
				msleep(1000);
				Message msg("SomeMessage", Any(9));
				loop.postTo(this, this, msg);
			}

			MessageLoop loop;
			Thread loopThd;
			Thread postThd;
			Atom atm;
		};

		Client cnt;

		cnt.start();
		msleep(1000 * 2);
		cnt.stop();

		UT_ASSERT(cnt.atm.get() == 9, "");
	}UT_END

	UT(unregisterDispatcher) {
		class Client {
		public:
			void start() {
				loop.registerDispatcher(this, bind1(this, &Client::processMsg));
				loopThd.execFunc(bind0(&loop, &MessageLoop::exec));
				postThd.execFunc(bind0(this, &Client::post));

				loop.unregisterDispatcher(this);
			}

			void stop() {
				postThd.join();
				loop.stop();
				loopThd.join();
			}

			void processMsg(Message msg) {
				printf("Message receive, type = %s, data = %d, to = %u, from = %u\n",
					msg.type.c_str(), msg.data.as<int>(), msg.to, msg.from);

				atm.set(msg.data.as<int>());
			}

			void post() {
				msleep(1000);
				Message msg("SomeMessage", Any(9));
				loop.postTo(this, this, msg);
			}

			MessageLoop loop;
			Thread loopThd;
			Thread postThd;
			Atom atm;
		};

		Client cnt;

		cnt.start();
		msleep(1000 * 2);
		cnt.stop();

		UT_ASSERT(cnt.atm.get() == 9, "");
	}UT_END

	UT_RUN
	UT_APP_END
	UT_RETURN
}