/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <PointerType.h>
#include <Serialize.h>

namespace Reflect
{

	PointerType::PointerType(const std::string& name, const Type& pointedType)
		:	Type(name), _pointedType(pointedType)
	{
	
	}

	bool PointerType::isBuiltin() const
	{
		return false;
	}

	bool PointerType::isClass() const
	{
		return false;
	}

	bool PointerType::isPointer() const
	{
		return true;
	}

	const Type& PointerType::pointedType() const
	{
		return _pointedType;
	}

	ObjHolder* PointerType::newInstance() const
	{
		return 0;
	}

	int PointerType::size(ObjHolder* obj, Serialization::Aux::ObjectRegistry* objReg) const
	{
		int ret = 0;
		if (obj->getRaw()) {
			ObjHolder* deref = obj->deref();
			int pos = objReg->add(deref);
			if (pos == -1) {
				// -1 => inserito nel registry! (non esisteva)
				const Type& mc = deref->type();
				ret += ((int) mc.name().size() + 1 + (int) sizeof(int)); // typename
				ret += mc.size(deref, objReg); 
			}
			else {
				ret += (7 + 1 + (int) sizeof(int) + (int) sizeof(int));  //__CYCLExxxx
			}
			// delete deref;  no... l'ownership è del registry
		}
		else {
			ret += (6 + 1 + (int) sizeof(int)); // __NULL
		}
		return ret;
	}

	void PointerType::store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry* objReg) const
	{
		if (obj->getRaw()) {
			ObjHolder* deref = obj->deref();
			int pos = objReg->add(deref);
			if (pos == -1) {
				// -1 => inserito nel registry! (non esisteva)
				const Type& mc = deref->type();
				Serialization::Aux::storeString(mc.name(), dst);
				mc.store(deref, dst, objReg);
			}
			else {
				Serialization::Aux::storeString("__CYCLE", dst);
				dst->write(&pos, sizeof(int));
			}
			// delete deref;  no... l'ownership è del registry
		}
		else {
			Serialization::Aux::storeString("__NULL", dst);
		}
	}

} // namespace Reflect
