/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_TYPEOF_H
#define REFLECT_TYPEOF_H

#include "BuiltinTypeImpl.h"
#include "ClassTypeImpl.h"
#include "StringType.h"
#include "PointerType.h"

namespace Reflect
{

	template <typename T> 
	struct TypeOf
	{
		static const ClassTypeImpl<T>& get()
		{
			return ClassTypeImpl<T>::getInstance();
		}
	};

	#define TYPEOF_SPECIALIZATION(name) \
		template <> \
		struct TypeOf<name> \
		{ \
			static const BuiltinTypeImpl<name>& get() \
			{ \
				static BuiltinTypeImpl<name> instance(#name); \
				return instance; \
			} \
		}; \
	
	TYPEOF_SPECIALIZATION(bool);
	TYPEOF_SPECIALIZATION(char);
	TYPEOF_SPECIALIZATION(int);
	TYPEOF_SPECIALIZATION(unsigned int);
	TYPEOF_SPECIALIZATION(float);
	TYPEOF_SPECIALIZATION(double);
	TYPEOF_SPECIALIZATION(long double);

	template <>
	struct TypeOf<std::string>
	{
		static const StringType& get()
		{
			static StringType instance("string");
			return instance;
		}
	};

	template <typename T>
	struct TypeOf<T*>
	{
		static const PointerTypeImpl<T*>& get()
		{
			static PointerTypeImpl<T*> instance("pointer", TypeOf<T>::get());
			return instance;
		}
	};

} // namespace Reflect

#endif
