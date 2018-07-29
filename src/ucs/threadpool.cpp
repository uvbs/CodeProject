#include "socketdef.h"
#include "threadpool.h"
#include <tr1/functional>
#include <thread>

namespace cloud_ucs { namespace threadpool {

using namespace std;

ThreadPool::ThreadPool(uint32_t max_threads, 
                       uint32_t max_queue_size,
                       uint32_t idle_interval, 
                       bool immediately)
    : init_(false)
    , quit_(false)
    , max_threads_(max_threads)
    , counter_(0)
    , idle_(0)
    , idle_interval_(idle_interval)
    , queue_size_(0)
    , max_queue_size_(max_queue_size)
    , immediately_(immediately)
    , waits_(max_threads)
    , signals_(0)
{
    pthread_cond_init(&init_cond_, NULL);
    pthread_mutex_init(&init_mutex_, NULL);
}

ThreadPool::ThreadPool(uint32_t max_threads, 
                       uint32_t max_queue_size, 
                       uint32_t idle_interval, 
                       bool immediately,
                       const ThreadInitializer& initializer)
    : init_(false)
    , quit_(false)
    , max_threads_(max_threads)
    , counter_(0)
    , idle_(0)
    , idle_interval_(idle_interval)
    , queue_size_(0)
    , max_queue_size_(max_queue_size)
    , immediately_(immediately)
    , waits_(max_threads)
    , signals_(0)
    , initializer_(initializer)
{
    pthread_cond_init(&init_cond_, NULL);
    pthread_mutex_init(&init_mutex_, NULL);
}

ThreadPool::~ThreadPool()
{
    Destroy();
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
}

void ThreadPool::Destroy()
{
    int status;

    if (pthread_mutex_lock(&mutex_) != 0) {
        return;
    }

    if (counter_ > 0) {
        quit_ = true;
        if (idle_ > 0) {
            status = pthread_cond_broadcast(&cond_);
            if (status != 0) {
                pthread_mutex_unlock(&mutex_);
                return;
            }
        }

        while (counter_ > 0) {
            status = pthread_cond_wait(&cond_, &mutex_);
            if (status != 0) {
                pthread_mutex_unlock(&mutex_);
                return;
            }
        }
    }

    pthread_mutex_unlock(&mutex_);
}

bool ThreadPool::Init()
{
    if (init_) {
        return true;
    }

    int status;

    status = pthread_mutex_init(&mutex_, NULL);
    if (status != 0) {
        return false;
    }

    status = pthread_cond_init(&cond_, NULL);
    if (status != 0) {
        pthread_mutex_destroy(&mutex_);
        return false;
    }

    if (immediately_) {
        for (uint32_t i = 0; i < max_threads_; ++i) {
            std::shared_ptr<std::thread> thread(
                    new std::thread(std::tr1::bind(&ThreadPool::WorkerMain, this)));
            thread->detach();
            counter_++;
        }

        pthread_mutex_lock(&init_mutex_);
        while (signals_ < waits_) {
            pthread_cond_wait(&init_cond_, &init_mutex_);
        }
        pthread_mutex_unlock(&init_mutex_);
    }

    init_ = true;
    return true;
}

bool ThreadPool::AddTask(std::shared_ptr<Task> task)
{
    int status;

    status = pthread_mutex_lock(&mutex_);
    if (status != 0) {
        return false;
    }

    if (max_queue_size_ != 0 && tasks_.size() >= max_queue_size_) {
        pthread_mutex_unlock(&mutex_);
        return false;
    }

    tasks_.push(task);

    if (idle_ > 0) {
        status = pthread_cond_signal(&cond_);
        if (status != 0) {
            pthread_mutex_unlock(&mutex_);
            return false;
        }
    } else if (counter_ < max_threads_) {
        std::shared_ptr<std::thread> thread(
                new std::thread(std::tr1::bind(&ThreadPool::WorkerMain, this)));
        thread->detach();
        counter_++;
    } 
    pthread_mutex_unlock(&mutex_);
    return true;
}

bool ThreadPool::Broadcast(std::shared_ptr<Task> task)
{
    int status;

    status = pthread_mutex_lock(&mutex_);
    if (status != 0) {
        return false;
    }

    for (map<pthread_t, TaskQueue *>::iterator it = mtasks_.begin();
         it != mtasks_.end();
         ++it) {
        it->second->push(task);
    }

    if (idle_ > 0) {
        status = pthread_cond_broadcast(&cond_);
        if (status != 0) {
            pthread_mutex_unlock(&mutex_);
            return false;
        }
    }
    pthread_mutex_unlock(&mutex_);

    return true;
}

void ThreadPool::WorkerMain()
{
    TaskQueue *tq = new TaskQueue();

    pthread_mutex_lock(&mutex_);
    mtasks_[pthread_self()] = tq;
    pthread_mutex_unlock(&mutex_);

    if (initializer_) {
        bool rc = initializer_();

        pthread_mutex_lock(&init_mutex_);
        if (++signals_ == waits_) {
            pthread_cond_signal(&init_cond_);
        }
        pthread_mutex_unlock(&init_mutex_);

        if (!rc) { return; }
    }

    while (1) {
        bool timedout = false;
        int status;

        if (pthread_mutex_lock(&mutex_)) {
            return;
        }

        while (tasks_.empty() && tq->empty()) {
            if (quit_) {
                if (--counter_ == 0) {
                    pthread_cond_broadcast(&cond_);
                }

                pthread_mutex_unlock(&mutex_);
                return;
            }

            //if (timedout) {
            //    counter_--;
            //    pthread_mutex_unlock(&mutex_);
            //    return;
            //}

            if (idle_interval_ > 0) {
                struct timespec timeout;
                timeout.tv_sec = time(NULL) + idle_interval_;
                timeout.tv_nsec = 0;

                idle_++;
                status = pthread_cond_timedwait(&cond_, &mutex_, &timeout);
                idle_--;
            } else {
                idle_++;
                status = pthread_cond_wait(&cond_, &mutex_);
                idle_--;
            }

            if (status == ETIMEDOUT) {
                timedout = true;
                continue;
            } else if (status != 0) {
                counter_--;
                pthread_mutex_unlock(&mutex_);
                return;
            }
        }

        TaskQueue *thisq = &tasks_;
        if (!tq->empty()) {
            thisq = tq;
        }

        std::shared_ptr<Task> task = thisq->front();
        thisq->pop();
        pthread_mutex_unlock(&mutex_);

        if (task != NULL) {
            (*task)();
        }
    }
}

} // namespace threadpool 
} // namespace cloud_ucs
