/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef FLIB_TYPETRAITS_H
#define FLIB_TYPETRAITS_H

#include "TypeList.h"
#include "NullType.h"

#include <string>
#include <vector>

namespace FLib
{

	template <typename T>
	struct RemovePointer
	{
		enum { result = false };
		typedef T Result;
	};

	template <typename T>
	struct RemovePointer<T*>
	{
		enum { result = true };
		typedef T Result;
	};

	template <typename T>
	struct PointerTraits
	{
		enum { result = false };
		typedef NullType Result;
	};

	template <typename T>
	struct PointerTraits<T*>
	{
		enum { result = true };
		typedef T Result;
	};

	template <typename T>
	struct StringTraits
	{
		enum { result = false };
	};

	template <>
	struct StringTraits<std::string>
	{
		enum { result = true };
	};

	template <typename T>
	struct VectorTraits
	{
		enum { result = false };
		typedef void Result;
	};

	template <typename T>
	struct VectorTraits<std::vector<T> >
	{
		enum { result = true };
		typedef T Result;
	};


	// loki-lib style TypeTraits
	template <typename T>
	class TypeTraits
	{
	private:
		typedef TYPELIST_1(void) Void;
		typedef TYPELIST_4(bool, char, int, unsigned int) Integrals;
		typedef TYPELIST_3(float, double, long double) Floats;

	public:

		enum { isPointer = PointerTraits<T>::result };
		enum { isString = StringTraits<T>::result };
		enum { isVector = VectorTraits<T>::result };

		typedef typename PointerTraits<T>::Result PointedType;

		enum { isVoid = IndexOf<Void,T>::result >= 0 };
		enum { isIntegral = IndexOf<Integrals,T>::result >= 0 };
		enum { isFloat = IndexOf<Floats,T>::result >= 0 };
		
		enum { isFundamental = isVoid || isIntegral || isFloat };
	};

}

#endif

