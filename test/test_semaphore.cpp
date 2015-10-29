#include "ut.h"
#include "Any.h"
#include <stdio.h>

using namespace ccc;

class Foobar {
public:
	std::string text() {
		return "This is Foobar";
	}
};

class Foobar2 {
public:
	std::string text() {
		return "This is Foobar2";
	}
};

int main() {
	UT_APP(ccc::Any);

	UT(initialize) {
		{Any anyObj;}
		{Any anyObj(0);}
		{Any anyObj((char*)"some text");}
		{Any anyObj(1.234);}
		{Any anyObj((wchar_t*)L"some text");}
		{
			Any anyObj(new Foobar);
			delete anyObj.as<Foobar*>();
		}
	} UT_END

	UT(delete) {
		Any* ptrObj = new Any((char*)"some text");
		delete ptrObj;
	} UT_END

	UT(asPtr) {
		Foobar* pfb1 = new Foobar;
		Any anyObj(pfb1);
		Foobar* pfb2 = *anyObj.asPtr<Foobar*>();

		UT_ASSERT(pfb1 == pfb2, "pfb1 != pfb2");
		UT_ASSERT(pfb2->text() == std::string("This is Foobar"), "*ppfb2->text() != This is Foobar");

		Foobar2** pBuf = anyObj.asPtr<Foobar2*>();

		UT_ASSERT(pBuf == 0, "asPtr() didn't return NULL");

		delete pfb1;
	} UT_END

	UT(as) {
		Foobar* pfb1 = new Foobar;
		Any anyObj(pfb1);
		Foobar* pfb2 = anyObj.as<Foobar*>();

		UT_ASSERT(pfb1 == pfb2, "pfb1 != pfb2");
		UT_ASSERT(pfb2->text() == std::string("This is Foobar"), "*ppfb2->text() != This is Foobar");

		try {
			Foobar2* p = anyObj.as<Foobar2*>();
		} catch (AnyException& e) {
			printf("%s\n", e.what());
		}

		Any anyObj2(0L);
		long& ref = anyObj2.as<long>();

		ref = 999;

		long& ref2 = anyObj2.as<long>();

		UT_ASSERT(ref2 == 999, "as() did'nt return ref correctly");

		delete pfb1;
	} UT_END


	UT(operator =) {
		struct Some {
			int n_;
			Some(int n) {
				n_ = n;
			}
		};

		Any any, any1(0), any2(1.2f), any3(std::string("foo")), any4(Some(7));

		any = any1;
		any1 = any2;
		any2 = any3;
		any3 = any4;

		UT_ASSERT(any.as<int>() == 0, "as<int>");
		UT_ASSERT(!(any1.as<float>() > 1.2f) && !(any1.as<float>() < 1.2f), "as<float>");
		UT_ASSERT(any2.as<std::string>() == std::string("foo"), "as<string>");
		UT_ASSERT(any3.as<Some>().n_ == 7, "as<Some>");
	} UT_END

	UT(operator ==) {
		Any a(1), b(1);
		UT_ASSERT(a != b, "operator == return true");
		UT_ASSERT(!(a == b), "operator != return false");
	} UT_END

	UT_RUN;
	UT_APP_END;

	UT_RETURN;
}