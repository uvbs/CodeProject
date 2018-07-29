/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_MEMBDATAPTR_H
#define REFLECT_MEMBDATAPTR_H

#include "FLib/TypeTraits.h"
#include "FLib/If.h"

namespace Reflect
{
	
	template <typename ClassT, typename FieldT>
	class MembDataPtr
	{
	public:
		MembDataPtr(FieldT ClassT::* mptr)
			:	_mptr(mptr)
		{
		
		}

		FieldT& get(ClassT& obj) const
		{
			return obj.*_mptr;
		}

		void set(ClassT& obj, const FieldT& val) const
		{
			obj.*_mptr = val;
		}

	private:
		FieldT ClassT::* _mptr;
	};

} // namespace Reflect

#endif
