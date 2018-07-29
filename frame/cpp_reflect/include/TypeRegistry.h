/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_TYPE_REGISTRY_H
#define REFLECT_TYPE_REGISTRY_H

#include <string>
#include <map>
#include <vector>
#include <typeinfo>

namespace Reflect 
{
	class Type;
	template <typename T> class ClassTypeTmpl;
	template <typename T> class BuiltinTypeImpl;
	class StringType;

	class TypeRegistry
	{
		template <typename T> friend class ClassTypeTmpl;
		template <typename T> friend class BuiltinTypeImpl;
		friend class StringType;
	public:
		static TypeRegistry& get();
		const Type* find(const std::string& str) const;
		const Type* find(const std::type_info& typeInfo) const;

		class Iterator 
		{
			friend class TypeRegistry;
		public:
			const Type* operator * () const;
			Iterator& operator ++ ();
			bool operator != (const Iterator& rhs) const;

		private:
			Iterator(const std::map<std::string, const Type*>::const_iterator& it);
			std::map<std::string, const Type*>::const_iterator _it;
		};

		Iterator begin() const;
		Iterator end() const;

	private:
		TypeRegistry();
		bool add(const Type* type);
		bool add(const std::type_info& typeInfo, const Type* type);

		std::map<std::string, const Type*> _types;
		std::map<const std::type_info*, const Type*> _typeInfoMap;
	};

} // namespace Reflect 

#endif
