/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_STRING_TYPE_H
#define REFLECT_STRING_TYPE_H

#include "BuiltinType.h"

namespace Reflect
{

	template <typename T>
	struct TypeOf;

	class StringType : public BuiltinType
	{
		friend struct TypeOf<std::string>;
			
	protected:
		StringType(const std::string& name);

	public:
		ObjHolder* newInstance() const;

		int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry*) const;
		void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry*) const;
		void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry*) const;
	};
	
} // namespace Reflect

#endif
