#ifndef __CCC_ATOM_H__
#define __CCC_ATOM_H__

#include "Common.h"

namespace ccc {

	class Atom {
		CCC_NONCOPYABLE(Atom)

	public:
		Atom(long val = 0) : val_(val) {}
		~Atom() {}

		long get() const;
		long set(long v); // return new value

		long add(long v); // return new value
		long sub(long v); // return new value
		long inc(); // return new value
		long dec(); // return new value

		long xchg(long v); // return old value
		long xchgcmp(long v, long cmp); // return old value

	private:
		volatile long val_;
	};
}

#endif // __CCC_ATOM_H__