#ifndef __CCC_REF_PTR_H__
#define __CCC_REF_PTR_H__

#include "Atom.h"
#include "Common.h"

namespace ccc {

	/**
	 * @class RefException
	 * @brief SimplePtr专用异常
	 */
	class RefException : public std::exception {
	public:
		RefException(const char* msg) : std::exception(msg) {}
	};

	namespace internal_ {
	
		template <typename T>
		class SimpleRef {
			CCC_NONCOPYLABLE(SimpleRef)

		private:
			~SimpleRef() {
				if (p_) delete p_;
				CCC_DBG_PRINT("SimpleRef deleting\n");
			}

		public:
			typedef T value_type; 
			typedef T* ptr_type;

			explicit SimpleRef(ptr_type ptr) : p_(ptr), nRefCnt_(0) {
				CCC_DBG_PRINT("SimpleRef creating\n");
			}

			void addRef() {
				nRefCnt_.inc();
				CCC_DBG_PRINT("SimpleRef addRef, %u\n", nRefCnt_.get());
			}

			void release() {
				CCC_DBG_PRINT("SimpleRef release, %u\n", nRefCnt_.get() - 1);
				if (nRefCnt_.dec() == 0) {
					delete this;
				}
			}

			ptr_type p_;
			ccc::Atom nRefCnt_;
		};
		
		template <typename T>
		class Ref {
			CCC_NONCOPYLABLE(Ref)

		public:
			typedef T value_type; 
			typedef T* ptr_type;

			explicit Ref(ptr_type ptr) : p_(ptr), nRefCnt_(0) {
				CCC_DBG_PRINT("Ref creating\n");
			}

			~Ref() {
				CCC_DBG_PRINT("Ref deleting\n");
			}

			void addRef() {
				if (p_) {
					nRefCnt_.inc();
				}

				CCC_DBG_PRINT("Ref addRef, %u\n", nRefCnt_.get());
			}

			void release() {
				if (nRefCnt_.dec() <= 0) {
					CCC_ASSERT(nRefCnt_.get() == 0);

					if (p_) {
						delete p_;
						p_ = 0;
					}

					nRefCnt_.set(0);
				}

				CCC_DBG_PRINT("Ref release, %u\n", nRefCnt_.get());
			}
			
			bool empty() const {
				return (p_ == 0);
			}

			ptr_type p_;
			ccc::Atom nRefCnt_;
		};
		
		/**
		 * 简单引用计数指针，不支持weak引用
		 */
		template <typename T>
		class SimplePtr {
			typedef internal_::SimpleRef<T> ref_type;

		public:
			typedef T value_type; 
			typedef T* ptr_type;

			SimplePtr() : pRef_(0) {}

			SimplePtr(ptr_type ptr) : pRef_(0) {
				if (ptr) {
					pRef_ = new ref_type(ptr);
					pRef_->addRef();
				}
			}

			SimplePtr(const SimplePtr& other) : pRef_(0) {
				ref_type* pRef = other.pRef_;

				if (pRef) {
					pRef->addRef();
					pRef_ = pRef;
				}
			}

			~SimplePtr() {
				if (pRef_) {
					pRef_->release();
				}
			}

			bool empty() const {
				return (!pRef_ || !pRef_->p_);
			}
			
			ptr_type ptr() {
				if (pRef_) return pRef_->p_;
				return 0;
			}
			
			const ptr_type ptr() const {
				if (pRef_) return pRef_->p_;
				return 0;
			}

			value_type& operator *() {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return *(pRef_->p_);
			}
			
			const value_type& operator *() const {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return *(pRef_->p_);
			}

			ptr_type operator ->() {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return pRef_->p_;
			}
			
			const ptr_type operator ->() const {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return pRef_->p_;
			}

			SimplePtr& operator =(const SimplePtr& other) {
				if (this == &other) {
					return *this;
				}

				if (pRef_) {
					pRef_->release();
					pRef_ = 0;
				}

				ref_type* pRef = other.pRef_;

				if (pRef) {
					pRef->addRef();
					pRef_ = pRef;
				}

				return *this;
			}
			
			SimplePtr& operator =(ptr_type ptr) {
				if (pRef_) {
					pRef_->release();
					pRef_ = 0;
				}
				
				if (ptr) {
					pRef_ = new ref_type(ptr);
					pRef_->addRef();
				}

				return *this;
			}

			bool operator ==(const SimplePtr& other) const {
				return (pRef_ == other.pRef_);
			}

			operator bool() {
				return !this->empty();
			}
			
			bool operator !() const {
				return this->empty();
			}

		private:
			mutable ref_type* pRef_;
		};

		/**
		 * 引用计数指针基类
		 */
		template <typename T>
		class RefPtrBase {
			CCC_NONCOPYLABLE(RefPtrBase)

		protected:
			typedef Ref<T> ref_type;
			typedef SimplePtr<ref_type> handle_type;

		public:
			typedef T value_type;
			typedef T* ptr_type;
			
			RefPtrBase() {}

			virtual ~RefPtrBase() {
				handle_ = 0;
			}

			ptr_type ptr() {
				if (this->empty()) {
					return 0;
				}

				return (handle_->p_);
			}

			const ptr_type ptr() const {
				if (this->empty()) {
					return 0;
				}

				return (handle_->p_);
			}

			bool empty() const {
				// 没有指向任何Ref或者Ref内部是空的
				return handle_.empty() || handle_->empty();
			}

			value_type& operator *() {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return *(handle_->p_);
			}
			
			const value_type& operator *() const {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return *(handle_->p_);
			}

			ptr_type operator ->() {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return (handle_->p_);
			}
			
			const ptr_type operator ->() const {
				if (this->empty()) {
					throw RefException("empty ptr.");
				}

				return (handle_->p_);
			}

			operator bool() {
				return !this->empty();
			}
			
			bool operator !() const {
				return this->empty();
			}

		protected:
			mutable handle_type handle_;
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

	public:
		RefPtr() {}
		
		RefPtr(const RefPtr& other) {
			handle_ = other.handle_;
			
			if (!handle_.empty()) {
				handle_->addRef();
			}
		}
		
		explicit RefPtr(ptr_type ptr) {
			if (ptr) {
				handle_ = new ref_type(ptr);
				handle_->addRef();
			}
		}
		
		virtual ~RefPtr() {
			if (!this->empty()) {
				handle_->release();
			}
		}

		RefPtr& operator =(const RefPtr& other) {
			if (&other == this) {
				return *this;
			}
			
			if (!handle_.empty()) {
				handle_->release();
			}

			handle_ = other.handle_;
			
			if (!handle_.empty()) {
				handle_->addRef();
			}

			return *this;
		}
		
		RefPtr& operator =(ptr_type ptr) {
			if (!handle_.empty()) {
				handle_->release();
				handle_ = 0;
			}

			if (ptr) {
				handle_ = new ref_type(ptr);
				handle_->addRef();
			}
			
			return *this;
		}

		bool operator ==(const RefPtr& other) const {
			return (handle_ == other.handle_);
		}
		
		bool operator !=(const RefPtr& other) const {
			return (handle_ != other.handle_);
		}
	};

	/**
	 * @class WeakPtr
	 * @brief 弱引用指针
	 */
	template <typename T>
	class WeakPtr : public internal_::RefPtrBase<T> {
	public:
		typedef RefPtr<T> strong_ptr_type;

		WeakPtr() {}

		explicit WeakPtr(const strong_ptr_type& other) {
			handle_ = other.handle_;
		}

		WeakPtr(const WeakPtr& other) {
			handle_ = other.handle_;
		}

		virtual ~WeakPtr() {
			handle_ = 0;
		}

		// 转换为强引用
		strong_ptr_type lock() {
			RefPtr<T> ret;

			try {
				handle_->addRef();
			} catch (RefException& e) {
				(void)e;
				return ret;
			}

			ret.handle_ = handle_;
			
			return ret;
		}

		WeakPtr& operator =(const WeakPtr& other) {
			if (&other != this) {
				handle_ = other.handle_;
			}

			return *this;
		}
		
		WeakPtr& operator =(const strong_ptr_type& other) {
			if (&other != this) {
				handle_ = other.handle_;
			}

			return *this;
		}

		bool operator ==(const WeakPtr& other) const {
			return (handle_ == other.handle_);
		}

		bool operator ==(const strong_ptr_type& other) const {
			return (handle_ == other.handle_);
		}
		
		bool operator !=(const WeakPtr& other) const {
			return (handle_ != other.handle_);
		}

		bool operator !=(const strong_ptr_type& other) const {
			return (handle_ != other.handle_);
		}
	};
}

#endif // __CCC_REF_PTR_H__