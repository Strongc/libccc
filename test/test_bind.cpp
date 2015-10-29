#include "ut.h"
#include "Bind.h"

using namespace std;
using namespace ccc;

struct Foobar0 {
	Foobar0() {
		printf("Foobar0 creating\n");
	}

	~Foobar0() {
		printf("Foobar0 deleting\n");
	}

	void setText() {
		t_ = "Foobar0";
		s_i++;
	}

	string text() {
		return t_;
	}

	string t_;
	static int s_i;
};

int g_i = 0;
int func0() {
	return ++g_i;
}

int Foobar0::s_i = 0;

int main() {
#if 1
	UT_APP(ccc::Bind);
#if 1
	UT(bind0 naked) {
		Foobar0 fb0;

        UT_ASSERT(fb0.text() == "", "before proc");

        Proc0 proc = bind0(&fb0, &Foobar0::setText);
        proc();

        UT_ASSERT(fb0.text() == "Foobar0", "after proc");

		Proc0 procNull = bind0((Foobar0*)0, &Foobar0::setText);
		procNull();
	} UT_END

	UT(bind0 RefPtr) {
		RefPtr<Foobar0> pfb0(new Foobar0), pfb01;

		UT_ASSERT(pfb0->text() == "", "before proc");

		Proc0 proc = bind0(pfb0, &Foobar0::setText);
		proc();

		UT_ASSERT(pfb0->text() == "Foobar0", "after proc");

		pfb0->t_ = "";
		pfb01 = pfb0;
		Proc0 proc1 = bind0(pfb01, &Foobar0::setText);
		pfb0 = 0;
		proc1();

		UT_ASSERT(pfb01->text() == "Foobar0", "after proc");

		string& refStr = pfb01->t_;
		refStr = "";
		pfb01 = 0;
		proc();

		UT_ASSERT(refStr == "Foobar0", "after proc");
		
		refStr = "";
		proc1();

		UT_ASSERT(refStr == "Foobar0", "after proc");

		Proc0 procNull = bind0(RefPtr<Foobar0>(), &Foobar0::setText);
		procNull();
	} UT_END
#endif
	UT(bind0 WeakPtr) {
		RefPtr<Foobar0> rpfb0(new Foobar0);
		WeakPtr<Foobar0> wpfb0(rpfb0);

		UT_ASSERT(wpfb0->text() == "", "before proc");

		Proc0 proc = bind0(wpfb0, &Foobar0::setText);

		proc();

		UT_ASSERT(wpfb0->text() == "Foobar0", "after proc");

		int i = rpfb0->s_i;
		rpfb0 = 0;

		proc();

		UT_ASSERT(i == Foobar0::s_i, "after proc");

		Proc0 procNull = bind0(WeakPtr<Foobar0>(), &Foobar0::setText);
		procNull();
	} UT_END

	UT(bind0 c function) {
		Proc0 proc = bind0(func0);
		proc();

		UT_ASSERT(g_i == 1, "after proc");
	} UT_END

	UT_RUN;
	UT_APP_END;

	UT_RETURN;
#endif
}