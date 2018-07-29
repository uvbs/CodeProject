/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_FIELD_H
#define REFLECT_FIELD_H

#include <string>
#include <vector>

namespace Serialization
{
	class OutArchive;
	class InArchive;

	namespace Aux
	{
		class ObjectRegistry;
	}
}

namespace Reflect 
{

	class Type;
	class ClassType;
	class ObjHolder;

	class Field
	{
	public:
		Field(const std::string& name, const Type& type, const ClassType& fieldOf);
		
		const std::string& name() const;
		const Type& type() const;
		const ClassType& fieldOf() const;

		virtual ObjHolder* get(ObjHolder* obj) const = 0;
		virtual void set(ObjHolder* obj, ObjHolder* val) const = 0;

		virtual int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry*) const = 0;
		virtual void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry*) const = 0;
		virtual void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry*) const = 0;

		bool hasMetadata(const Type& metadata) const;

	protected:
		void addMetadata(const Type& metadata);

	private:
		std::string _name;
		const Type& _type;
		const ClassType& _fieldOf;

		std::vector<const Type*> _metadata;
	};

} // namespace Reflect 

#endif
