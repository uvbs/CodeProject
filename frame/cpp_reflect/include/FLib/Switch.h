/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef FLIB_SWITCH_H
#define FLIB_SWITCH_H

#include "If.h"
#include "TypeList.h"

#define CASE(COND,THEN)\
	typename FLib::If<COND,THEN,FLib::NullType>::Result


namespace FLib
{
	
	namespace Aux
	{

		template <class TList>
		struct GetFirstNotNull;

		template <class T, class Tail>
		struct GetFirstNotNull<TypeList<T, Tail> >
		{
			typedef T Result;	
			enum { result = true };
		};

		template <class Tail>
		struct GetFirstNotNull<TypeList<NullType, Tail> >
		{
			typedef typename GetFirstNotNull<Tail>::Result Result;
		};

		template <>
		struct GetFirstNotNull<NullType>
		{
			enum { result = true }; // if every type is a NullType
			typedef NullType Result;
		};

	} // namespace Aux
	
	// CaseList -> the last one is the default case.
	template <class CaseList>
	struct Switch
	{
		typedef typename Aux::GetFirstNotNull<CaseList>::Result Result;
	};

} // namespace FLib

#endif

