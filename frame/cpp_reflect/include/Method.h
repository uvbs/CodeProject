/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_METHOD_H
#define REFLECT_METHOD_H

#include <string>
#include <vector>

namespace Reflect 
{

	class ObjHolder;

	class Method
	{
	public:
		Method(const std::string& name);
		const std::string& name() const;

		virtual ObjHolder* invoke(ObjHolder* obj, const std::vector<ObjHolder*>& params) const = 0;

	private:
		std::string _name;
	};

} // namespace Reflect 

#endif
