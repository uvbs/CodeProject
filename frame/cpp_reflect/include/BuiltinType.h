/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_BUILTIN_TYPE_H
#define REFLECT_BUILTIN_TYPE_H

#include "Type.h"

namespace Reflect
{

	class BuiltinType : public Type
	{
	protected:
		BuiltinType(const std::string& name);
	public:
		bool isBuiltin() const;
		bool isClass() const;
		bool isPointer() const;
	};
	
} // namespace Reflect

#endif
