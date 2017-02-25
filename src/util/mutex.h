////////////////////////////////////////////////////////////////////////////////////
// 
// @file: mutex.h
// @author: by Mr.Chen
// @date: 2015/4/4	19:33
// @brief: mutex lock, ref from BAIDU and FALSHGET.
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "baselock.h"
#include "condition.h"

#ifdef _WIN32
#include <Windows.h>
#endif
   
   class Mutex
   {
   protected:
#ifndef _WIN32
         mutable  pthread_mutex_t mId;
#else
         CRITICAL_SECTION mId;
#endif
   
   private:
         //  no value sematics, therefore private and not implemented.
         Mutex (const Mutex&);
         Mutex& operator= (const Mutex&);

   public:
         Mutex();
         ~Mutex();

         void lock();
         void unLock();

#ifndef _WIN32
         pthread_mutex_t* getId()
         {
            return &mId;
         }
#endif
   };
   
   class RWMutex
   {
      public:
         RWMutex();
         ~RWMutex();
         
         void unLock();
         void readLock();
         void writeLock();
#if 0
         unsigned int readerCount() const;
         unsigned int pendingWriterCount() const;
#endif

      private:
#ifdef _WIN32
         Mutex mMutex;
         Condition mReadCondition;
         Condition mPendingWriteCondition;
         unsigned int mReaderCount;
         bool mWriterHasLock;
         unsigned int mPendingWriterCount;
#else
         pthread_rwlock_t mId;
#endif
   };
   
   class ReadLockTraits
   {
      public:
         inline static void lock(RWMutex& mtx)
         {
            mtx.readLock();
         }
    
         inline static void unLock(RWMutex& mtx)
         {
            mtx.unLock();
         }
   };
   
   class WriteLockTraits
   {
      public:
         inline static void lock(RWMutex& mtx)
         {
            mtx.writeLock();
         }
    
         inline static void unLock(RWMutex& mtx)
         {
            mtx.unLock();
         }
   };

   class LockTraits
   {
      public:
         inline static void lock(Mutex& mtx)
         {
            mtx.lock();
         }
    
         inline static void unLock(Mutex& mtx)
         {
            mtx.unLock();
         }
   };

   typedef BaseLock<RWMutex, ReadLockTraits> RLock;
   typedef BaseLock<RWMutex, WriteLockTraits> WLock;
   typedef BaseLock<Mutex, LockTraits> Lock;

#endif // _Mutex_h_
