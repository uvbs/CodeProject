/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_CLASS_TYPE_H
#define REFLECT_CLASS_TYPE_H

#include "Type.h"

namespace Reflect 
{
	
	class Field;
	class Method;

	class ClassType;
	class UpCaster;

	class BaseClass
	{
	public:
		BaseClass(const ClassType& baseClass, const UpCaster* upCaster)
			:	_baseClass(&baseClass), _upCaster(upCaster)
		{
		
		}

		BaseClass(const BaseClass& bc)
			:	_baseClass(bc._baseClass), _upCaster(bc._upCaster)
		{
		
		}

		BaseClass& operator = (const BaseClass& bc)
		{
			if (this != &bc)
			{
				_baseClass = bc._baseClass;
				_upCaster = bc._upCaster;
			}
			return *this;
		}

		const ClassType& baseClass() const
		{
			return *_baseClass;
		}

		const UpCaster& upCaster() const
		{
			return *_upCaster;
		}

	private:
		const ClassType* _baseClass;
		const UpCaster* _upCaster;
	};

	class ClassType : public Type
	{
	protected:
		ClassType(const std::string& name);

	public:
		bool isBuiltin() const;
		bool isClass() const;
		bool isPointer() const;

		// Fields Iterator
		typedef std::vector<const Field*>::const_iterator FieldsIterator;
		FieldsIterator fieldsBegin() const;
		FieldsIterator fieldsEnd() const;

		// BaseClasses Iterator
		typedef std::vector<BaseClass>::const_iterator BaseClassesIterator;
		BaseClassesIterator baseClassesBegin() const;
		BaseClassesIterator baseClassesEnd() const;

		const Field* findField(const std::string& name) const;
		const Method* findMethod(const std::string& name) const;

		int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry* objReg) const;
		void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry* objReg) const;
		void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry* objReg) const;

	protected:
		void add(const Field* field);
		void add(const Method* method);
		void add(const BaseClass& baseClass);

	private:
		std::vector<const Field*> _fields;
		std::vector<const Method*> _methods;
		std::vector<BaseClass> _baseClasses;
	};

} // namespace Reflect 

#endif
