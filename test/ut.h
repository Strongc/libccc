#ifndef __UT_H__
#define __UT_H__

#include <stdio.h>
#include <vector>
#include <string>

namespace ut {
	struct ITestUnit {
		virtual int run() = 0;
		virtual std::string name() = 0;
	};

	int g_nTestRet;

#ifdef UT_BLOCK_BEFORE_QUIT
	char getchar() {
		return ::getchar();
	}
#else
	char getchar() {return '\0';}
#endif //UT_BLOCK_BEFORE_QUIT
}

// main函数返回
#define UT_RETURN \
	if (ut::g_nTestRet) printf("[UT] ******** Test Failed! ********");\
	ut::getchar();\
	return ut::g_nTestRet;

// 定义单元测试
#define UT_APP(name) \
	do {\
		printf("[UT] ======== %s Test ========\n\n", (#name));\
		std::string appName(#name);\
		std::vector<ut::ITestUnit*> vecUnits;
		bool success = true;

// 定义单元测试结束
#define UT_APP_END \
		for (std::vector<ut::ITestUnit*>::iterator it = vecUnits.begin(); it != vecUnits.end(); ++it) {\
			delete (*it);\
		}\
		printf("[UT] ======== %s Test %s ========\n\n", appName.c_str(), (success ? "Success" : "Failed"));\
	} while (0);

#define _UT_PRINT_START(name) printf("[UT] Test Case Start, name = %s\n", (name));
#define _UT_PRINT_END(name) printf("[UT] Test Case End, name = %s\n", (name));

// 断言
#define UT_ASSERT(expr, nonce) _UT_ASSERT(expr, nonce, __FILE__, __LINE__)
#define _UT_ASSERT(expr, nonce, file, line) \
	do {\
		if (!(expr)) {\
			printf("[UT] Assert Failed, %s\n\tfile = %s\n\tline = %d\n", (nonce), file, line);\
			return -1;\
		}\
	} while (0);

// 定义测试项开始
#define UT(uname) \
	do {\
		std::string strUname(#uname);\
		typedef struct TestUnit : public ut::ITestUnit {\
			std::string unitname_;\
			TestUnit(const std::string& uinitname) : unitname_(uinitname) {}\
			virtual std::string name() {\
				return unitname_;\
			}\
			virtual int run() {\
				_UT_PRINT_START(unitname_.c_str())\
                try {

// 定义测试项结束
#define UT_END \
                } catch (std::exception& e) {\
					printf("[UT] Exception Catched, e = %s\n", e.what());\
					return -1;\
				} catch (...) {\
					printf("[UT] Exception Catched, e is unknown\n");\
				}\
				_UT_PRINT_END(unitname_.c_str())\
				return 0;\
			}\
		} TestUnit;\
		vecUnits.push_back(new TestUnit(strUname));\
	} while (0);

// 运行已定义的测试项
#define UT_RUN \
	do {\
		for (std::vector<ut::ITestUnit*>::iterator it = vecUnits.begin(); it != vecUnits.end(); ++it) {\
			int nRet = (*it)->run();\
			if (nRet != 0) {\
				printf("[UT] Test Case Failed, name = %s\n\n", (*it)->name().c_str());\
				success = false;\
				ut::g_nTestRet = -1;\
			} else {\
				printf("[UT] Test Case Success, name = %s\n\n", (*it)->name().c_str());\
			}\
		}\
	} while (0);

#endif // __UT_H__