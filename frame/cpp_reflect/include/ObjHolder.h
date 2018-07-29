/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_OBJHOLDER_H
#define REFLECT_OBJHOLDER_H

#include "FLib/TypeTraits.h"
#include "FLib/Switch.h"

#include "Type.h"
#include "TypeRegistry.h"

#include <sstream>

namespace Reflect
{

	class ObjHolder
	{
	public:
		virtual ~ObjHolder() {}

		virtual void releaseContent() const = 0;

		virtual const Type& type() const = 0;

		virtual std::string toString() const = 0;

		virtual ObjHolder* deref() const = 0;

		virtual ObjHolder* clone() const = 0;

		template <typename T> 
		static ObjHolder* create(T* obj);
		template <typename T> 
		static ObjHolder* create(T* obj, const Type* type);

		template <typename T>
		T& get() const;

		virtual void* getRaw() const = 0;
	};

	namespace Aux
	{

		struct Print
		{
			template <typename T>
			inline static std::string apply(T* obj)
			{
				std::ostringstream stream;
				stream << *obj;
				return stream.str();
			}
		};

		struct NoPrint
		{
			template <typename T>
			inline static std::string apply(T* obj)
			{	
				return "";
			}
		};

		template <typename T>
		struct ToString;

		template <typename T>
		struct ToString<T*>
		{
			inline static std::string apply(T* obj)
			{
				return FLib::If<
						FLib::TypeTraits<T>::isFundamental || FLib::TypeTraits<T>::isString,
						Print, NoPrint>::Result::apply(obj);
			}
		};

		template <typename T>
		struct ToString<T**>
		{
			inline static std::string apply(T** obj)
			{
				printf("Trace: Perché vuoi stampare un puntatore?");
				return "";
			}
		};

		template <typename T>
		struct Deref;

		template <typename T>
		struct Deref<T*>
		{
			inline static ObjHolder* apply(T* obj)
			{
				printf("Trace: Perché vuoi dereferenziare qualcosa che non è un puntatore?\n");
				return 0;
			}
		};

		template <typename T>
		struct Deref<T**>
		{
			inline static ObjHolder* apply(T** obj)
			{
				const Type* type = TypeRegistry::get().find(typeid(**obj));
				return ObjHolder::create(*obj, type);
			}
		};

		template <typename T>
		struct ReleaseContent;

		template <typename T>
		struct ReleaseContent<T*>
		{	
			inline static void apply(T* obj)
			{
				delete obj;
			}
		};

		template <typename T>
		struct ReleaseContent<T**>
		{
			inline static void apply(T** obj)
			{
				printf("Trace: Non rilasciare qui l'oggetto!\n");
			}
		};
		
	} // namespace Aux

	template <typename T>
	class ObjHolderImpl : public ObjHolder
	{
	public:
		ObjHolderImpl(T* held, const Type* type)
			:	_held(held), _type(type)
		{
			
		}

		const Type& type() const
		{
			return * _type;
		}

		std::string toString() const
		{
			return Aux::ToString<T*>::apply(_held);
		}

		ObjHolder* deref() const
		{
			return Aux::Deref<T*>::apply(_held);
		}

		void releaseContent() const
		{
			Aux::ReleaseContent<T*>::apply(_held);
		}

		T* getContent() const
		{
			return _held;
		}

		void* getRaw() const
		{
			return _held;	
		}

		ObjHolder* clone() const
		{
			return new ObjHolderImpl<T>(_held, _type);
		}

		~ObjHolderImpl() 
		{
		
		}

	private:
		T* _held;
		const Type* _type;
	};

	template <typename T> 
	ObjHolder* ObjHolder::create(T* obj)
	{
		return new ObjHolderImpl<T>(obj, &TypeOf<T>::get());
	}

	template <typename T> 
	ObjHolder* ObjHolder::create(T* obj, const Type* type)
	{
		return new ObjHolderImpl<T>(obj, type);
	}

	template <typename T>
	T& ObjHolder::get() const
	{
		return * static_cast<const ObjHolderImpl<T>*>(this)->getContent();
	}

} // namespace Reflect

#endif

