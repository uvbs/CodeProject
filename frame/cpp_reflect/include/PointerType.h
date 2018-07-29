/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_POINTER_TYPE_H
#define REFLECT_POINTER_TYPE_H

#include <cassert>

#include "Type.h"
#include "TypeRegistry.h"
#include "ObjHolder.h"
#include "Serialize.h"
#include "Archive.h"

namespace Reflect
{

	template <typename T>
	struct TypeOf;

	class PointerType : public Type
	{
	protected:
		PointerType(const std::string& name, const Type& pointedType);

	public:
		bool isBuiltin() const;

		bool isClass() const;

		bool isPointer() const;

		const Type& pointedType() const;

		ObjHolder* newInstance() const;

		int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry* objReg) const;

		void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry* objReg) const;

	private:
		const Type& _pointedType;
	};
	
	template <typename T>
	class PointerTypeImpl : public PointerType
	{
		friend struct TypeOf<T>;
		typedef typename FLib::PointerTraits<T>::Result PointeeType;
	protected:
		PointerTypeImpl(const std::string& name, const Type& pointedType)
			:	PointerType(name, pointedType)
		{
		
		}

	public:
		void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry* objReg) const
		{
			std::string name(Serialization::Aux::fetchString(src));
			if (name == "__NULL") {
				obj->get<PointeeType*>() = 0;
			} 
			else if (name == "__CYCLE") {
				int pos = * static_cast<int*>(src->read(sizeof(int)));
				ObjHolder* pObj = objReg->get(pos);
				obj->get<PointeeType*>() = & (pObj->get<PointeeType>());
				// delete pObj; no... l'ownership è del registry
			}
			else {
				const Type* ty = TypeRegistry::get().find(name);
				assert(ty);
				ObjHolder* pObj = ty->newInstance();
				assert(pObj);
				objReg->add(pObj);
				ty->fetch(pObj, src, objReg);
				obj->get<PointeeType*>() = & (pObj->get<PointeeType>());
				// delete pObj;  no... l'ownership è del registry
			}
		}
	};

} // namespace Reflect

#endif
