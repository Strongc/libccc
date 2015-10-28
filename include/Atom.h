#ifndef __CCC_ATOM_H__
#define __CCC_ATOM_H__

#include "Common.h"

namespace ccc {

	class Atom {
		CCC_NONCOPYLABLE(Atom)

	public:
		Atom(int val = 0) : val_(val) {}
		~Atom() {}

		int get() const;
		int set(int v); // return new value

		int add(int v); // return new value
		int sub(int v); // return new value
		int inc(); // return new value
		int dec(); // return new value

		int xchg(int v); // return old value
		int xchgcmp(int v, int cmp); // return old value

	private:
		volatile int val_;
	};
}

#endif // __CCC_ATOM_H__