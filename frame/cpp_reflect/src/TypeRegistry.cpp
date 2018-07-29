/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <TypeRegistry.h>
#include <Type.h>

namespace Reflect 
{

	TypeRegistry& TypeRegistry::get()
	{
		static TypeRegistry typeRegistry;
		return typeRegistry;
	}

	TypeRegistry::TypeRegistry()
	{
	
	}

	bool TypeRegistry::add(const Type* type)
	{
		return _types.insert(std::make_pair(type->name(), type)).second;
	}

	bool TypeRegistry::add(const std::type_info& typeInfo, const Type* type)
	{
		return _typeInfoMap.insert(std::make_pair(&typeInfo, type)).second;
	}

	const Type* TypeRegistry::find(const std::string& str) const
	{
		std::map<std::string, const Type*>::const_iterator it = _types.find(str);
		return  (it != _types.end()) ? (*it).second : 0;
	}

	const Type* TypeRegistry::find(const std::type_info& typeInfo) const
	{
		std::map<const std::type_info*, const Type*>::const_iterator it = _typeInfoMap.find(&typeInfo);
		return  (it != _typeInfoMap.end()) ? (*it).second : 0;
	}

	const Type* TypeRegistry::Iterator::operator * () const
	{
		return (*_it).second;
	}

	TypeRegistry::Iterator& TypeRegistry::Iterator::operator ++ ()
	{
		++_it;
		return (*this);
	}

	bool TypeRegistry::Iterator::operator != (const TypeRegistry::Iterator& rhs) const
	{	
		return (_it != rhs._it);
	}

	TypeRegistry::Iterator::Iterator(const std::map<std::string, const Type*>::const_iterator& it)
		:	_it(it)
	{
	
	}

	TypeRegistry::Iterator TypeRegistry::begin() const
	{
		return Iterator(_types.begin());
	}

	TypeRegistry::Iterator TypeRegistry::end() const
	{
		return Iterator(_types.end());
	}

} // namespace Reflect

