#include "ut.h"
#include "ccc.h"
#include <stdio.h>

using namespace ccc;
using namespace std;

class Foobar {
public:
	static int count;

	Foobar() {
		count++;
	}

	~Foobar() {
		count--;
	}

	void hello() {
		printf("Hello Foobar\n");
	}
};

int Foobar::count = 0;

int main() {
	UT_APP(ccc::RefPtr);

	UT(RefPtr) {
		RefPtr<Foobar> p1(new Foobar);
		RefPtr<Foobar> p2(p1);

		p1 = 0;

		UT_ASSERT(Foobar::count == 1, "");

		p2 = 0;

		UT_ASSERT(Foobar::count == 0, "");
	} UT_END

	UT(WeakPtr) {
		RefPtr<Foobar> p1(new Foobar);
		WeakPtr<Foobar> p2(p1);

		p1 = 0;
		RefPtr<Foobar> p3 = p2.lock();

		UT_ASSERT(Foobar::count == 0, "");
		UT_ASSERT(p3.empty(), "");

		p1 = new Foobar;
		p2 = p1;
		p3 = p2.lock();

		p1 = 0;

		UT_ASSERT(Foobar::count == 1, "");
		UT_ASSERT(!p3.empty(), "");
	} UT_END

	UT_RUN
	UT_APP_END
	UT_RETURN
}