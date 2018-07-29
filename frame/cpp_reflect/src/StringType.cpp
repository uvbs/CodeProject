/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <StringType.h>
#include <ObjHolder.h>
#include <Serialize.h>
#include <TypeOf.h> // bah... to avoid compiling problem in ObjHolder.h

namespace Reflect
{

	StringType::StringType(const std::string& name)
		:	BuiltinType(name)
	{
		TypeRegistry::get().add(this);
		TypeRegistry::get().add(typeid(std::string), this);
	}

	ObjHolder* StringType::newInstance() const
	{
		return ObjHolder::create(new std::string(""));
	}

	int StringType::size(ObjHolder* obj, Serialization::Aux::ObjectRegistry*) const
	{
		return ((int) obj->get<std::string>().size() + (int) sizeof(int) + 1);
	}

	void StringType::store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry*) const
	{
		Serialization::Aux::storeString(obj->get<std::string>(), dst);
	}

	void StringType::fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry*) const
	{
		obj->get<std::string>() = Serialization::Aux::fetchString(src);
	}
	
} // namespace Reflect

