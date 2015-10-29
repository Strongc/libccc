#include "ut.h"
#include "Atom.h"
#include <stdio.h>

using namespace ccc;


int main() {
	UT_APP(ccc::Atom);

	UT(initiallize) {
		Atom a, b(8), c(0), d(-8);
	} UT_END

	UT(get) {
		Atom a, b(8), c(0), d(-8);

		UT_ASSERT(a.get() == 0, "");
		UT_ASSERT(b.get() == 8, "");
		UT_ASSERT(c.get() == 0, "");
		UT_ASSERT(d.get() == -8, "");
	} UT_END
	
	UT(set) {
		Atom a(0);

		UT_ASSERT(a.set(1) == 1, "");
		UT_ASSERT(a.set(-1) == -1, "");
		UT_ASSERT(a.set(0) == 0, "");
		UT_ASSERT(a.set(0xffffffff) == 0xffffffff, "");
	} UT_END

	UT(inc & dec) {
		Atom a(0);

		UT_ASSERT(a.inc() == 1, "");
		UT_ASSERT(a.dec() == 0, "");
	} UT_END

	UT_RUN;
	UT_APP_END;
	UT_RETURN;
}