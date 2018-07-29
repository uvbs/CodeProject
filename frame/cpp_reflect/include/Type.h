/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_TYPE_H
#define REFLECT_TYPE_H

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

	class ObjHolder;
	class ClassType;

	class Type
	{
	protected:
		Type(const std::string& name);

	public:
		const std::string& name() const;
		bool operator == (const Type& type) const;

		virtual bool isBuiltin() const = 0;
		virtual bool isClass() const = 0;
		virtual bool isPointer() const = 0;

		const ClassType& toClassType() const;

		virtual ObjHolder* newInstance() const = 0;

		virtual int size(ObjHolder* obj, Serialization::Aux::ObjectRegistry*) const = 0;
		virtual void store(ObjHolder* obj, Serialization::OutArchive* dst, Serialization::Aux::ObjectRegistry*) const = 0;
		virtual void fetch(ObjHolder* obj, Serialization::InArchive* src, Serialization::Aux::ObjectRegistry*) const = 0;

		bool hasMetadata(const Type& metadata) const;

	protected:
		void addMetadata(const Type& metadata);

	private:
		std::string _name;
		std::vector<const Type*> _metadata;
	};

} // namespace Reflect

#endif
