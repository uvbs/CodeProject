/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_BUILTIN_IMPL_TYPE_H
#define REFLECT_BUILTIN_IMPL_TYPE_H

#include "BuiltinType.h"
#include "ObjHolder.h"
#include "TypeRegistry.h"
		
#include "Archive.h"
#include "Serialize.h"

namespace Reflect
{

	template <typename T>
	struct TypeOf;

	template <typename T>
	class BuiltinTypeImpl :	public BuiltinType
	{
		friend struct TypeOf<T>;
	private:
		BuiltinTypeImpl(const std::string& name)
			:	BuiltinType(name)
		{
			TypeRegistry::get().add(this);
			TypeRegistry::get().add(typeid(T), this);
		}

	public:

		ObjHolder* newInstance() const
		{
			return ObjHolder::create(new T);
		}

		int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry*) const
		{
			return sizeof(T);
		}

		void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry*) const
		{
			dst->write(& (obj->get<T>()), sizeof(T));
		}

		void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry*) const
		{
			obj->get<T>() = * (T*) src->read(sizeof(T));
		}
	};

} // namespace Reflect

#endif
