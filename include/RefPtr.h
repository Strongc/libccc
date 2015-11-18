#ifndef __CCC_REF_PTR_H__
#define __CCC_REF_PTR_H__

#include "Atom.h"
#include "Common.h"

namespace ccc {

	/**
	 * @class RefException
	 * @brief RefPtr专用异常
	 */
	class RefException : public std::exception {
	public:
		RefException(const char* msg) : std::exception(msg) {}
	};

	namespace internal_ {
	
		template <typename T>
		class SafeProxy {
			CCC_NONCOPYLABLE(SafeProxy)

		private:
			~SafeProxy() {
				if (p_) delete p_;
				CCC_DBG_PRINT("SafeProxy deleting\n");
			}

		public:
			typedef T value_type; 
			typedef T* ptr_type;

			explicit SafeProxy(ptr_type ptr) : p_(ptr), nRefCnt_(0) {
				CCC_DBG_PRINT("SafeProxy creating\n");
			}

			void addRef() {
				nRefCnt_.inc();
				CCC_DBG_PRINT("SafeProxy addRef, %u\n", nRefCnt_.get());
			}

			void release() {
				CCC_DBG_PRINT("SafeProxy release, %u\n", nRefCnt_.get() - 1);
				if (nRefCnt_.dec() == 0) {
					delete this;
				}
			}

			ptr_type p_;
			ccc::Atom nRefCnt_;
		};
		
		template <typename T>
		class RefProxy {
			CCC_NONCOPYLABLE(RefProxy)

		public:
			typedef T value_type; 
			typedef T* ptr_type;

			explicit RefProxy(ptr_type ptr) : p_(ptr), nRefCnt_(0) {
				CCC_DBG_PRINT("Ref creating\n");
			}

			~RefProxy() {
				CCC_DBG_PRINT("Ref deleting\n");
			}

			void incObjRefCount() {
				if (p_) {
					nRefCnt_.inc();
				}

				CCC_DBG_PRINT("Ref addRef, %u\n", nRefCnt_.get());
			}

			void decObjRefCount() {
				/*
				 * RefProxy 对象的生存期是由外部的SafePtr管理的，所以不需要也不能在release中析构自�?
				 */
				if (nRefCnt_.dec() <= 0) {
					if (p_) {
						delete p_;
						p_ = 0;
					}

					nRefCnt_.set(0);
				}

				CCC_DBG_PRINT("Ref release, %u\n", nRefCnt_.get());
			}

			bool empty() const {
				return (nRefCnt_.get() <= 0);
			}

			ptr_type p_;
			ccc::Atom nRefCnt_;
		};
		
		/**
		 * 简单引用计数指针，不支持weak引用
		 */
		template <typename T>
		class SafePtr {
			typedef internal_::SafeProxy<T> proxy_type;

		public:
			typedef T value_type; 
			typedef T* ptr_type;

			SafePtr() : pProxy_(0) {}

			SafePtr(ptr_type ptr) : pProxy_(0) {
				if (ptr) {
					pProxy_ = new proxy_type(ptr);
					pProxy_->addRef();
				}
			}

			SafePtr(const SafePtr& other) : pProxy_(0) {
				proxy_type* pRef = other.pProxy_;

				if (pRef) {
					pRef->addRef();
					pProxy_ = pRef;
				}
			}

			~SafePtr() {
				if (pProxy_) {
					pProxy_->release();
				}
			}

			bool empty() const {
				return (!pProxy_);
			}
			
			ptr_type ptr() {
				if (pProxy_) return pProxy_->p_;
				return 0;
			}
			
			const ptr_type ptr() const {
				if (pProxy_) return pProxy_->p_;
				return 0;
			}

			value_type& operator *() {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return *(pProxy_->p_);
			}
			
			const value_type& operator *() const {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return *(pProxy_->p_);
			}

			ptr_type operator ->() {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return pProxy_->p_;
			}
			
			const ptr_type operator ->() const {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return pProxy_->p_;
			}

			SafePtr& operator =(const SafePtr& other) {
				if (this == &other) {
					return *this;
				}

				if (pProxy_) {
					pProxy_->release();
					pProxy_ = 0;
				}

				proxy_type* pProxy = other.pProxy_;

				if (pProxy) {
					pProxy->addRef();
					pProxy_ = pProxy;
				}

				return *this;
			}
			
			SafePtr& operator =(ptr_type ptr) {
				if (pProxy_) {
					pProxy_->release();
					pProxy_ = 0;
				}
				
				if (ptr) {
					pProxy_ = new proxy_type(ptr);
					pProxy_->addRef();
				}

				return *this;
			}

			bool operator ==(const SafePtr& other) const {
				return (pProxy_ == other.pProxy_);
			}
			
			bool operator !=(const SafePtr& other) const {
				return (pProxy_ != other.pProxy_);
			}

			operator bool() {
				return !this->empty();
			}
			
			bool operator !() const {
				return this->empty();
			}

		private:
			mutable proxy_type* pProxy_;
		};

		/**
		 * 引用计数指针基类
		 */
		template <typename T>
		class RefPtrBase {
			CCC_NONCOPYLABLE(RefPtrBase)

		protected:
			typedef RefProxy<T> proxy_type;
			typedef SafePtr<proxy_type> proxy_ptr_type;

		public:
			typedef T value_type;
			typedef T* ptr_type;
			
			RefPtrBase() : pProxy_(0) {}

			virtual ~RefPtrBase() {
				pProxy_ = 0;
			}

			ptr_type ptr() {
				if (this->empty()) {
					return 0;
				}

				return (pProxy_->p_);
			}

			const ptr_type ptr() const {
				if (this->empty()) {
					return 0;
				}

				return (pProxy_->p_);
			}

			bool empty() const {
				// 没有指向任何RefProxy对象或者RefProxy内部为空
				return !pProxy_ || pProxy_->empty();
			}

			value_type& operator *() {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return *(pProxy_->p_);
			}
			
			const value_type& operator *() const {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return *(pProxy_->p_);
			}

			ptr_type operator ->() {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return (pProxy_->p_);
			}
			
			const ptr_type operator ->() const {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return (pProxy_->p_);
			}

			operator bool() {
				return !this->empty();
			}
			
			bool operator !() const {
				return this->empty();
			}

		protected:
			/*
			 * 指向RefProxy的智能指针，RefProxy控制对象的生存期，SafeProxy控制RefProxy的生存期
			 */
			mutable proxy_ptr_type pProxy_;
		};
	}

	template <typename T> class WeakPtr;

	/**
	 * @class RefPtr
	 * @brief 引用计数指针
	 */
	template <typename T>
	class RefPtr : public internal_::RefPtrBase<T> {
		friend class WeakPtr<T>;
		
		void setRefProxy(proxy_ptr_type pProxy) {
			try {
				pProxy->incObjRefCount();
			} catch (RefException& e) {
				(void)e;
				return;
			}

			pProxy_ = pProxy;
		}

	public:
		RefPtr() {}
		
		RefPtr(const RefPtr& other) {
			pProxy_ = other.pProxy_;
			
			if (!this->empty()) {
				pProxy_->incObjRefCount();
			}
		}
		
		explicit RefPtr(ptr_type ptr) {
			if (ptr) {
				pProxy_ = new proxy_type(ptr);
				pProxy_->incObjRefCount();
			}
		}
		
		virtual ~RefPtr() {
			if (!this->empty()) {
				pProxy_->decObjRefCount();
			}
		}

		RefPtr& operator =(const RefPtr& other) {
			if (&other == this) {
				return *this;
			}
			
			if (!this->empty()) {
				pProxy_->decObjRefCount();
			}

			pProxy_ = other.pProxy_;
			
			if (!this->empty()) {
				pProxy_->incObjRefCount();
			}

			return *this;
		}
		
		RefPtr& operator =(ptr_type ptr) {
			if (!this->empty()) {
				pProxy_->decObjRefCount();
				pProxy_ = 0;
			}

			if (ptr) {
				pProxy_ = new proxy_type(ptr);
				pProxy_->incObjRefCount();
			}
			
			return *this;
		}

		bool operator ==(const RefPtr& other) const {
			return (pProxy_ == other.pProxy_);
		}
		
		bool operator !=(const RefPtr& other) const {
			return (pProxy_ != other.pProxy_);
		}
	};

	/**
	 * @class WeakPtr
	 * @brief 弱引用指�?
	 */
	template <typename T>
	class WeakPtr : public internal_::RefPtrBase<T> {
	public:
		typedef RefPtr<T> strong_ptr_type;

		WeakPtr() {}

		explicit WeakPtr(const strong_ptr_type& other) {
			pProxy_ = other.pProxy_;
		}

		WeakPtr(const WeakPtr& other) {
			pProxy_ = other.pProxy_;
		}

		virtual ~WeakPtr() {
			pProxy_ = 0;
		}

		// 转换为强引用
		strong_ptr_type lock() {
			strong_ptr_type ret;
			ret.setRefProxy(pProxy_);			
			return ret;
		}

		WeakPtr& operator =(const WeakPtr& other) {
			if (&other != this) {
				pProxy_ = other.pProxy_;
			}

			return *this;
		}
		
		WeakPtr& operator =(const strong_ptr_type& other) {
			pProxy_ = other.pProxy_;
			return *this;
		}

		bool operator ==(const WeakPtr& other) const {
			return (pProxy_ == other.pProxy_);
		}

		bool operator ==(const strong_ptr_type& other) const {
			return (pProxy_ == other.pProxy_);
		}
		
		bool operator !=(const WeakPtr& other) const {
			return (pProxy_ != other.pProxy_);
		}

		bool operator !=(const strong_ptr_type& other) const {
			return (pProxy_ != other.pProxy_);
		}
	};
}

#endif // __CCC_REF_PTR_H__