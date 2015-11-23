#ifndef __LC_BUFFER_H__
#define __LC_BUFFER_H__

#include "Common.h"
#include <string.h>

namespace base {

	template <typename POD>
	class BufferT {
		LC_INCOMPARABLE(BufferT);

	public:
		typedef POD element_type;
		typedef POD* ptr_type;

		BufferT(ptr_type src, unsigned count) : data(0), len(count) {
			if (!src || !count) {
				return;
			}
			
			data = new element_type[count];
			memcpy(data, src, count);
		}

		~BufferT() {
			if (data) delete [] data;
		}
		
		BufferT& operator =(const BufferT& other) {
			if (&other == this) {
				return *this;
			}
			
			if (data) delete [] data;
			len = 0;
			
			if (other.len) {
				data = new element_type[other.len];
				memcpy(data, other.data, other.len);
				len = other.len;
			}
		}

		ptr_type data;
		unsigned len;
	};
	
	typedef BufferT<char> Buffer;
}

#endif // __LC_BUFFER_H__