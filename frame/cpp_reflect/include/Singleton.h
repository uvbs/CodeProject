/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef REFLECT_SINGLETON_H
#define REFLECT_SINGLETON_H

namespace Reflect 
{

	template <typename T>
	class Singleton
	{
	public:
		static const T& getInstance()
		{
			// static T instance;
					/* 
					 *	To inizialize the singleton I have to call 
					 *	first this function. But in this way i have to
					 *	call getInstance for every types to export them with the Registry.
					 *	Without calling this, the registry will be empty.
					 *	So i can make instance static of Singleton class, and not
					 *	static of the static method getInstance
					 */
			return instance;
		}
	protected:
		Singleton()
		{
		
		}

		static T instance;
	};

	template <typename T>
	T Singleton<T>::instance;

} // namespace Reflect 

#endif
