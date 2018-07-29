/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_CLASS_TYPE_IMPL_H
#define REFLECT_CLASS_TYPE_IMPL_H

#include "ClassType.h"
#include "ObjHolder.h"
#include "TypeRegistry.h"

namespace Reflect 
{

	template <typename T>
	class ClassTypeTmpl : public ClassType
	{
	public:
		typedef T Type;

	protected:
		ClassTypeTmpl(const std::string& name)
			:	ClassType(name)
		{
			TypeRegistry::get().add(this);
			TypeRegistry::get().add(typeid(T), this);
		}

	public:

		ObjHolder* newInstance() const
		{
			return ObjHolder::create(new T);
		}
	};

	template <typename T>
	class ClassTypeImpl;

} // namespace Reflect 

#endif
