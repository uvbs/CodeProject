#include "mutex.h"
#include "assert.h"

Mutex::Mutex()
{
#ifndef _WIN32
   int ret = pthread_mutex_init(&mId, 0);
   Assert(ret == 0);
#else
   // Note:  Windows Critical sections are recursive in nature and perhaps
   //        this implementation calls for a non-recursive implementation
   //        (since there also exists a RecursiveMutex class).  The effort
   //        to make this non-recursive just doesn't make sense though. (SLG)
   InitializeCriticalSection(&mId);
#endif
}

Mutex::~Mutex()
{
#ifndef _WIN32
   int ret = pthread_mutex_destroy(&mId);
   Assert(ret == 0);
#else
   DeleteCriticalSection(&mId);
#endif
}

void Mutex::lock()
{
#ifndef _WIN32
   int ret = pthread_mutex_lock(&mId);
   Assert(ret == 0);
#else
   EnterCriticalSection(&mId);
#endif
}

void Mutex::unLock()
{
#ifndef _WIN32
   int ret = pthread_mutex_unlock(&mId);
   Assert(ret == 0);
#else
   LeaveCriticalSection(&mId);
#endif
}

RWMutex::RWMutex()
#ifdef _WIN32
   :mReaderCount(0),
   mWriterHasLock(false),
   mPendingWriterCount(0)
#endif
{
#ifndef _WIN32
   pthread_rwlock_init(&mId, 0);
#endif
}

RWMutex::~RWMutex()
{
#ifndef _WIN32
   pthread_rwlock_destroy(&mId);
#endif
}

void RWMutex::unLock()
{
#ifdef _WIN32

   Lock lock(mMutex);

   // Unlocking a write lock.
   if ( mWriterHasLock )
   {
      Assert( mReaderCount == 0 );
      mWriterHasLock = false;
      // Pending writers have priority. Could potentially starve readers.
      if ( mPendingWriterCount > 0 )
      {
         mPendingWriteCondition.signal();
      }

      // No writer, no pending writers, so all the readers can go.
      else
      {
         mReadCondition.broadcast();
      }
   }

   // Unlocking a read lock.
   else
   {
      Assert( mReaderCount > 0 );
      mReaderCount--;
      if ( mReaderCount == 0 && mPendingWriterCount > 0 )
      {
         mPendingWriteCondition.signal();
      }
   }
#else
   pthread_rwlock_unlock(&mId);
#endif
}

void RWMutex::readLock()
{
#ifdef _WIN32

   Lock lock(mMutex);
   
   while ( mWriterHasLock || mPendingWriterCount > 0 )
   {
      mReadCondition.wait(mMutex);
   }
   
   mReaderCount++;
#else
   pthread_rwlock_rdlock(&mId);
#endif
}

void RWMutex::writeLock()
{
#ifdef _WIN32

   Lock lock(mMutex);
  
   mPendingWriterCount++;
   while ( mWriterHasLock || mReaderCount > 0 )
   {
      mPendingWriteCondition.wait(mMutex);
   }
   
   mPendingWriterCount--;
   mWriterHasLock = true;
#else
   pthread_rwlock_wrlock(&mId);
#endif
}

#if 0
unsigned int RWMutex::readerCount() const
{
   return ( mReaderCount );   
}

unsigned int RWMutex::pendingWriterCount() const
{
   return ( mPendingWriterCount );
}
#endif
