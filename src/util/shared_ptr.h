////////////////////////////////////////////////////////////////////////////////////
// 
// @file: shared_ptr.h
// @author: by Mr.Chen
// @date: 2014/10/10/ 17:10
// @brief: ÷«ƒ‹÷∏’ÎSharePtr, ref from OGRE.
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef _SHAREDPTR_H
#define _SHAREDPTR_H

#include "myassert.h"
#include "lock.h"
#include <iostream>
using namespace std;

namespace sys_util 
{

#define OGRE_AUTO_SHARED_MUTEX mutable MLock *OGRE_AUTO_MUTEX_NAME;
#define OGRE_SET_AUTO_SHARED_MUTEX_NULL OGRE_AUTO_MUTEX_NAME = 0;
#define OGRE_NEW_AUTO_SHARED_MUTEX MyAssert(!OGRE_AUTO_MUTEX_NAME); OGRE_AUTO_MUTEX_NAME = new MLock();
#define OGRE_LOCK_MUTEX(name) AutoMLock ogrenameLock(name);
#define OGRE_COPY_AUTO_SHARED_MUTEX(from) MyAssert(!OGRE_AUTO_MUTEX_NAME); OGRE_AUTO_MUTEX_NAME = from;
#define OGRE_LOCK_AUTO_SHARED_MUTEX MyAssert(OGRE_AUTO_MUTEX_NAME); AutoMLock ogreAutoMutexLock(*OGRE_AUTO_MUTEX_NAME);
#define OGRE_MUTEX_CONDITIONAL(mutex) if (mutex)
#define OGRE_DELETE_AUTO_SHARED_MUTEX MyAssert(OGRE_AUTO_MUTEX_NAME); delete OGRE_AUTO_MUTEX_NAME;
	/** Reference-counted shared pointer, used for objects where implicit destruction is 
        required. 
    @remarks
        This is a standard shared pointer implementation which uses a reference 
        count to work out when to delete the object. 
	@par
		If OGRE_THREAD_SUPPORT is defined to be 1, use of this class is thread-safe.
    */
	template<class T> 
	class SharedPtr
	{
	protected:
		T* _pRep;
		unsigned int* _pUseCount;

	public:
		OGRE_AUTO_SHARED_MUTEX // public to allow external locking
		/** Constructor, does not initialise the SharedPtr.
			@remarks
				<b>Dangerous!</b> You have to call bind() before using the SharedPtr.
		*/
		SharedPtr() : _pRep(0), _pUseCount(0)
        {
            OGRE_SET_AUTO_SHARED_MUTEX_NULL
        }

		/** Constructor.
		@param rep The pointer to take ownership of
		@param freeMode The mechanism to use to free the pointer
		*/
        template< class Y>
		explicit SharedPtr(Y* rep) 
			: _pRep(rep)
			, _pUseCount(rep ?  new unsigned int(1) : 0)
		{
            OGRE_SET_AUTO_SHARED_MUTEX_NULL
			if (rep)
			{
				OGRE_NEW_AUTO_SHARED_MUTEX
			}
		}
		SharedPtr(const SharedPtr& r)
            : _pRep(0), _pUseCount(0)
		{
			// lock & copy other mutex pointer
            
            OGRE_SET_AUTO_SHARED_MUTEX_NULL
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    _pRep = r._pRep;
			    _pUseCount = r._pUseCount;
			    // Handle zero pointer gracefully to manage STL containers
			    if (_pUseCount)
			    {
				    ++(*_pUseCount); 
			    }
            }
		}
		SharedPtr& operator=(const SharedPtr& r) 
		{
			if (_pRep == r._pRep)
			{
				return *this;
			}
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}
		
		template< class Y>
		SharedPtr(const SharedPtr<Y>& r)
            : _pRep(0), _pUseCount(0)
		{
			// lock & copy other mutex pointer

            OGRE_SET_AUTO_SHARED_MUTEX_NULL
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    _pRep = r.getPointer();
			    _pUseCount = r.useCountPointer();
			    // Handle zero pointer gracefully to manage STL containers
			    if (_pUseCount)
			    {
				    ++(*_pUseCount);
			    }
            }
		}
		template< class Y>
		SharedPtr& operator=(const SharedPtr<Y>& r) 
		{
			if (_pRep == r.getPointer())
			{
				return *this;
			}
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}
		virtual ~SharedPtr() 
		{
            release();
		}

		inline T& operator*() const 
		{ 
			MyAssert(_pRep); 
			return *_pRep; 
		}
		inline T* operator->() const 
		{ 
			MyAssert(_pRep); 
			return _pRep; 
		}
		inline T* get() const { return _pRep; }

		/** Binds rep to the SharedPtr.
			@remarks
				Assumes that the SharedPtr is uninitialised!
		*/
		void bind(T* rep) 
		{
			MyAssert(!_pRep && !_pUseCount);
            OGRE_NEW_AUTO_SHARED_MUTEX
			OGRE_LOCK_AUTO_SHARED_MUTEX
			_pUseCount = new unsigned int (1);
			_pRep = rep;
		}

		inline bool unique() const 
		{ 
			OGRE_LOCK_AUTO_SHARED_MUTEX 
			MyAssert(_pUseCount); 
			return *_pUseCount == 1; 
		}
		inline unsigned int useCount() const 
		{ 
			OGRE_LOCK_AUTO_SHARED_MUTEX 
			MyAssert(_pUseCount); 
			return *_pUseCount; 
		}
		inline unsigned int* useCountPointer() const { return _pUseCount; }

		inline T* getPointer() const { return _pRep; }
		inline bool isNull(void) const { return _pRep == 0; }
        inline void setNull(void) 
		{ 
			if (_pRep)
			{
				// can't scope lock mutex before release in case deleted
				release();
				_pRep = 0;
				_pUseCount = 0;
			}
        }

    protected:
        inline void release(void)
        {
			bool destroyThis = false;
            /* If the mutex is not initialized to a non-zero value, then
               neither is pUseCount nor pRep.
             */
            OGRE_MUTEX_CONDITIONAL(OGRE_AUTO_MUTEX_NAME)
			{
				// lock own mutex in limited scope (must unlock before destroy)
				OGRE_LOCK_AUTO_SHARED_MUTEX
				if (_pUseCount)
				{
					if (--(*_pUseCount) == 0) 
					{
						destroyThis = true;
	                }
				}
            }
			if (destroyThis)
			{
				destroy();
			}
            OGRE_SET_AUTO_SHARED_MUTEX_NULL
        }

        virtual void destroy(void)
        {
            // IF YOU GET A CRASH HERE, YOU FORGOT TO FREE UP POINTERS
            // BEFORE SHUTTING OGRE DOWN
            // Use setNull() before shutdown or make sure your pointer goes
            // out of scope before OGRE shuts down to avoid this.
			delete _pRep;
			
			// use OGRE_FREE instead of OGRE_DELETE_T since 'unsigned int' isn't a destructor
			// we only used OGRE_NEW_T to be able to use constructor
			delete _pUseCount;
			OGRE_DELETE_AUTO_SHARED_MUTEX
        }

		virtual void swap(SharedPtr<T> &other) 
		{
			std::swap(_pRep, other._pRep);
			std::swap(_pUseCount, other._pUseCount);
#if OGRE_THREAD_SUPPORT
			std::swap(OGRE_AUTO_MUTEX_NAME, other.OGRE_AUTO_MUTEX_NAME);
#endif
		}
	};

	template<class T, class U> inline bool operator==(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() != b.get();
	}

	template<class T, class U> inline bool operator<(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return std::less<const void*>()(a.get(), b.get());
	}
	/** @} */
	/** @} */

} //namespace sys_util


#endif //_SHAREDPTR_H

