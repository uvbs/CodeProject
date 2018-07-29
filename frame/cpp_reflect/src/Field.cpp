/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <Field.h>
#include <Type.h>

namespace Reflect 
{

	Field::Field(const std::string& name, const Type& type, const ClassType& fieldOf)
		:	_name(name), _type(type), _fieldOf(fieldOf)
	{
	
	}

	const std::string& Field::name() const
	{
		return _name;
	}

	const Type& Field::type() const
	{
		return _type;	
	}

	const ClassType& Field::fieldOf() const
	{
		return _fieldOf;
	}

	bool Field::hasMetadata(const Type& metadata) const
	{
		for (std::vector<const Type*>::const_iterator it = _metadata.begin(); it != _metadata.end(); ++it)
			if ((*it)->operator == (metadata)) return true;
		return false;
	}

	void Field::addMetadata(const Type& metadata)
	{
		_metadata.push_back(&metadata);
	}

} // namespace Reflect 
