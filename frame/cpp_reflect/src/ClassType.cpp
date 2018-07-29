/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <ClassType.h>
#include <Field.h>
#include <Method.h>
#include <UpCaster.h>

namespace Reflect 
{

	ClassType::ClassType(const std::string& name)
		:	Type(name)
	{

	}

	bool ClassType::isBuiltin() const
	{
		return false;
	}

	bool ClassType::isClass() const
	{
		return true;
	}

	bool ClassType::isPointer() const
	{
		return false;
	}

	void ClassType::add(const Field* field)
	{
		_fields.push_back(field);
	}

	void ClassType::add(const Method* method)
	{
		_methods.push_back(method);
	};

	void ClassType::add(const BaseClass& baseClass)
	{
		_baseClasses.push_back(baseClass);
	};

	ClassType::FieldsIterator ClassType::fieldsBegin() const
	{
		return _fields.begin();
	}

	ClassType::FieldsIterator ClassType::fieldsEnd() const
	{
		return _fields.end();
	}

	ClassType::BaseClassesIterator ClassType::baseClassesBegin() const
	{
		return _baseClasses.begin();
	}

	ClassType::BaseClassesIterator ClassType::baseClassesEnd() const
	{
		return _baseClasses.end();
	}

	const Field* ClassType::findField(const std::string& name) const
	{
		for (std::vector<const Field*>::const_iterator it = _fields.begin(); it != _fields.end(); ++it)
		{
			if ((*it)->name() == name) return (*it);
		}
		return 0;
	}

	const Method* ClassType::findMethod(const std::string& name) const
	{
		for (std::vector<const Method*>::const_iterator it = _methods.begin(); it != _methods.end(); ++it)
		{
			if ((*it)->name() == name) return (*it);
		}
		return 0;
	}

	int ClassType::size(ObjHolder* obj, Serialization::Aux::ObjectRegistry* objReg) const
	{
		int ret = 0;
		for (FieldsIterator it = fieldsBegin(); it != fieldsEnd(); ++it)
		{
			ret += (*it)->size(obj, objReg);
		}

		for (std::vector<BaseClass>::const_iterator it = _baseClasses.begin(); it != _baseClasses.end(); ++it)
		{
			ObjHolder* subObj = (*it).upCaster().apply(obj);
			ret += (*it).baseClass().size(subObj, objReg);
			delete subObj;
		}

		return ret;	
	}

	void ClassType::store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry* objReg) const
	{
		for (FieldsIterator it = fieldsBegin(); it != fieldsEnd(); ++it)
		{
			(*it)->store(obj, dst, objReg);
		}

		for (std::vector<BaseClass>::const_iterator it = _baseClasses.begin(); it != _baseClasses.end(); ++it)
		{
			ObjHolder* subObj = (*it).upCaster().apply(obj);
			(*it).baseClass().store(subObj, dst, objReg);
			delete subObj;
		}	
	}

	void ClassType::fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry* objReg) const
	{
		for (FieldsIterator it = fieldsBegin(); it != fieldsEnd(); ++it)
		{
			(*it)->fetch(obj, src, objReg);
		}

		for (std::vector<BaseClass>::const_iterator it = _baseClasses.begin(); it != _baseClasses.end(); ++it)
		{
			ObjHolder* subObj = (*it).upCaster().apply(obj);
			(*it).baseClass().fetch(subObj, src, objReg);
			delete subObj;
		}	
	}

} // namespace Reflect

