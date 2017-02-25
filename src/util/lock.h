////////////////////////////////////////////////////////////////////////////////////
// 
// @file: lock.h
// @author: by Mr.Chen
// @date: 2014/5/1	11:43
// @brief: 线程锁, ref from BAIDU.
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef  __LOCK_H_
#define  __LOCK_H_

namespace sys_util {

	//自动锁模板类
	template <typename Lock>
	class AutoLock
	{
	public:
		inline AutoLock(Lock &l) : _lock(l) 
		{
			l.lock();
		}
		inline ~AutoLock() 
		{
			_lock.unlock();
		}

	private:
		Lock &_lock;
	};

	#if defined(_WIN32)
	#include <Windows.h>

	class MLock
	{
	public:
		MLock() 
		{
			InitializeCriticalSection(&_lock);
		}
		~MLock() 
		{
			DeleteCriticalSection(&_lock);
		}
		void lock() 
		{
			EnterCriticalSection(&_lock);
		}
		void unlock() 
		{
			LeaveCriticalSection(&_lock);
		}
		int trylock() 
		{ 
			return TryEnterCriticalSection(&_lock); 
		}

	private:
		CRITICAL_SECTION _lock; //共享锁变量
	};

	typedef AutoLock<MLock> AutoMLock;

	#elif defined(_LINUX64)
	#include <sys/time.h>
	#include <pthread.h>

	//互斥对象锁
	class MLock
	{
	public:
		inline MLock() 
		{
			pthread_mutex_init(&_lock, NULL);
		}
		inline ~MLock() 
		{
			pthread_mutex_destroy(&_lock);
		}
		inline int lock() 
		{
			return pthread_mutex_lock(&_lock);
		}
		inline int unlock() 
		{
			return pthread_mutex_unlock(&_lock);
		}
		inline int trylock() 
		{
			return pthread_mutex_trylock(&_lock);
		}

	private:
		pthread_mutex_t _lock;
		friend class MCondition;
	};

	//读写锁
	class RWLock
	{
	public:
		inline RWLock() 
		{
			pthread_rwlock_init(&_lock,NULL);
		}
		inline ~RWLock() 
		{
			pthread_rwlock_destroy(&_lock);
		}
		inline int rdLock() 
		{
			return pthread_rwlock_rdlock(&_lock);
		}
		inline int tryRdLock() 
		{
			return pthread_rwlock_tryrdlock(&_lock);
		}
		inline int rdLock_ms(const struct timespec *ts) 
		{
			return pthread_rwlock_timedrdlock(&_lock, ts);
		}
		inline int wrLock() 
		{
			return pthread_rwlock_wrlock(&_lock);
		}
		inline int tryWrLock() 
		{
			return pthread_rwlock_trywrlock(&_lock);
		}
		inline int unlock() 
		{
			return pthread_rwlock_unlock(&_lock);
		}

	private:
		pthread_rwlock_t _lock;
	};

	//自旋锁
	class SpinLock
	{
	public:
		inline SpinLock() 
		{
			pthread_spin_init(&_lock, PTHREAD_PROCESS_PRIVATE);
		}
		inline ~SpinLock() 
		{
			pthread_spin_destroy(&_lock);
		}
		inline int lock() 
		{
			return pthread_spin_lock(&_lock);
		}
		inline int unlock() {
			return pthread_spin_unlock(&_lock);
		}
		inline int trylock() 
		{
			return pthread_spin_trylock(&_lock);
		}

	private:
		pthread_spinlock_t _lock;
	};

	//条件锁
	class MCondition
	{
	public:
		MCondition(MLock &l) : _lock(l._lock), _waits(0) 
		{
			pthread_cond_init(&_cond, NULL);
		}
		MCondition(pthread_mutex_t &l) : _lock(l), _waits(0) 
		{
			pthread_cond_init(&_cond, NULL);
		}
		~MCondition() 
		{
			pthread_cond_destroy(&_cond);
		}
		inline int wait(const timeval *tv) 
		{
			++ _waits;
			int ret = 0;
			if (tv) 
			{
				timespec ts;
				ts.tv_sec = tv->tv_sec;
				ts.tv_nsec = tv->tv_usec * 1000;
				ret = pthread_cond_timedwait(&_cond, &_lock, &ts);
				--_waits;
				return ret;
			}
			ret = pthread_cond_wait(&_cond, &_lock);
			-- _waits;
			return ret;
		}
		inline void signal() 
		{
			pthread_cond_signal(&_cond);
		}
		inline void signalAll() 
		{
			pthread_cond_broadcast(&_cond);
		}
		inline size_t waits() { return _waits; }

	private:
		pthread_cond_t _cond;
		pthread_mutex_t &_lock;
		size_t _waits;
	};

	typedef AutoLock<MLock> AutoMLock;
	typedef AutoLock<SpinLock> AutoSLock;

	//自动读锁
	class AutoRdLock
	{
	private:
		RWLock &_lock;

	public:
		inline AutoRdLock(RWLock &l): _lock(l) 
		{   
			_lock.rdLock(); 
		}
		inline ~AutoRdLock() 
		{ 
			_lock.unlock(); 
		}
	};

	//自动写锁
	class AutoWrLock
	{
	private:
		RWLock &_lock;

	public:
		inline AutoWrLock(RWLock &l): _lock(l) 
		{   
			_lock.wrLock(); 
		}
		inline ~AutoWrLock() 
		{ 
			_lock.unlock(); 
		}
	};

	#endif	//_LINUX64

} // namespace sys_util
using namespace sys_util;

#endif  //__LOCK_H_
