#include "ut.h"
#include "ccc.h"
#include <stdio.h>

using namespace ccc;
using namespace std;

class MessageLoopDemo {
public:
	void start() {
		loop.registerDispatcher(this, bind1(this, &MessageLoopDemo::processMsg));
		loopThd.runFunc(bind0(&loop, &MessageLoop::exec));
		postThd.runFunc(bind0(this, &MessageLoopDemo::post));
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

int main() {
	MessageLoopDemo demo;

	demo.start();
	msleep(20 * 1000);
	demo.stop();

	getchar();
	return 0;
}