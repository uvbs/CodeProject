/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <BuiltinType.h>

namespace Reflect
{

	BuiltinType::BuiltinType(const std::string& name)
		:	Type(name)
	{

	}

	bool BuiltinType::isBuiltin() const
	{
		return true;
	}

	bool BuiltinType::isClass() const
	{
		return false;
	}

	bool BuiltinType::isPointer() const
	{
		return false;
	}

} // namespace Reflect
