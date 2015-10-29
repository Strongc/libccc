#ifndef __CCC_BIND_H__
#define __CCC_BIND_H__

#include "Common.h"
#include "RefPtr.h"

namespace ccc {

	namespace internal_ {
		struct MemProcTrigger0Base {
			virtual void proc() = 0;
			virtual MemProcTrigger0Base* clone() = 0;
		};

		template <typename P>
		struct MemProcTrigger1Base {
			virtual void proc(P p) = 0;
			virtual MemProcTrigger1Base* clone() = 0;
		};
		
		template <typename P1, typename P2>
		struct MemProcTrigger2Base {
			virtual void proc(P1 p1, P2 p2) = 0;
			virtual MemProcTrigger2Base* clone() = 0;
		};
		
		template <typename P1, typename P2, typename P3>
		struct MemProcTrigger3Base {
			virtual void proc(P1 p1, P2 p2, P3 p3) = 0;
			virtual MemProcTrigger3Base* clone() = 0;
		};
		
		template <typename T, typename R>
		struct MemProcTrigger0 : public MemProcTrigger0Base {
			typedef R (T::*func_type)();
			typedef ccc::WeakPtr<T> ref_ptr_type;

			MemProcTrigger0(ref_ptr_type that, func_type f) : that_(that), f_(f) {}
			
			MemProcTrigger0& operator =(const MemProcTrigger0& other) {
				if (&other == this) return *this;
				
				that_ = other.that_;
				f_ = other.f_;
				
				return *this;
			}
			
			virtual void proc() {
				ref_ptr_type::strong_ptr_type ptr = that_->lock();
				
				if (!ptr || !f_) {
					return;
				}

				that_->*f_();
			}
			
			virtual MemProcTrigger0Base* clone() {
				return new MemProcTrigger0(that_, f_);
			}

			ref_ptr_type that_;
			func_type f_;
		};
		
		template <typename T, typename R, typename P>
		struct MemProcTrigger1 : public MemProcTrigger1Base<P> {
			typedef R (T::*func_type)(P);
			typedef ccc::WeakPtr<T> ref_ptr_type;

			MemProcTrigger1(ref_ptr_type that, func_type f) : that_(that), f_(f) {}
			
			MemProcTrigger1& operator =(const MemProcTrigger1& other) {
				if (&other == this) return *this;
				
				that_ = other.that_;
				f_ = other.f_;
				
				return *this;
			}
			
			virtual void proc(P p) {
				ref_ptr_type::strong_ptr_type ptr = that_->lock();
				
				if (!ptr || !f_) {
					return;
				}

				that_->*f_(p);
			}

			virtual MemProcTrigger1Base* clone() {
				return new MemProcTrigger1(that_, f_);
			}

			ref_ptr_type that_;
			func_type f_;
		};
		
		template <typename T, typename R, typename P1, typename P2>
		struct MemProcTrigger2 : public MemProcTrigger2Base<P1, P2> {
			typedef R (T::*func_type)(P1, P2);
			typedef ccc::WeakPtr<T> ref_ptr_type;

			MemProcTrigger2(ref_ptr_type that, func_type f) : that_(that), f_(f) {}
			
			MemProcTrigger2& operator =(const MemProcTrigger2& other) {
				if (&other == this) return *this;
				
				that_ = other.that_;
				f_ = other.f_;
				
				return *this;
			}
			
			virtual void proc(P1 p1, P2 p2) {
				ref_ptr_type::strong_ptr_type ptr = that_->lock();
				
				if (!ptr || !f_) {
					return;
				}

				that_->*f_(p1, p2);
			}

			virtual MemProcTrigger2Base* clone() {
				return new MemProcTrigger2(that_, f_);
			}

			ref_ptr_type that_;
			func_type f_;
		};
		
		template <typename T, typename R, typename P1, typename P2, typename P3>
		struct MemProcTrigger3 : public MemProcTrigger3Base<P1, P2, P3> {
			typedef R (T::*func_type)(P1, P2, P3);
			typedef ccc::WeakPtr<T> ref_ptr_type;

			MemProcTrigger3(ref_ptr_type that, func_type f) : that_(that), f_(f) {}
			
			MemProcTrigger3& operator =(const MemProcTrigger3& other) {
				if (&other == this) return *this;
				
				that_ = other.that_;
				f_ = other.f_;
				
				return *this;
			}
			
			virtual void proc(P1 p1, P2 p2, P3 p3) {
				ref_ptr_type::strong_ptr_type ptr = that_->lock();
				
				if (!ptr || !f_) {
					return;
				}

				ptr->*f_(p1, p2, p3);
			}
			
			virtual MemProcTrigger3Base* clone() {
				return new MemProcTrigger3(that_, f_);
			}

			ref_ptr_type that_;
			func_type f_;
		};
	}

	/**
	 * @class Binded0
	 * @brief 无参数的成员函数绑定对象
	 */
	class Binded0 {
	public:
		Binded0() : pTrigger_(0) {}

		template <typename T, typename R,
			template <typename T> class PtrT>
		Binded0(PtrT<T> that, R (T::*func)()) {
			assert(that && func);
			pTrigger_ = new MemProcTrigger0<T, R, PtrT>(that, func);
		}

		~Binded0() {
			delete pTrigger_;
		}

		bool equal(const Binded0& other) const {
			return false;
		}

		Binded0& operator =(const Binded0& other) {
			if (this == &other) return *this;
			
			delete pTrigger_;
			pTrigger_ = other.pTrigger_->clone();
			
			return *this;
		}
		
		bool operator ==(const Binded0& other) const {
			return this->equal(other);
		}
		
		bool operator !=(const Binded0& other) const {
			return !this->equal(other);
		}

		void operator ()() {
			pTrigger_->proc();
		}
		
	private:
		internal_::MemProcTrigger0Base* pTrigger_;
	};

	/**
	 * @class Binded1
	 * @brief 1参数的成员函数绑定对豿
	 */
	template <typename P>
	class Binded1 {
	public:
		Binded1() : pTrigger_(0) {}

		template <typename T, typename R, template <typename T> class PtrT>
		Binded1(PtrT<T> that, R (T::*func)(P)) {
			assert(that && func);
			pTrigger_ = new MemProcTrigger1<T, R, P, PtrT>(that, func);
		}

		~Binded1() {
			delete pTrigger_;
		}

		bool equal(const Binded1& other) const {
			return false;
		}

		Binded1& operator =(const Binded1& other) {
			if (this == &other) return *this;
			
			delete pTrigger_;
			pTrigger_ = other.pTrigger_->clone();
			
			return *this;
		}
		
		bool operator ==(const Binded1& other) const {
			return this->equal(other);
		}
		
		bool operator !=(const Binded1& other) const {
			return !this->equal(other);
		}

		void operator ()(P p) {
			pTrigger_->proc(p);
		}
		
	private:
		internal_::MemProcTrigger1Base<P>* pTrigger_;
	};
	
	/**
	 * @class Binded2
	 * @brief 2参数的成员函数绑定对豿
	 */
	template <typename P1, typename P2>
	class Binded2 {
	public:
		Binded2() : pTrigger_(0) {}

		template <typename T, typename R,
			template <typename T> class PtrT>
		Binded2(PtrT<T> that, R (T::*func)(P1, P2)) {
			assert(that && func);
			pTrigger_ = new MemProcTrigger1<T, R, P1, P2, PtrT>(that, func);
		}

		~Binded2() {
			delete pTrigger_;
		}

		bool equal(const Binded2& other) const {
			return false;
		}

		Binded2& operator =(const Binded2& other) {
			if (this == &other) return *this;
			
			delete pTrigger_;
			pTrigger_ = other.pTrigger_->clone();
			
			return *this;
		}
		
		bool operator ==(const Binded2& other) const {
			return this->equal(other);
		}
		
		bool operator !=(const Binded2& other) const {
			return !this->equal(other);
		}

		void operator ()(P1 p1, P2 p2) {
			pTrigger_->proc(p1, p2);
		}
		
	private:
		internal_::MemProcTrigger2Base<P1, P2>* pTrigger_;
	};
	
	/**
	 * @class Binded3
	 * @brief 3参数的成员函数绑定对豿
	 */
	template <typename P1, typename P2, typename P3>
	class Binded3 {
	public:
		Binded3() : pTrigger_(0) {}

		template <typename T, typename R,
			template <typename T> class PtrT>
		Binded3(PtrT<T> that, R (T::*func)(P1, P2, P3)) {
			assert(that && func);
			pTrigger_ = new MemProcTrigger1<T, R, P1, P2, P3, PtrT>(that, func);
		}

		~Binded3() {
			delete pTrigger_;
		}

		bool equal(const Binded3& other) const {
			return false;
		}

		Binded3& operator =(const Binded3& other) {
			if (this == &other) return *this;
			
			delete pTrigger_;
			pTrigger_ = other.pTrigger_->clone();
			
			return *this;
		}
		
		bool operator ==(const Binded3& other) const {
			return this->equal(other);
		}
		
		bool operator !=(const Binded3& other) const {
			return !this->equal(other);
		}

		void operator ()(P1 p1, P2 p2, P3 p3) {
			pTrigger_->proc(p1, p2, p3);
		}

	private:
		internal_::MemProcTrigger3Base<P1, P2, P3>* pTrigger_;
	};

	/**
	 * @function 绑定无参数成员函敿
	 * @param p this指针对象
	 * @param f 成员函数指针
	 * @return 生成的函数对豿
	 */
	template <typename T, typename R>
	Binded0 bind(const WeakPtr<T>& p, R (T::*f)()) {
		assert(p && f);
		return Binded0(p, f);
	}
	
	/**
	 * @function 绑定1参数成员函数
	 * @param p this指针对象
	 * @param f 成员函数指针
	 * @return 生成的函数对豿
	 */
	template <typename T, typename R, typename P>
	Binded1<P> bind(const WeakPtr<T>& p, R (T::*f)(P)) {
		assert(p && f);
		return Binded1<P>(p, f);
	}

	/**
	 * @function 绑定2参数成员函数
	 * @param p this指针对象
	 * @param f 成员函数指针
	 * @return 生成的函数对豿
	 */
	template <typename T, typename R, typename P1, typename P2>
	Binded2<P1, P2> bind(const WeakPtr<T>& p, R (T::*f)(P1, P2)) {
		assert(p && f);
		return Binded2<P1, P2>(p, f);
	}

	/**
	 * @function 绑定3参数成员函数
	 * @param p this指针对象
	 * @param f 成员函数指针
	 * @return 生成的函数对豿
	 */
	template <typename T, typename R, typename P1, typename P2, typename P3>
	Binded3<P1, P2, P3> bind(const WeakPtr<T>& p, R (T::*f)(P1, P2, P3)) {
		assert(p && f);
		return Binded3<P1, P2, P3>(p, f);
	}
}

#endif // __CCC_BIND_H__