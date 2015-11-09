#ifndef __CCC_BIND_H__
#define __CCC_BIND_H__

#include "Common.h"
#include "RefPtr.h"

namespace ccc {

	namespace internal_ {
		struct Trigger0Base {
			virtual void fire() = 0;
			virtual Trigger0Base* clone() = 0;
			virtual bool equal(const Trigger0Base* pOther)  = 0;
		};

		template <typename P>
		struct Trigger1Base {
			virtual void fire(P p) = 0;
			virtual Trigger1Base* clone() = 0;
			virtual bool equal(const Trigger1Base* pOther)  = 0;
		};
		
		template <typename P1, typename P2>
		struct Trigger2Base {
			virtual void fire(P1 p1, P2 p2) = 0;
			virtual Trigger2Base* clone() = 0;
			virtual bool equal(const Trigger2Base* pOther)  = 0;
		};
		
		template <typename P1, typename P2, typename P3>
		struct Trigger3Base {
			virtual void fire(P1 p1, P2 p2, P3 p3) = 0;
			virtual Trigger3Base* clone() = 0;
			virtual bool equal(const Trigger3Base* pOther)  = 0;
		};
		
		typedef ccc::Void Void;
		
		template <typename T>
		struct NakedThatTraits {
			typedef T* naked_ptr_type;
			typedef T* ref_ptr_type;
			typedef T* strong_ptr_type;
			
			static strong_ptr_type lock(strong_ptr_type ptr) {
				return ptr;
			}

			static naked_ptr_type naked(ref_ptr_type ptr) {
				return ptr;
			}
		};
		
		template <typename T>
		struct RefThatTraits {
			typedef T* naked_ptr_type;
			typedef ccc::RefPtr<T> ref_ptr_type;
			typedef ccc::RefPtr<T> strong_ptr_type;
			
			static strong_ptr_type lock(ref_ptr_type ptr) {
				return ptr;
			}

			static naked_ptr_type naked(strong_ptr_type ptr) {
				return ptr.ptr();
			}
		};
		
		template <typename T>
		struct WeakThatTraits {
			typedef T* naked_ptr_type;
			typedef ccc::WeakPtr<T> ref_ptr_type;
			typedef ccc::RefPtr<T> strong_ptr_type;
			
			static strong_ptr_type lock(ref_ptr_type ptr) {
				return ptr.lock();
			}

			static naked_ptr_type naked(strong_ptr_type ptr) {
				return ptr.ptr();
			}
		};
		
		template <typename T, typename R, typename ThatTraits>
		struct Trigger0 : public Trigger0Base {
			typedef R (T::*func_type)();
			typedef typename ThatTraits::ref_ptr_type ref_ptr_type;

			Trigger0(const ref_ptr_type& that, func_type f) : that_(that), f_(f) {}
			
			Trigger0& operator =(const Trigger0& other) {
				if (&other == this) return *this;
				
				that_ = other.that_;
				f_ = other.f_;
				
				return *this;
			}
			
			virtual void fire() {
				typename ThatTraits::strong_ptr_type ptr = ThatTraits::lock(that_);
				
				if (!ptr || !f_) {
					return;
				}

				ThatTraits::naked_ptr_type pn = ThatTraits::naked(ptr);

				(pn->*f_)();
			}
			
			virtual Trigger0Base* clone() {
				return new Trigger0(that_, f_);
			}

			virtual bool equal(const Trigger0Base* pOther) {
				if (pOther == this) return true;

				try {
					Trigger0* pConcret = dynamic_cast<Trigger0*>(const_cast<Trigger0Base*>(pOther));

					if (!pConcret) {
						return false;
					}

					return (pConcret->that_ == that_ && pConcret->f_ == f_);
				} catch (...) {
					return false;
				}
			}

			ref_ptr_type that_;
			func_type f_;
		};
		
		template <typename T, typename R, typename P, typename ThatTraits>
		struct Trigger1 : public Trigger1Base<P> {
			typedef R (T::*func_type)(P);
			typedef typename ThatTraits::ref_ptr_type ref_ptr_type;

			Trigger1(const ref_ptr_type& that, func_type f) : that_(that), f_(f) {}
			
			Trigger1& operator =(const Trigger1& other) {
				if (&other == this) return *this;
				
				that_ = other.that_;
				f_ = other.f_;
				
				return *this;
			}
			
			virtual void fire(P p) {
				typename ThatTraits::strong_ptr_type ptr = ThatTraits::lock(that_);
				
				if (!ptr || !f_) {
					return;
				}

				ThatTraits::naked_ptr_type pn = ThatTraits::naked(ptr);

				(pn->*f_)(p);
			}

			virtual Trigger1Base* clone() {
				return new Trigger1(that_, f_);
			}

			virtual bool equal(const Trigger1Base* pOther) {
				if (pOther == this) return true;

				try {
					Trigger1* pConcret = dynamic_cast<Trigger1*>(const_cast<Trigger1Base*>(pOther));

					if (!pConcret) {
						return false;
					}

					return (pConcret->that_ == that_ && pConcret->f_ == f_);
				} catch (...) {
					return false;
				}
			}

			ref_ptr_type that_;
			func_type f_;
		};
		
		template <typename T, typename R, typename P1, typename P2, typename ThatTraits>
		struct Trigger2 : public Trigger2Base<P1, P2> {
			typedef R (T::*func_type)(P1, P2);
			typedef typename ThatTraits::ref_ptr_type ref_ptr_type;

			Trigger2(const ref_ptr_type& that, func_type f) : that_(that), f_(f) {}
			
			Trigger2& operator =(const Trigger2& other) {
				if (&other == this) return *this;
				
				that_ = other.that_;
				f_ = other.f_;
				
				return *this;
			}
			
			virtual void fire(P1 p1, P2 p2) {
				typename ThatTraits::strong_ptr_type ptr = ThatTraits::lock(that_);
				
				if (!ptr || !f_) {
					return;
				}

				ThatTraits::naked_ptr_type pn = ThatTraits::naked(ptr);

				(pn->*f_)(p1, p2);
			}

			virtual Trigger2Base* clone() {
				return new Trigger2(that_, f_);
			}

			virtual bool equal(const Trigger2Base* pOther) {
				if (pOther == this) return true;

				try {
					Trigger2* pConcret = dynamic_cast<Trigger2*>(const_cast<Trigger2Base*>(pOther));

					if (!pConcret) {
						return false;
					}

					return (pConcret->that_ == that_ && pConcret->f_ == f_);
				} catch (...) {
					return false;
				}
			}

			ref_ptr_type that_;
			func_type f_;
		};
		
		template <typename T, typename R, typename P1, typename P2, typename P3, typename ThatTraits>
		struct Trigger3 : public Trigger3Base<P1, P2, P3> {
			typedef R (T::*func_type)(P1, P2, P3);
			typedef typename ThatTraits::ref_ptr_type ref_ptr_type;

			Trigger3(const ref_ptr_type& that, func_type f) : that_(that), f_(f) {}
			
			Trigger3& operator =(const Trigger3& other) {
				if (&other == this) return *this;
				
				that_ = other.that_;
				f_ = other.f_;
				
				return *this;
			}
			
			virtual void fire(P1 p1, P2 p2, P3 p3) {
				typename ThatTraits::strong_ptr_type ptr = ThatTraits::lock(that_);
				
				if (!ptr || !f_) {
					return;
				}

				ThatTraits::naked_ptr_type pn = ThatTraits::naked(ptr);

				(pn->*f_)(p1, p2, p3);
			}
			
			virtual Trigger3Base* clone() {
				return new Trigger3(that_, f_);
			}

			virtual bool equal(const Trigger3Base* pOther) {
				if (pOther == this) return true;

				try {
					Trigger3* pConcret = dynamic_cast<Trigger3*>(const_cast<Trigger3Base*>(pOther));

					if (!pConcret) {
						return false;
					}

					return (pConcret->that_ == that_ && pConcret->f_ == f_);
				} catch (...) {
					return false;
				}
			}

			ref_ptr_type that_;
			func_type f_;
		};
		
		template <typename R>
		struct Trigger0<Void, R, Void> : public Trigger0Base {
			typedef R (*func_type)();

			Trigger0(func_type f) : f_(f) {}
			
			Trigger0& operator =(const Trigger0& other) {
				if (&other == this) return *this;
				
				f_ = other.f_;
				
				return *this;
			}
			
			virtual void fire() {
				if (!f_) {
					return;
				}

				f_();
			}
			
			virtual Trigger0Base* clone() {
				return new Trigger0(f_);
			}

			virtual bool equal(const Trigger0Base* pOther) {
				if (pOther == this) return true;

				try {
					Trigger0* pConcret = dynamic_cast<Trigger0*>(const_cast<Trigger0Base*>(pOther));

					if (!pConcret) {
						return false;
					}

					return (pConcret->f_ == f_);
				} catch (...) {
					return false;
				}
			}

			func_type f_;
		};
		
		template <typename R, typename P>
		struct Trigger1<Void, R, P, Void> : public Trigger1Base<P> {
			typedef R (*func_type)(P);

			Trigger1(func_type f) : f_(f) {}
			
			Trigger1& operator =(const Trigger1& other) {
				if (&other == this) return *this;

				f_ = other.f_;
				
				return *this;
			}
			
			virtual void fire(P p) {
				if (!f_) {
					return;
				}

				f_(p);
			}

			virtual Trigger1Base* clone() {
				return new Trigger1(f_);
			}

			virtual bool equal(const Trigger1Base* pOther) {
				if (pOther == this) return true;

				try {
					Trigger1* pConcret = dynamic_cast<Trigger1*>(const_cast<Trigger1Base*>(pOther));

					if (!pConcret) {
						return false;
					}

					return (pConcret->f_ == f_);
				} catch (...) {
					return false;
				}
			}

			func_type f_;
		};
		
		template <typename R, typename P1, typename P2>
		struct Trigger2<Void, R, P1, P2, Void> : public Trigger2Base<P1, P2> {
			typedef R (*func_type)(P1, P2);

			Trigger2(func_type f) : f_(f) {}
			
			Trigger2& operator =(const Trigger2& other) {
				if (&other == this) return *this;

				f_ = other.f_;
				
				return *this;
			}
			
			virtual void fire(P1 p1, P2 p2) {
				if (!f_) {
					return;
				}

				f_(p1, p2);
			}

			virtual Trigger2Base* clone() {
				return new Trigger2(f_);
			}

			virtual bool equal(const Trigger2Base* pOther) {
				if (pOther == this) return true;

				try {
					Trigger2* pConcret = dynamic_cast<Trigger2*>(const_cast<Trigger2Base*>(pOther));

					if (!pConcret) {
						return false;
					}

					return (pConcret->f_ == f_);
				} catch (...) {
					return false;
				}
			}

			func_type f_;
		};
		
		template <typename R, typename P1, typename P2, typename P3>
		struct Trigger3<Void, R, P1, P2, P3, Void> : public Trigger3Base<P1, P2, P3> {
			typedef R (*func_type)(P1, P2, P3);

			Trigger3(func_type f) : f_(f) {}
			
			Trigger3& operator =(const Trigger3& other) {
				if (&other == this) return *this;

				f_ = other.f_;
				
				return *this;
			}
			
			virtual void fire(P1 p1, P2 p2, P3 p3) {
				if (!f_) {
					return;
				}

				f_(p1, p2, p3);
			}
			
			virtual Trigger3Base* clone() {
				return new Trigger3(f_);
			}

			virtual bool equal(const Trigger3Base* pOther) {
				if (pOther == this) return true;

				try {
					Trigger3* pConcret = dynamic_cast<Trigger3*>(const_cast<Trigger3Base*>(pOther));

					if (!pConcret) {
						return false;
					}

					return (pConcret->f_ == f_);
				} catch (...) {
					return false;
				}
			}

			func_type f_;
		};
	}

	/**
	 * @class Proc0
	 * @brief 无参数的成员函数绑定对象
	 */
	class Proc0 {
		typedef internal_::Void Void;

	public:
		Proc0() : pTrigger_(0) {}

		Proc0(const Proc0& other) : pTrigger_(0) {
			if (other.pTrigger_) {
				pTrigger_ = other.pTrigger_->clone();
			}
		}

		template <typename T, typename R, typename ThatTraits>
		static Proc0 create(const typename ThatTraits::ref_ptr_type& that, R (T::*func)()) {
			CCC_ASSERT(!!that && func);

			Proc0 p;

			if (!!that) {
				p.pTrigger_ = new internal_::Trigger0<T, R, ThatTraits>(that, func);
			}

			return p;
		}
		
		template <typename R>
		static Proc0 createc(R (*func)()) {
			CCC_ASSERT(func);

			Proc0 p;
			p.pTrigger_ = new internal_::Trigger0<Void, R, Void>(func);

			return p;
		}

		~Proc0() {
			if (pTrigger_) delete pTrigger_;
		}

		bool equal(const Proc0& other) const {
			if (this == &other) return true;
			return (pTrigger_ && pTrigger_->equal(other.pTrigger_));
		}

		Proc0& operator =(const Proc0& other) {
			if (this == &other) return *this;

			if (pTrigger_) delete pTrigger_;
			pTrigger_ = other.pTrigger_->clone();

			return *this;
		}
		
		bool operator ==(const Proc0& other) const {
			return this->equal(other);
		}
		
		bool operator !=(const Proc0& other) const {
			return !this->equal(other);
		}

		void operator ()() {
			if (pTrigger_) pTrigger_->fire();
		}
		
	private:
		internal_::Trigger0Base* pTrigger_;
	};

	/**
	 * @class Proc1
	 * @brief 1参数的成员函数绑定对�?	
	 */
	template <typename P>
	class Proc1 {
		typedef internal_::Void Void;

	public:
		Proc1() : pTrigger_(0) {}

		Proc1(const Proc1& other) : pTrigger_(0) {
			if (other.pTrigger_) {
				pTrigger_ = other.pTrigger_->clone();
			}
		}

		template <typename T, typename R, typename ThatTraits>
		static Proc1 create(const typename ThatTraits::ref_ptr_type& that, R (T::*func)(P)) {
			CCC_ASSERT(!!that && func);
			
			Proc1 p;
			if (!!that) {
				p.pTrigger_ = new internal_::Trigger1<T, R, P, ThatTraits>(that, func);
			}

			return p;
		}
		
		template <typename R>
		static Proc1 createc(R (*func)(P)) {
			CCC_ASSERT(func);
			
			Proc1 p;
			p.pTrigger_ = new internal_::Trigger1<Void, R, P, Void>(func);
			
			return p;
		}

		~Proc1() {
			if (pTrigger_) delete pTrigger_;
		}

		bool equal(const Proc1& other) const {
			if (this == &other) return true;
			return (pTrigger_ && pTrigger_->equal(other.pTrigger_));
		}

		Proc1& operator =(const Proc1& other) {
			if (this == &other) return *this;
			
			if (pTrigger_) delete pTrigger_;
			pTrigger_ = other.pTrigger_->clone();
			
			return *this;
		}
		
		bool operator ==(const Proc1& other) const {
			return this->equal(other);
		}
		
		bool operator !=(const Proc1& other) const {
			return !this->equal(other);
		}

		void operator ()(P p) {
			if (pTrigger_) pTrigger_->fire(p);
		}
		
	private:
		internal_::Trigger1Base<P>* pTrigger_;
	};
	
	/**
	 * @class Proc2
	 * @brief 2参数的成员函数绑定对�?	 */
	template <typename P1, typename P2>
	class Proc2 {
		typedef internal_::Void Void;

	public:
		Proc2() : pTrigger_(0) {}

		Proc2(const Proc2& other) : pTrigger_(0) {
			if (other.pTrigger_) {
				pTrigger_ = other.pTrigger_->clone();
			}
		}

		template <typename T, typename R, typename ThatTraits>
		static Proc2 create(const typename ThatTraits::ref_ptr_type& that, R (T::*func)(P1, P2)) {
			CCC_ASSERT(!!that && func);
			
			Proc2 p;

			if (!!that) {
				p.pTrigger_ = new internal_::Trigger2<T, R, P1, P2, ThatTraits>(that, func);
			}

			return p;
		}
		
		template <typename R>
		static Proc2 createc(R (*func)(P1, P2)) {
			CCC_ASSERT(func);
			
			Proc2 p;
			p.pTrigger_ = new internal_::Trigger2<Void, R, P1, P2, Void>(func);
			
			return p;
		}

		~Proc2() {
			if (pTrigger_) delete pTrigger_;
		}

		bool equal(const Proc2& other) const {
			if (this == &other) return true;
			return (pTrigger_ && pTrigger_->equal(other.pTrigger_));
		}

		Proc2& operator =(const Proc2& other) {
			if (this == &other) return *this;
			
			if (pTrigger_) delete pTrigger_;
			pTrigger_ = other.pTrigger_->clone();
			
			return *this;
		}
		
		bool operator ==(const Proc2& other) const {
			return this->equal(other);
		}
		
		bool operator !=(const Proc2& other) const {
			return !this->equal(other);
		}

		void operator ()(P1 p1, P2 p2) {
			if (pTrigger_) pTrigger_->fire(p1, p2);
		}
		
	private:
		internal_::Trigger2Base<P1, P2>* pTrigger_;
	};
	
	/**
	 * @class Proc3
	 * @brief 3参数的成员函数绑定对�?	
	 */
	template <typename P1, typename P2, typename P3>
	class Proc3 {
		typedef internal_::Void Void;

	public:
		Proc3() : pTrigger_(0) {}

		Proc3(const Proc3& other) : pTrigger_(0) {
			if (other.pTrigger_) {
				pTrigger_ = other.pTrigger_->clone();
			}
		}

		template <typename T, typename R, typename ThatTraits>
		static Proc3 create(const typename ThatTraits::ref_ptr_type& that, R (T::*func)(P1, P2, P3)) {
			CCC_ASSERT(!!that && func);
			
			Proc3 p;

			if (!!that) {
				p.pTrigger_ = new internal_::Trigger3<T, R, P1, P2, P3, ThatTraits>(that, func);
			}

			return p;
		}
		
		template <typename R>
		static Proc3 createc(R (*func)(P1, P2, P3)) {
			CCC_ASSERT(func);
			
			Proc3 P;
			p.pTrigger_ = new internal_::Trigger3<Void, R, P1, P2, P3, Void>(func);
			
			return p;
		}


		~Proc3() {
			if (pTrigger_) delete pTrigger_;
		}

		bool equal(const Proc3& other) const {
			if (this == &other) return true;
			return (pTrigger_ && pTrigger_->equal(other.pTrigger_));
		}

		Proc3& operator =(const Proc3& other) {
			if (this == &other) return *this;
			
			if (pTrigger_) delete pTrigger_;
			pTrigger_ = other.pTrigger_->clone();
			
			return *this;
		}
		
		bool operator ==(const Proc3& other) const {
			return this->equal(other);
		}
		
		bool operator !=(const Proc3& other) const {
			return !this->equal(other);
		}

		void operator ()(P1 p1, P2 p2, P3 p3) {
			if (pTrigger_) pTrigger_->fire(p1, p2, p3);
		}

	private:
		internal_::Trigger3Base<P1, P2, P3>* pTrigger_;
	};

	/**
	 * 绑定无参数成员函数
	 * @param p this指针对象
	 * @param f 成员函数指针
	 * @return 生成的函数对象
	 */
	template <typename T, typename R>
	Proc0 bind0(T* p, R (T::*f)()) {
		CCC_ASSERT(!!p && f);
		return Proc0::create<T, R, internal_::NakedThatTraits<T> >(p, f);
	}
	
	template <typename T, typename R>
	Proc0 bind0(const RefPtr<T>& p, R (T::*f)()) {
		CCC_ASSERT(!!p && f);
		return Proc0::create<T, R, internal_::RefThatTraits<T> >(p, f);
	}
	
	template <typename T, typename R>
	Proc0 bind0(const WeakPtr<T>& p, R (T::*f)()) {
		CCC_ASSERT(!!p && f);
		return Proc0::create<T, R, internal_::WeakThatTraits<T> >(p, f);
	}
	
	/**
	 * 绑定1参数成员函数
	 * @param p this指针对象
	 * @param f 成员函数指针
	 * @return 生成的函数对�?	
	 */
	template <typename T, typename R, typename P>
	Proc1<P> bind1(T* p, R (T::*f)(P)) {
		CCC_ASSERT(!!p && f);
		return Proc1<P>::create<T, R, internal_::NakedThatTraits<T> >(p, f);
	}
	
	template <typename T, typename R, typename P>
	Proc1<P> bind1(RefPtr<T>& p, R (T::*f)(P)) {
		CCC_ASSERT(!!p && f);
		return Proc1<P>::create<T, R, internal_::RefThatTraits<T> >(p, f);
	}
	
	template <typename T, typename R, typename P>
	Proc1<P> bind1(WeakPtr<T>& p, R (T::*f)(P)) {
		CCC_ASSERT(!!p && f);
		return Proc1<P>::create<T, R, internal_::WeakThatTraits<T> >(p, f);
	}

	/**
	 * 绑定2参数成员函数
	 * @param p this指针对象
	 * @param f 成员函数指针
	 * @return 生成的函数对�?	
	 */
	template <typename T, typename R, typename P1, typename P2>
	Proc2<P1, P2> bind2(T* p, R (T::*f)(P1, P2)) {
		CCC_ASSERT(!!p && f);
		return Proc2<P1, P2>::create<T, R, internal_::NakedThatTraits<T> >(p, f);
	}
	
	template <typename T, typename R, typename P1, typename P2>
	Proc2<P1, P2> bind2(RefPtr<T>& p, R (T::*f)(P1, P2)) {
		CCC_ASSERT(!!p && f);
		return Proc2<P1, P2>::create<T, R, internal_::RefThatTraits<T> >(p, f);
	}
	
	template <typename T, typename R, typename P1, typename P2>
	Proc2<P1, P2> bind2(WeakPtr<T>& p, R (T::*f)(P1, P2)) {
		CCC_ASSERT(!!p && f);
		return Proc2<P1, P2>::create<T, R, internal_::WeakThatTraits<T> >(p, f);
	}

	/**
	 * 绑定3参数成员函数
	 * @param p this指针对象
	 * @param f 成员函数指针
	 * @return 生成的函数对�?	
	 */
	template <typename T, typename R, typename P1, typename P2, typename P3>
	Proc3<P1, P2, P3> bind3(T* p, R (T::*f)(P1, P2, P3)) {
		CCC_ASSERT(!!p && f);
		return Proc3<P1, P2, P3>::create<T, R, internal_::NakedThatTraits<T> >(p, f);
	}
	
	template <typename T, typename R, typename P1, typename P2, typename P3>
	Proc3<P1, P2, P3> bind3(RefPtr<T>& p, R (T::*f)(P1, P2, P3)) {
		CCC_ASSERT(!!p && f);
		return Proc3<P1, P2, P3>::create<T, R, internal_::RefThatTraits<T> >(p, f);
	}
	
	template <typename T, typename R, typename P1, typename P2, typename P3>
	Proc3<P1, P2, P3> bind3(WeakPtr<T>& p, R (T::*f)(P1, P2, P3)) {
		CCC_ASSERT(!!p && f);
		return Proc3<P1, P2, P3>::create<T, R, internal_::WeakThatTraits<T> >(p, f);
	}
	
	/**
	 * 绑定无参数函数
	 * @param f 函数指针
	 * @return 生成的函数对�?	
	 */
	template <typename R>
	Proc0 bind0(R (*f)()) {
		CCC_ASSERT(f);
		return Proc0::createc(f);
	}
	
	/**
	 * 绑定1参数函数
	 * @param f 函数指针
	 * @return 生成的函数对�?	
	 */
	template <typename R, typename P>
	Proc1<P> bind1(R (*f)(P)) {
		CCC_ASSERT(f);
		return Proc1<P>::createc<R>(f);
	}

	/**
	 * 绑定2参数函数
	 * @param f 函数指针
	 * @return 生成的函数对�?	
	 */
	template <typename R, typename P1, typename P2>
	Proc2<P1, P2> bind2(R (*f)(P1, P2)) {
		CCC_ASSERT(f);
		return Proc2<P1, P2>::createc<R>(f);
	}

	/**
	 * 绑定3参数函数
	 * @param f 函数指针
	 * @return 生成的函数对�?	
	 */
	template <typename R, typename P1, typename P2, typename P3>
	Proc3<P1, P2, P3> bind3(R (*f)(P1, P2, P3)) {
		CCC_ASSERT(f);
		return Proc3<P1, P2, P3>::createc<R>(f);
	}
}

#endif // __CCC_BIND_H__