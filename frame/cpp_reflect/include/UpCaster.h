/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_UPCASTER_H
#define REFLECT_UPCASTER_H

#include <ObjHolder.h>

namespace Reflect 
{

	class UpCaster
	{
	public:
		virtual ObjHolder* apply(ObjHolder* obj) const = 0;
	};

	template <typename Derived, typename Base>
	class UpCasterImpl : public UpCaster
	{
	public:
		ObjHolder* apply(ObjHolder* obj) const
		{
			Derived* myObj = & obj->get<Derived>();
			Base* mySubObj = myObj;
			return ObjHolder::create(mySubObj);
		}
	};

} // namespace Reflect

#endif
