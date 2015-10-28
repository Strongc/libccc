#include "ut.h"
#include "Atom.h"
#include <stdio.h>

using namespace ccc;


int main() {
	UT_APP(ccc::Atom);

	UT(initiallize) {
		Atom a, b(8);

		UT_ASSERT(a.get() == 0, "");
		UT_ASSERT(b.get() == 8, "");
	} UT_END

	UT_RUN;
	UT_APP_END;

	UT_RETURN;
}