#include "mutex.h"
#include "time_system.h"
#include "condition.h"
#include "myassert.h"

#ifdef _LINUX64
#include<errno.h>
#endif

Condition::Condition()
{
#ifndef _WIN32
   int ret = pthread_cond_init(&mId,0);
   MyAssert(ret == 0);
#else
   m_blocked = 0;
   m_gone = 0;
   m_waiting = 0;
   m_gate = reinterpret_cast<void*>(CreateSemaphore(0, 1, 1, 0));
   m_queue = reinterpret_cast<void*>(CreateSemaphore(0, 0, LONG_MAX, 0));
   m_mutex = reinterpret_cast<void*>(CreateMutex(0, 0, 0));

   if (!m_gate || !m_queue || !m_mutex)
   {
      int res = 0;
      if (m_gate)
      {
         res = CloseHandle(reinterpret_cast<HANDLE>(m_gate));
         MyAssert(res);
      }
      if (m_queue)
      {
         res = CloseHandle(reinterpret_cast<HANDLE>(m_queue));
         MyAssert(res);
      }
      if (m_mutex)
      {
         res = CloseHandle(reinterpret_cast<HANDLE>(m_mutex));
         MyAssert(res);
      }

      MyAssert(0);
   }
#endif
}

Condition::~Condition ()
{
#ifndef _WIN32
   int ret = pthread_cond_destroy(&mId);
   MyAssert(ret != EBUSY);
#else
    int res = 0;
    res = CloseHandle(reinterpret_cast<HANDLE>(m_gate));
    MyAssert(res);
    res = CloseHandle(reinterpret_cast<HANDLE>(m_queue));
    MyAssert(res);
    res = CloseHandle(reinterpret_cast<HANDLE>(m_mutex));
    MyAssert(res);
#endif
}

#if defined(_WIN32)
void
Condition::enterWait ()
{
   int res = 0;
   res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_gate), INFINITE);
   MyAssert(res == WAIT_OBJECT_0);
   ++m_blocked;
   res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_gate), 1, 0);
   MyAssert(res);
}
#endif
void
Condition::wait (Mutex& mutex)
{
#ifndef _WIN32
   int ret = pthread_cond_wait(&mId, mutex.getId());
   MyAssert(ret == 0);
#else
   enterWait();

   // Release the mutex
   mutex.unLock();

   // do wait
   {
      int res = 0;
      res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_queue), INFINITE);
      MyAssert(res == WAIT_OBJECT_0);

      unsigned was_waiting=0;
      unsigned was_gone=0;

      res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_mutex), INFINITE);
      MyAssert(res == WAIT_OBJECT_0);
      was_waiting = m_waiting;
      was_gone = m_gone;
      if (was_waiting != 0)
      {
         if (--m_waiting == 0)
         {
            if (m_blocked != 0)
            {
               res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_gate), 1, 0); // open m_gate
               MyAssert(res);
               was_waiting = 0;
            }
            else if (m_gone != 0)
                m_gone = 0;
         }
      }
      else if (++m_gone == (ULONG_MAX / 2))
      {
         // timeout occured, normalize the m_gone count
         // this may occur if many calls to wait with a timeout are made and
         // no call to notify_* is made
         res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_gate), INFINITE);
         MyAssert(res == WAIT_OBJECT_0);
         m_blocked -= m_gone;
         res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_gate), 1, 0);
         MyAssert(res);
         m_gone = 0;
      }
      res = ReleaseMutex(reinterpret_cast<HANDLE>(m_mutex));
      MyAssert(res);

      if (was_waiting == 1)
      {
         for (/**/ ; was_gone; --was_gone)
         {
            // better now than spurious later
            res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_queue),
                  INFINITE);
            MyAssert(res == WAIT_OBJECT_0);
         }
         res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_gate), 1, 0);
         MyAssert(res);
      }
   }

   // Reacquire the mutex
   mutex.lock();
#endif
}

void
Condition::wait (Mutex* mutex)
{
   wait(*mutex);
}

bool
Condition::wait(Mutex& mutex, 
                unsigned int ms)
{
   if (ms == 0)
   {
      wait(mutex);
      return true;
   }
#ifndef _WIN32
   uint64 expires64 = TimeSystem::getSinglePtr()->getTimeMilliSecs() + ms;
   timespec expiresTS;
   expiresTS.tv_sec = expires64 / 1000;
   expiresTS.tv_nsec = (expires64 % 1000) * 1000000L;

   MyAssert( expiresTS.tv_nsec < 1000000000L );

   int ret = pthread_cond_timedwait(&mId, mutex.getId(), &expiresTS);

   if (ret == EINTR || ret == ETIMEDOUT)
   {
      return false;
   }
   else
   {
      MyAssert( ret == 0 );
      return true;
   }
#else
   enterWait();

   // Release the mutex
   mutex.unLock();

   //  do timed wait
   bool ret = false;
   unsigned int res = 0;

   res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_queue),ms);
   MyAssert(res != WAIT_FAILED && res != WAIT_ABANDONED);
   ret = (res == WAIT_OBJECT_0);

   unsigned was_waiting=0;
   unsigned was_gone=0;

   res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_mutex), INFINITE);
   MyAssert(res == WAIT_OBJECT_0);
   was_waiting = m_waiting;
   was_gone = m_gone;
   if (was_waiting != 0)
   {
      if (!ret) // timeout
      {
         if (m_blocked != 0)
            --m_blocked;
         else
            ++m_gone; // count spurious wakeups
      }
      if (--m_waiting == 0)
      {
         if (m_blocked != 0)
         {
            res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_gate), 1, 0); // open m_gate
            MyAssert(res);
            was_waiting = 0;
         }
         else if (m_gone != 0)
            m_gone = 0;
      }
   }
   else if (++m_gone == (ULONG_MAX / 2))
   {
      // timeout occured, normalize the m_gone count
      // this may occur if many calls to wait with a timeout are made and
      // no call to notify_* is made
      res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_gate), INFINITE);
      MyAssert(res == WAIT_OBJECT_0);
      m_blocked -= m_gone;
      res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_gate), 1, 0);
      MyAssert(res);
      m_gone = 0;
   }
   res = ReleaseMutex(reinterpret_cast<HANDLE>(m_mutex));
   MyAssert(res);

   if (was_waiting == 1)
   {
      for (/**/ ; was_gone; --was_gone)
      {
         // better now than spurious later
         res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_queue), INFINITE);
         MyAssert(res ==  WAIT_OBJECT_0);
      }
      res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_gate), 1, 0);
      MyAssert(res);
   }

   // Reacquire the mutex
   mutex.lock();

   return ret;
#endif
}

bool
Condition::wait (Mutex* mutex, unsigned int ms)
{
   return wait(*mutex, ms);
}

void
Condition::signal ()
{
#ifndef _WIN32
   int ret = pthread_cond_signal(&mId);
   MyAssert(ret == 0);
#else
    unsigned signals = 0;

   int res = 0;
   res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_mutex), INFINITE);
   MyAssert(res == WAIT_OBJECT_0);

   if (m_waiting != 0) // the m_gate is already closed
   {
      if (m_blocked == 0)
      {
         res = ReleaseMutex(reinterpret_cast<HANDLE>(m_mutex));
         MyAssert(res);
         return;
      }

      ++m_waiting;
      --m_blocked;
      signals = 1;
   }
   else
   {
      res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_gate), INFINITE);
      MyAssert(res == WAIT_OBJECT_0);
      if (m_blocked > m_gone)
      {
         if (m_gone != 0)
         {
            m_blocked -= m_gone;
            m_gone = 0;
         }
         signals = m_waiting = 1;
         --m_blocked;
      }
      else
      {
         res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_gate), 1, 0);
         MyAssert(res);
      }
   }

   res = ReleaseMutex(reinterpret_cast<HANDLE>(m_mutex));
   MyAssert(res);

   if (signals)
   {
      res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_queue), signals, 0);
      MyAssert(res);
   }
#endif
}

void
Condition::broadcast()
{
#ifndef _WIN32
   pthread_cond_broadcast(&mId);
#else
   unsigned signals = 0;

   int res = 0;
   res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_mutex), INFINITE);
   MyAssert(res == WAIT_OBJECT_0);

   if (m_waiting != 0) // the m_gate is already closed
   {
      if (m_blocked == 0)
      {
         res = ReleaseMutex(reinterpret_cast<HANDLE>(m_mutex));
         MyAssert(res);
         return;
      }

      m_waiting += (signals = m_blocked);
      m_blocked = 0;
   }
   else
   {
      res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_gate), INFINITE);
      MyAssert(res == WAIT_OBJECT_0);
      if (m_blocked > m_gone)
      {
         if (m_gone != 0)
         {
            m_blocked -= m_gone;
            m_gone = 0;
         }
         signals = m_waiting = m_blocked;
         m_blocked = 0;
      }
      else
      {
         res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_gate), 1, 0);
         MyAssert(res);
      }
   }

   res = ReleaseMutex(reinterpret_cast<HANDLE>(m_mutex));
   MyAssert(res);

   if (signals)
   {
      res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_queue), signals, 0);
      MyAssert(res);
   }
#endif
}

