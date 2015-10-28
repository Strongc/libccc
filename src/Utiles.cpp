#include "Utiles.h"
#include "internal/Common_internal.h"

namespace ccc {

void msleep(unsigned ms) {
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(1000* ms);
#endif
}

}
