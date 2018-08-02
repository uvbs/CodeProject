////////////////////////////////////////////////////////////////////////////////////
// 
// @file: any_obj.h
// @author: by Mr.Chen
// @date: 2014/10/10/ 20:08
// @brief: 任意对象类型
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __ANY_OBJ_H_
#define __ANY_OBJ_H_

#include <typeinfo>
#include "myassert.h"
#include <iostream>
#include <ostream>
#include <string.h>

namespace sys_util {

	class Any 
	{
	public: // constructors

		Any() : _content(0)
		{
		}

		template<typename ValueType>
		explicit Any(const ValueType & value)
			: _content(new holder<ValueType>(value))
		{
		}

		Any(const Any & other)
			: _content(other._content ? other._content->clone() : 0)
		{
		}

		virtual ~Any()
		{
			destroy();
		}

	public: // modifiers

		Any& swap(Any & rhs)
		{
			std::swap(_content, rhs._content);
			return *this;
		}

		template<typename ValueType>
		Any& operator=(const ValueType & rhs)
		{
			Any(rhs).swap(*this);
			return *this;
		}

		Any & operator=(const Any & rhs)
		{
			Any(rhs).swap(*this);
			return *this;
		}

	public: // queries

		bool isEmpty() const
		{
			return !_content;
		}

		const std::type_info& getType() const
		{
			return _content ? _content->getType() : typeid(void);
		}

		inline friend std::ostream& operator <<
			( std::ostream& o, const Any& v )
		{
			if (v._content)
				v._content->writeToStream(o);
			return o;
		}

		void destroy()
		{
			delete _content;
			_content = NULL;
		}

	protected: // types

		class placeholder 
		{
		public: // structors

			virtual ~placeholder()
			{}

		public: // queries

			virtual const std::type_info& getType() const = 0;

			virtual placeholder * clone() const = 0;

			virtual void writeToStream(std::ostream& o) = 0;

		};

		template<typename ValueType>
		class holder : public placeholder
		{
		public: // structors

			holder(const ValueType & value)
				: _held(value)
			{
			}

		public: // queries

			virtual const std::type_info & getType() const
			{
				return typeid(ValueType);
			}

			virtual placeholder * clone() const
			{
				return new holder(_held);
			}

			virtual void writeToStream(std::ostream& o)
			{
				//o << _held;
			}

		public: // representation
			ValueType _held;

		};


	protected: // representation
		placeholder * _content;

		template<typename ValueType>
		friend ValueType * any_cast(Any *);


	public:

		template<typename ValueType>
		ValueType operator()() const
		{
			if (!_content) 
			{
				MyAssert("Bad cast from uninitialised Any");
			}
			else if (getType() == typeid(ValueType))
			{
				return static_cast<Any::holder<ValueType> *>(_content)->_held;
			}
			else
			{
				MyAssert("Bad cast from uninitialised Any");
			}
		}

		template <typename ValueType>
		ValueType get(void) const
		{
			if (!_content) 
			{
				MyAssert("Bad cast from uninitialised Any");
			}
			else if (getType() == typeid(ValueType))
			{
				return static_cast<Any::holder<ValueType> *>(_content)->_held;
			}
			else
			{
				MyAssert("Bad cast from uninitialised Any");
			}
		}

	};


	template<typename ValueType>
	ValueType * any_cast(Any * operand)
	{
		return operand && (::strcmp(operand->getType().name(), typeid(ValueType).name()) == 0)
			? &static_cast<Any::holder<ValueType> *>(operand->_content)->_held
			: 0;
	}

	template<typename ValueType>
	const ValueType * any_cast(const Any * operand)
	{
		return any_cast<ValueType>(const_cast<Any *>(operand));
	}

	template<typename ValueType>
	ValueType any_cast(const Any & operand)
	{
		const ValueType * result = any_cast<ValueType>(&operand);
		if (!result)
		{
			MyAssert("Bad cast from type");
		}
		return *result;
	}

}// namespace sys_util

#endif // __any_obj_h_
