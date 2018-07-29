/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <Serialize.h>
#include <ClassType.h>
#include <ObjHolder.h>
#include <Archive.h>
#include <TypeRegistry.h>

namespace Serialization 
{

	namespace Aux
	{

		void storeString(const std::string& str, OutArchive* dst)
		{
			int strSize = (int) str.size() + 1;
			dst->write(&strSize, sizeof(int));
			dst->write(str.c_str(), strSize);
		}

		const char* fetchString(InArchive* src)
		{
			int strSize = * (int*) src->read(sizeof(int));
			return (const char*) src->read(strSize);
		}

		ObjectRegistry::ObjectRegistry()
		{
		
		}

		ObjectRegistry::~ObjectRegistry()
		{
			for (unsigned i = 0; i < _registry.size(); ++i)
			{
				delete _registry[i];
			}
		}

		int ObjectRegistry::add(Reflect::ObjHolder* obj)
		{
			for (unsigned i = 0; i < _registry.size(); ++i)
			{
				if (_registry[i]->getRaw() == obj->getRaw()) return (int) i;
			}

			_registry.push_back(obj);

			return -1;
		}

		Reflect::ObjHolder* ObjectRegistry::get(int pos)
		{
			return _registry[pos];
		}

		void ObjectRegistry::clear()
		{
			for (unsigned i = 0; i < _registry.size(); ++i)
			{
				delete _registry[i];
			}
			_registry.clear();
		}

		ConstPointerRegistry::ConstPointerRegistry()
		{
		
		}

		ConstPointerRegistry::~ConstPointerRegistry()
		{
		
		}

		int ConstPointerRegistry::add(const void* obj)
		{
			for (unsigned i = 0; i < _registry.size(); ++i)
			{
				if (_registry[i] == obj) return (int) i;
			}

			_registry.push_back(obj);

			return -1;
		}

		const void* ConstPointerRegistry::get(int pos)
		{
			return _registry[pos];
		}
			
		void ConstPointerRegistry::clear()
		{
			_registry.clear();
		}

	}

	OutArchive* serialize(Reflect::ObjHolder* obj)
	{
		const Reflect::Type& type = obj->type();
		int size = ((int) sizeof(int) + (int) type.name().size() + 1);
		Aux::ObjectRegistry objReg;
		objReg.add(obj->clone());
		size += type.size(obj, &objReg);
		objReg.clear(); // release the ObjHolder owned by the registry.
		OutArchive* dst = new OutArchive(size);
		Aux::storeString(type.name(), dst);
		objReg.add(obj->clone());
		type.store(obj, dst, &objReg);
		return dst;	
	}

	Reflect::ObjHolder* deserialize(InArchive* src)
	{
		const Reflect::Type* type = Reflect::TypeRegistry::get().find(Aux::fetchString(src));
		if (type)
		{
			Reflect::ObjHolder* res = type->newInstance();
			Aux::ObjectRegistry objReg;
			objReg.add(res->clone());
			type->fetch(res, src, &objReg);
			return res;
		}
		return 0;
	}

} // namespace Serialization
