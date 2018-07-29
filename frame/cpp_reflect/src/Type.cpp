/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <Type.h>
#include <ClassType.h>
#include <cassert>

namespace Reflect 
{
	Type::Type(const std::string& name)
		:	_name(name)
	{

	}

	const std::string& Type::name() const
	{
		return _name;
	}

	bool Type::operator == (const Type& type) const
	{
		// not works with dll, where there are more instances of builtins, or of some same type.
		return (this == &type);
	}

	const ClassType& Type::toClassType() const
	{
		assert(isClass());
		return static_cast<const ClassType&>(*this);
	}

	bool Type::hasMetadata(const Type& metadata) const
	{
		for (std::vector<const Type*>::const_iterator it = _metadata.begin(); it != _metadata.end(); ++it)
			if ((*it)->operator == (metadata)) return true;
		return false;
	}

	void Type::addMetadata(const Type& metadata)
	{
		_metadata.push_back(&metadata);
	}

} // namespace Reflect 
