#include "Utiles.h"
#include "internal/Common_internal.h"
#include "Atom.h"

namespace ccc {

void msleep(unsigned ms) {
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(1000 * ms);
#endif
}

unsigned getTicks() {
#ifdef _WIN32
	return (unsigned)GetTickCount();
#else
	struct timespec ts;
	unsigned theTick = 0U;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	theTick = ts.tv_nsec / 1000000;
	theTick += ts.tv_sec * 1000;
	return theTick;
#endif
}

unsigned getId() {
	static Atom atm(0);
	return atm.inc();
}

}
