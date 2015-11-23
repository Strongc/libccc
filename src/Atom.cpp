#include "Atom.h"
#include "Lock.h"
#include "internal/Common_internal.h"
#include <vector>

namespace ccc {

namespace internal_ {

#ifndef _WIN32
	class AtomMutexMap {
	public:
		AtomMutexMap() {
			for (int i  = 0; i < LOCK_COUNT; ++i) {
				mutexs_.push_back(new base::Lock);
			}
		}

		~AtomMutexMap() {
			for (int i  = 0; i < LOCK_COUNT; ++i) {
				delete mutexs_.at(i);
			}
		}

		base::Lock* getMutex(void *hash) {
			unsigned pos = (unsigned)hash % LOCK_COUNT;
			return mutexs_.at(pos);
		}

	private:
		enum {LOCK_COUNT = 13};
		std::vector<base::Mutex*> mutexs_;
	};

	AtomMutexMap g_atomMutexMap;
#endif // _WIN32
}

#if !defined(_WIN32) && !defined(CCC_ATOM_LOCK)
#define CCC_ATOM_LOCK(ptr) CCC_LOCK_P(internal_::g_atomMutexMap.getMutex(ptr))
#endif

long Atom::get() const {
	return val_;
}

long Atom::set(long v) {
#ifdef _WIN32
	InterlockedExchange((LONG *)&val_, v);
#else
	CCC_ATOM_LOCK(this);
	val_ = v;
#endif
	return v;
}

long Atom::add(long v) {
	int tmp = v;
#ifdef _WIN32
	return InterlockedExchangeAdd((LONG *)&val_, tmp) + v;
#else
	CCC_ATOM_LOCK(this);
	val_ += v;
	return val_;
#endif
}

long Atom::sub(long v) {
	return add(-v);
}

long Atom::inc() {
#ifdef _WIN32
	return InterlockedIncrement((LONG *)&val_);
#else
	return add(1);
#endif
}

long Atom::dec() {
#ifdef _WIN32
	return InterlockedDecrement((LONG *)&val_);
#else
	return add(-1);
#endif
}

long Atom::xchg(long v) {
#ifdef _WIN32
	return InterlockedExchange((LONG *)&val_, v);
#else
	CCC_ATOM_LOCK(this);
	long ret = val_;
	val_ = v;
	return ret;
#endif
}

long Atom::xchgcmp(long v, long cmp) {
#ifdef _WIN32
	return InterlockedCompareExchange((LONG *)&val_, v, cmp);
#else
	CCC_ATOM_LOCK(this);

	long ret = val_;
	if (val_ == cmp) {
		val_ = v;
	}

	return ret;
#endif
}

}