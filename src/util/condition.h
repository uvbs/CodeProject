////////////////////////////////////////////////////////////////////////////////////
// 
// @file: condition.h
// @author: by Mr.Chen
// @date: 2015/4/4	19:32
// @brief: condition for lock, ref from BAIDU.
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _Condition_h_
#define _Condition_h_

#include "type.h"

class Mutex;   
class Condition
{
      public:
         Condition();
         virtual ~Condition();

         void wait (Mutex& mtx);
         bool wait (Mutex& mutex, unsigned int ms);

         void wait (Mutex* mutex);
         bool wait (Mutex* mutex, unsigned int ms);
         void signal();
         void broadcast();

      private:
         Condition (const Condition&);
         Condition& operator= (const Condition&);

      private:
#ifndef _WIN32
         mutable  pthread_cond_t mId;
#else
         void enterWait ();

         void* m_gate;
         void* m_queue;
         void* m_mutex;
         unsigned m_gone;  // # threads that timed out and never made it to m_queue
         unsigned long m_blocked; // # threads blocked on the condition
         unsigned m_waiting; // # threads no longer waiting for the condition but
         // still waiting to be removed from m_queue
#endif

};
   
#endif // _base_Condition_h_
