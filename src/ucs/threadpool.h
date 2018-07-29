#ifndef CLOUD_UCS_THREAD_POOL_H_
#define CLOUD_UCS_THREAD_POOL_H_

#include "type.h"
#include <stdint.h>
#include <map>
#include <queue>
#include <pthread.h>
#include <tr1/functional>
#include <memory>

namespace cloud_ucs { namespace threadpool {

class Task {
public:
    Task() {}
    virtual ~Task() {}
    virtual void operator()() = 0;
};

class ThreadPool {
public:
    typedef std::tr1::function<bool()> ThreadInitializer;

    ThreadPool(uint32_t max_threads, 
               uint32_t max_queue_size = 0,
               uint32_t idle_interval = 5, 
               bool immediately = false);
    ThreadPool(uint32_t max_threads, 
               uint32_t max_queue_size, 
               uint32_t idle_interval, 
               bool immediately,
               const ThreadInitializer& initializer);
    ~ThreadPool();

    bool Init();
    void Destroy();
    bool AddTask(std::shared_ptr<Task> task);
    bool Broadcast(std::shared_ptr<Task> task);

private:
    void WorkerMain();

private:
    typedef std::queue<std::shared_ptr<Task> > TaskQueue;

    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    TaskQueue tasks_;
    std::map<pthread_t, TaskQueue *> mtasks_;
    bool init_;
    bool quit_;
    uint32_t max_threads_;
    uint32_t counter_;
    int idle_;
    int idle_interval_;
    uint32_t queue_size_;
    uint32_t max_queue_size_;

    bool immediately_;
    uint32_t waits_;
    uint32_t signals_;
    pthread_cond_t init_cond_;
    pthread_mutex_t init_mutex_;
    ThreadInitializer initializer_;
};

} // namespace threadpool
} // namespace cloud_ucs

#endif // CLOUD_UCS_THREAD_POOL_H_
