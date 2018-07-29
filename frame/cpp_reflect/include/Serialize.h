/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef SERIALIZATION_SERIALIZE_H
#define SERIALIZATION_SERIALIZE_H

#include <string>
#include <vector>

namespace Reflect
{
	class ObjHolder;
}

namespace Serialization 
{

	class OutArchive;
	class InArchive;
	
	namespace Aux
	{

		void storeString(const std::string& str, OutArchive* dst);

		const char* fetchString(InArchive* src);

		class ObjectRegistry
		{
		public:
			ObjectRegistry();
			~ObjectRegistry();
			int add(Reflect::ObjHolder* obj);
			Reflect::ObjHolder* get(int pos);
			
			void clear();

		private:
			std::vector<Reflect::ObjHolder*> _registry;
		};

		class ConstPointerRegistry
		{
		public:
			ConstPointerRegistry();
			~ConstPointerRegistry();
			int add(const void* obj);
			const void* get(int pos);
			
			void clear();

		private:
			std::vector<const void*> _registry;
		};

	}

	OutArchive* serialize(Reflect::ObjHolder* obj);

	Reflect::ObjHolder* deserialize(InArchive* src);
	
} // namespace Serialization 

#endif
