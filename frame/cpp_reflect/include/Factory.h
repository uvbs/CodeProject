/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_FACTORY_H
#define REFLECT_FACTORY_H

#include "MethodImpl.h"
#include "FieldImpl.h"

#include "FLib/Switch.h"
#include "FLib/TypeTraits.h"

namespace Reflect
{

	namespace Factory 
	{

		template <typename ClassT, typename RetType>
		const Method* createMethod(const std::string& name, RetType (ClassT::* funp)())
		{
			return new MethodImpl0<ClassT, RetType>(name, funp);
		}

		template <typename ClassT, typename RetType, typename Par1Type>
		const Method* createMethod(const std::string& name, RetType (ClassT::* funp)(Par1Type))
		{
			return new MethodImpl1<ClassT, RetType, Par1Type>(name, funp);
		}

		template <typename ClassT, typename RetType, typename Par1Type, typename Par2Type>
		const Method* createMethod(const std::string& name, RetType (ClassT::* funp)(Par1Type, Par2Type))
		{
			return new MethodImpl2<ClassT, RetType, Par1Type, Par2Type>(name, funp);
		}

		namespace Aux
		{
			template <template <typename, typename> class MetaField>
			struct FieldFactory
			{
				template <typename ClassT, typename FieldT>
				static const Field* apply(	const std::string& name, 
											const Type& type, 
											const ClassType& fieldOf, 
											FieldT ClassT::* mptr)
				{
					return new MetaField<ClassT, FieldT>(name, type, fieldOf, mptr);
				}	
			};
		}
			
		template <typename ClassT, typename FieldT>
		const Field* createField(const std::string& name, FieldT ClassT::* mptr)
		{
			typedef CASE(FLib::TypeTraits<FieldT>::isFundamental, Aux::FieldFactory<BuiltinField>) Case0;
			typedef CASE(FLib::TypeTraits<FieldT>::isString, Aux::FieldFactory<StringField>) Case1;
			typedef CASE(FLib::TypeTraits<FieldT>::isPointer, Aux::FieldFactory<PointerField>) Case2;
			typedef Aux::FieldFactory<CompositeField> Default;

			typedef TYPELIST_4(Case0, Case1, Case2, Default) CaseList;

			return FLib::Switch<CaseList>::Result::apply(name, TypeOf<FieldT>::get(), TypeOf<ClassT>::get(), mptr);
		}

	} // namespace Factory

} // namespace Reflect

#endif

