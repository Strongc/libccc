#include "ut.h"
#include "Bind.h"

using namespace std;
using namespace ccc;

int main() {
	UT_APP(ccc::Bind);

	struct Foobar1 {
		void setText(string t) {
			t_ = t;
		}

		string text() {
			return t_;
		}

		string t_;
	};

	UT(bind 0) {
		struct Foobar0 {
			void setText() {
				t_ = "Foobar0";
			}

			string text() {
				return t_;
			}

			string t_;
		};

		Foobar0 fb0;
	} UT_END

	UT_RUN;
	UT_APP_END;

	UT_RETURN;
}