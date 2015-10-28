#include "Atom.h"
#include "internal/Common_internal.h"

namespace ccc {

int Atom::get() const {
	return val_;
}

int Atom::set(int v) {
#ifdef _WIN32
	InterlockedExchange((LONG *)&val_, v);
#else
	val_ = v;
#endif
	return v;
}

int Atom::add(int v) {
	int tmp = v;
#ifdef _WIN32
	return InterlockedExchangeAdd((LONG *)&val_, tmp) + v;
#elif defined(__i386__) || defined(__x86_64__)
	asm volatile ("lock; xaddl %0,%1"
		: "=r" (tmp), "=m" (val_)
		: "0" (tmp), "m" (val_)
		: "memory", "cc");
	return tmp + v;
#else
	return __sync_fetch_and_add(&val_, tmp) + v;
#endif
}

int Atom::sub(int v) {
	return add(-v);
}

int Atom::inc() {
#ifdef _WIN32
	return InterlockedIncrement((LONG *)&val_);
#else
	return add(1);
#endif
}

int Atom::dec() {
#ifdef _WIN32
	return InterlockedDecrement((LONG *)&val_);
#else
	return add(-1);
#endif
}

int Atom::xchg(int v) {
#ifdef _WIN32
	return InterlockedExchange((LONG *)&val_, v);
#elif defined(__i386__) || defined(__x86_64__)
	int prev = v;
	asm volatile ("xchgl %0, %1"
		: "=r" (prev), "+m" (val_)
		: "0" (prev));
	return prev;
#else
	__sync_synchronize();
	return __sync_lock_test_and_set(&val_, v);
#endif
}

int Atom::xchgcmp(int v, int cmp) {
#ifdef _WIN32
	return InterlockedCompareExchange((LONG *)&val_, v, cmp);
#elif defined(__i386__) || defined(__x86_64__)
	int prev;
	asm volatile ("lock; cmpxchgl %1, %2"
		: "=a" (prev)
		: "r" (v), "m" (val_), "0"(cmp)
		: "memory", "cc");
	return prev;
#else
	return __sync_val_compare_and_swap(&val_, cmp, v);
#endif
}

}