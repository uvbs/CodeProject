/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_FIELD_IMPL_H
#define REFLECT_FIELD_IMPL_H

#include "Field.h"
#include "MembDataPtr.h"
#include "Type.h"
#include "ObjHolder.h"
#include "Archive.h"
#include "Serialize.h"

namespace Reflect
{
	
	template <typename ClassT, typename FieldT>
	class FieldImpl : public Field
	{
	public:
		FieldImpl(	const std::string& name, 
					const Type& type, 
					const ClassType& fieldOf,
					FieldT ClassT::* mptr)	
			:	Field(name, type, fieldOf),
				_mptr(mptr)
		{
		
		}

		ObjHolder* get(ObjHolder* obj) const
		{
			return ObjHolder::create(&_mptr.get(obj->get<ClassT>()));
		}

		void set(ObjHolder* obj, ObjHolder* val) const
		{
			_mptr.set(obj->get<ClassT>(), val->get<FieldT>());
		}

	protected:
		FieldT& _get(ObjHolder* obj) const
		{
			return _mptr.get(obj->get<ClassT>());
		}

		void _set(ObjHolder* obj, const FieldT& val) const
		{
			_mptr.set(obj->get<ClassT>(), val);
		}

	private:
		MembDataPtr<ClassT, FieldT> _mptr;
	};

	template <typename ClassT, typename FieldT>
	class BuiltinField : public FieldImpl<ClassT, FieldT>
	{
	public:
		BuiltinField(	const std::string& name, 
						const Type& type, 
						const ClassType& fieldOf,
						FieldT ClassT::* mptr)	
			:	FieldImpl<ClassT, FieldT>(name, type, fieldOf, mptr)
		{
		
		}

		int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry*) const
		{
			return sizeof(FieldT);
		}

		void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry*) const
		{
			dst->write(&_get(obj), sizeof(FieldT));
		}

		void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry*) const
		{
			_set(obj, * (FieldT*) src->read(sizeof(FieldT)));
		}

	};

	template <typename ClassT, typename FieldT>
	class StringField : public BuiltinField<ClassT, FieldT>
	{
	public:
		StringField(const std::string& name, 
					const Type& type, 
					const ClassType& fieldOf,
					FieldT ClassT::* mptr)	
			:	BuiltinField<ClassT, FieldT>(name, type, fieldOf, mptr)
		{
		
		}

		int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry*) const
		{
			return ((int) _get(obj).size() + (int) sizeof(int) + 1);
		}

		void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry*) const
		{
			Serialization::Aux::storeString(_get(obj), dst);
		}

		void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry*) const
		{
			_set(obj, Serialization::Aux::fetchString(src)); // auotomatic conversion from const char* to std::string
		}

	};

	template <typename ClassT, typename FieldT>
	class CompositeField : public FieldImpl<ClassT, FieldT>
	{
	public:
		CompositeField(	const std::string& name, 
						const Type& type, 
						const ClassType& fieldOf,
						FieldT ClassT::* mptr)	
			:	FieldImpl<ClassT, FieldT>(name, type, fieldOf, mptr)
		{
		
		}

		int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry* objReg) const
		{
			ObjHolder* subObj = get(obj);
			int ret = type().size(subObj, objReg);
			delete subObj;
			return ret;		
		}

		void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry* objReg) const
		{
			ObjHolder* subObj = get(obj);
			type().store(subObj, dst, objReg);
			delete subObj;		
		}

		void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry* objReg) const
		{
			ObjHolder* subObj = get(obj);
			type().fetch(subObj, src, objReg);
			delete subObj;
		}
	};

	template <typename ClassT, typename FieldT>
	class PointerField : public FieldImpl<ClassT, FieldT>
	{
	public:
		PointerField(	const std::string& name, 
						const Type& type, 
						const ClassType& fieldOf,
						FieldT ClassT::* mptr)	
			:	FieldImpl<ClassT, FieldT>(name, type, fieldOf, mptr)
		{
		
		}

		int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry* objReg) const
		{	
			int ret = 0;
			ObjHolder* subObj = get(obj); // ha l'indirizzo di un puntatore
			ret += type().size(subObj, objReg);
			delete subObj;
			return ret;
		}

		void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry* objReg) const
		{
			ObjHolder* subObj = get(obj); // ha l'indirizzo di un puntatore
			type().store(subObj, dst, objReg);
			delete subObj;
		}

		void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry* objReg) const
		{
			ObjHolder* toSet = get(obj);
			type().fetch(toSet, src, objReg);
			delete toSet;
		}
	};

} // namespace Reflect

#endif
