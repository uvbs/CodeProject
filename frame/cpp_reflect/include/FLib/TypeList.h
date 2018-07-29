/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef FLIB_TYPELIST_H
#define FLIB_TYPELIST_H

// loki-lib style TYPELIST_N

#include "NullType.h"

#define TYPELIST_1(T0) FLib::TypeList<T0,FLib::NullType>
#define TYPELIST_2(T0, T1) FLib::TypeList<T0, TYPELIST_1(T1) >
#define TYPELIST_3(T0, T1, T2) FLib::TypeList<T0, TYPELIST_2(T1, T2) >
#define TYPELIST_4(T0, T1, T2, T3) FLib::TypeList<T0, TYPELIST_3(T1, T2, T3) >
#define TYPELIST_5(T0, T1, T2, T3, T4) FLib::TypeList<T0, TYPELIST_4(T1, T2, T3, T4) >
#define TYPELIST_6(T0, T1, T2, T3, T4, T5) FLib::TypeList<T0, TYPELIST_5(T1, T2, T3, T4, T5) >
#define TYPELIST_7(T0, T1, T2, T3, T4, T5, T6) FLib::TypeList<T0, TYPELIST_6(T1, T2, T3, T4, T5, T6) >
#define TYPELIST_8(T0, T1, T2, T3, T4, T5, T6, T7) FLib::TypeList<T0, TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) >

namespace FLib
{	

	template <typename T, typename U>
	struct TypeList
	{
		typedef T Head;
		typedef U Tail;
	};

	template <typename TList>
	struct Length;

	template <typename Head, typename Tail>
	struct Length<TypeList<Head, Tail> >
	{
		enum { result = 1 + Length<Tail>::result };
	};

	template <>
	struct Length<NullType>
	{
		enum { result = 0 };
	};

	template <typename TList, typename T> 
	struct IndexOf;

	template <typename T>
	struct IndexOf<NullType, T>
	{
		enum { result = -1 };
	};

	template <typename T, typename Tail>
	struct IndexOf<TypeList<T, Tail>, T>
	{
		enum { result = 0 };
	};

	template <typename Head, typename Tail, typename T>
	struct IndexOf<TypeList<Head, Tail>, T>
	{
	private:
		enum { temp = IndexOf<Tail, T>::result };
	public:
		enum { result = (temp == -1 ? -1 : 1 + temp) };
	};

	template <typename TList, int index> 
	struct TypeAt;

	template <typename Head, typename Tail>
	struct TypeAt<TypeList<Head, Tail>, 0>
	{
		typedef Head Result;
	};

	template <typename Head, typename Tail, int i>
	struct TypeAt<TypeList<Head, Tail>, i>
	{
		typedef typename TypeAt<Tail, i - 1>::Result Result;
	};

	template <typename TList, typename T> 
	struct Append;

	template <> 
	struct Append<NullType, NullType>
	{
		typedef NullType Result;
	};

	template <typename T> 
	struct Append<NullType, T>
	{
		typedef TYPELIST_1(T) Result;
	};

	template <typename Head, typename Tail>
	struct Append<NullType, TypeList<Head, Tail> >
	{
		typedef TypeList<Head, Tail> Result;
	};

	template <typename Head, typename Tail, typename T>
	struct Append<TypeList<Head, Tail>, T>
	{
		typedef TypeList<Head, typename Append<Tail, T>::Result> Result;
	};
	
	namespace Aux 
	{
	
		template <typename RList, typename TList, int idx, typename T>
		struct ChangeIdxHelper;
	
		template <typename RList, int idx, typename T>
		struct ChangeIdxHelper<RList, NullType, idx, T>
		{
			typedef RList Result;
		};
	
		template <typename RList, typename Head, typename Tail, typename T>
		struct ChangeIdxHelper<RList, TypeList<Head,Tail>, 0, T>
		{
			typedef typename 
				ChangeIdxHelper<
					typename Append<RList,T>::Result, 
					Tail, - 1, T>::Result Result;
		};
	
		template <typename RList, typename Head, typename Tail, int idx, typename T>
		struct ChangeIdxHelper<RList, TypeList<Head,Tail>, idx, T>
		{
			typedef typename 
				ChangeIdxHelper<
					typename Append<RList,Head>::Result,
					Tail, idx - 1, T>::Result Result;
		};
	
	} // namespace Aux
	
	// Crea una nuova lista da TList, sostituendo idx-esimo elemento con T
	template <typename TList, int idx, typename T>
	struct ChangeIdx
	{
		typedef typename Aux::ChangeIdxHelper<NullType, TList, idx, T>::Result Result;
	};
}

#endif

