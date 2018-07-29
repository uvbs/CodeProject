/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef FLIB_IF_H
#define FLIB_IF_H

namespace FLib
{

	template <bool g, typename Then, typename Else>
	struct If;

	template <typename Then, typename Else>
	struct If<true, Then, Else>
	{
		typedef Then Result;
	};

	template <typename Then, typename Else>
	struct If<false, Then, Else>
	{
		typedef Else Result;
	};

	template <
		bool g, 
		template <class> class Then, 
		template <class> class Else, 
		typename TParam
	> 
	struct If_;

	template <
		template <class> class Then, 
		template <class> class Else, 
		typename TParam
	> 
	struct If_<true, Then, Else, TParam>
	{
		typedef Then<TParam> Result;
	};

	template <
		template <class> class Then, 
		template <class> class Else, 
		typename TParam
	> 
	struct If_<false, Then, Else, TParam>
	{
		typedef Else<TParam> Result;
	};

	struct TrueType {};
	struct FalseType {};

	template <typename Cond, typename Then, typename Else>
	struct IfT;

	template <typename Then, typename Else>
	struct IfT<TrueType, Then, Else>
	{
		typedef Then Result;
	};

	template <typename Then, typename Else>
	struct IfT<FalseType, Then, Else>
	{
		typedef Else Result;
	};

}

#endif

