/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_METHOD_IMPL_H
#define REFLECT_METHOD_IMPL_H

#include "Method.h"
#include "ObjHolder.h"

/*	Constrains:
 *	1. No method overloading;
 *	2. Just pointer.
 */

#include "FLib/TypeTraits.h"

namespace Reflect 
{

	template <typename ClassT, typename RetType>
	class MethodImpl0 : public Method
	{
	public:
		typedef RetType (ClassT::* FunType)();

		MethodImpl0(const std::string& name, FunType funp)
			:	Method(name), _funp(funp)
		{
		
		}

		ObjHolder* invoke(ObjHolder* obj, const std::vector<ObjHolder*>& params) const
		{
			assert(params.size() == 0);
			assert(obj->type() == TypeOf<ClassT>::get());
			return ObjHolder::create((obj->get<ClassT>().*_funp)());
		}

	private:
		FunType _funp;
	};

	template <typename ClassT>
	class MethodImpl0<ClassT, void> : public Method
	{
		typedef void (ClassT::* FunType)();
	public:
		MethodImpl0(const std::string& name, FunType funp)
			:	Method(name), _funp(funp)
		{
		
		}

		ObjHolder* invoke(ObjHolder* obj, const std::vector<ObjHolder*>& params) const
		{
			assert(params.size() == 0);
			assert(obj->type() == TypeOf<ClassT>::get());
			(obj->get<ClassT>().*_funp)();
			return 0;
		}

	private:
		FunType _funp;
	};

	template <typename ClassT, typename RetType, typename Par1Type>
	class MethodImpl1 : public Method
	{
		typedef RetType (ClassT::* FunType)(Par1Type);
	public:
		MethodImpl1(const std::string& name, FunType funp)
			:	Method(name), _funp(funp)
		{
		
		}

		ObjHolder* invoke(ObjHolder* obj, const std::vector<ObjHolder*>& params) const
		{
			typedef typename FLib::RemovePointer<Par1Type>::Result Par1;
			assert(params.size() == 1);
			assert(obj->type() == TypeOf<ClassT>::get());
			assert(params[0]->type() == TypeOf<Par1>::get());
			return ObjHolder::create((obj->get<ClassT>().*_funp)(&(params[0]->get<Par1>())));
		}

	private:
		FunType _funp;
	};

	template <typename ClassT, typename Par1Type>
	class MethodImpl1<ClassT, void, Par1Type> : public Method
	{
		typedef void (ClassT::* FunType)(Par1Type);
	public:
		MethodImpl1(const std::string& name, FunType funp)
			:	Method(name), _funp(funp)
		{
		
		}

		ObjHolder* invoke(ObjHolder* obj, const std::vector<ObjHolder*>& params) const
		{
			typedef typename FLib::RemovePointer<Par1Type>::Result Par1;
			assert(params.size() == 1);
			assert(obj->type() == TypeOf<ClassT>::get());
			assert(params[0]->type() == TypeOf<Par1>::get());
			(obj->get<ClassT>().*_funp)(&(params[0]->get<Par1>()));
			return 0;
		}

	private:
		FunType _funp;
	};

	template <typename ClassT, typename RetType, typename Par1Type, typename Par2Type>
	class MethodImpl2 : public Method
	{
		typedef RetType (ClassT::* FunType)(Par1Type, Par2Type);
	public:
		MethodImpl2(const std::string& name, FunType funp)
			:	Method(name), _funp(funp)
		{
		
		}

		ObjHolder* invoke(ObjHolder* obj, const std::vector<ObjHolder*>& params) const
		{
			typedef typename FLib::RemovePointer<Par1Type>::Result Par1;
			typedef typename FLib::RemovePointer<Par2Type>::Result Par2;
			assert(params.size() == 2);
			assert(obj->type() == TypeOf<ClassT>::get());
			assert(params[0]->type() == TypeOf<Par1>::get());
			assert(params[1]->type() == TypeOf<Par2>::get());
			return ObjHolder::create((obj->get<ClassT>().*_funp)(
								&(params[0]->get<Par1>()), 
								&(params[1]->get<Par2>())));
		}

	private:
		FunType _funp;
	};

	template <typename ClassT, typename Par1Type, typename Par2Type>
	class MethodImpl2<ClassT, void, Par1Type, Par2Type> : public Method
	{
		typedef void (ClassT::* FunType)(Par1Type, Par2Type);
	public:
		MethodImpl2(const std::string& name, FunType funp)
			:	Method(name), _funp(funp)
		{
		
		}

		ObjHolder* invoke(ObjHolder* obj, const std::vector<ObjHolder*>& params) const
		{
			typedef typename FLib::RemovePointer<Par1Type>::Result Par1;
			typedef typename FLib::RemovePointer<Par2Type>::Result Par2;
			assert(params.size() == 2);
			assert(obj->type() == TypeOf<ClassT>::get());
			assert(params[0]->type() == TypeOf<Par1>::get());
			assert(params[1]->type() == TypeOf<Par2>::get());
			(obj->get<ClassT>().*_funp)(
						&(params[0]->get<Par1>()), 
						&(params[1]->get<Par2>()));
			return 0;
		}

	private:
		FunType _funp;
	};

} // namespace Reflect

#endif

