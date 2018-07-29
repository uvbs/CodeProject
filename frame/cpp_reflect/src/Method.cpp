/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#include <Method.h>

namespace Reflect 
{

	Method::Method(const std::string& name)
		:	_name(name)
	{
	
	}

	const std::string& Method::name() const
	{
		return _name;
	}

} // namespace Reflect 

