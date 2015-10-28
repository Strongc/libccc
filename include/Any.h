#ifndef __CCC_ANY_H__
#define __CCC_ANY_H__

#include "Common.h"

namespace ccc {

	namespace internal_ {

		struct ContainerBase {
			inline ContainerBase() {}
			virtual ~ContainerBase() {}

			virtual ContainerBase* clone() = 0;

			CCC_NONCOPYLABLE(ContainerBase)
		};

		template <typename T>
		struct ContainerImpl : public ContainerBase {
			typedef T value_type;

			inline explicit ContainerImpl(const T& val) : val_(val) {}
			virtual ~ContainerImpl() {}

			virtual ContainerBase* clone() {
				ContainerImpl* ptr = new ContainerImpl(val_);
				return ptr;
			}

			value_type val_;
		};
	}

	/**
	 * @class AnyException
	 * @brief ��Any��ȡֵʱ�׳����쳣
	 */
	class AnyException : public std::exception {
	public:
		AnyException(const char* msg) : std::exception(msg) {}
	};

	/**
	 * @class Any��ģ��
	 * @brief ��Ҫ�޸�Any���ܣ���ʵ��AnyT<Container>�����࣬����ContainerΪ�Զ���ContainerBase����ģ��
	 */
	template <template <typename T> class ContainerT>
	class AnyT {
	public:
		AnyT() : p_(0) {}

		template <typename T>
		explicit inline AnyT(const T& val) {
			p_ = new ContainerT<T>(val);
		}

		virtual ~AnyT() {
			if (p_) delete p_;
		}

		template <typename T>
		T* asPtr() throw() {
			typedef ContainerT<T> container_type;

			container_type* p = NULL;

			try {
				p = dynamic_cast<container_type*>(p_);
				//assert(p);
			} catch (std::exception& e) {
				(void)e;
				p = 0;
			}

			if (!p) {
				return 0;
			}

			return &p->val_;
		}

		template <typename T>
		T& as() {
			T* pRet = this->asPtr<T>();

			if (!pRet) {
				throw AnyException("[Any::as] Pointer cast failed.");
			}

			return *pRet;
		}

		AnyT& operator =(const AnyT& other) {
			if (this == &other) {
				return *this;
			}

			if (p_) {
				delete p_;
				p_ = 0;
			}

			internal_::ContainerBase* pOther = other.p_;

			if (pOther) {
				p_ = pOther->clone();
			}

			return *this;
		}

		// ��Զ�����
		bool operator ==(const AnyT& other) const {
			return false;
		}

		bool operator !=(const AnyT& other) const {
			return true;
		}

	protected:
		internal_::ContainerBase* p_;
	};

	/**
	 * @class Any
	 * @brief ������������
	 */
	typedef AnyT<internal_::ContainerImpl> Any;
}

#endif // __CCC_ANY_H__