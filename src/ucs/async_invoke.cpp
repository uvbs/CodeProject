#include <iostream>
#include "async_invoke.h"
#include "event_watcher.h"

namespace cloud_ucs {

using namespace std;

class DelegateTask : public threadpool::Task {
public:
    typedef std::tr1::function<void()> CompleteHandler;
    DelegateTask(std::shared_ptr<IDelegate> delegate, const CompleteHandler& complete_handler)
        : threadpool::Task()
        , delegate_(delegate)
        , complete_handler_(complete_handler)
    {}

    ~DelegateTask() 
    {}

    void operator()()
    {
        (*delegate_)();
        complete_handler_();
    }

private:
    std::shared_ptr<IDelegate> delegate_;
    CompleteHandler              complete_handler_;
};

class DelegateResult {
public:
    DelegateResult(std::shared_ptr<IDelegate> delegate, 
            const DelegateCompleteHandler& complete_handler)
        : delegate_(delegate)
        , complete_handler_(complete_handler)
    {}

    ~DelegateResult()
    {}

    void CompleteHandler()
    {
        complete_handler_(delegate_);
    }

private:
    std::shared_ptr<IDelegate> delegate_;
    DelegateCompleteHandler      complete_handler_;
};

AsyncInvoke::AsyncInvoke()
    : event_base_(NULL)
{
}

AsyncInvoke::~AsyncInvoke()
{
}

bool AsyncInvoke::Init(struct event_base* base, unsigned int thread_num)
{
    if (base == NULL) {
        return false;
    }

    event_base_ = base;

    if (thread_num == 0) {
        thread_num = 1;
    } else if (thread_num > 100) {
        thread_num = 100;
    } 
 
    std::shared_ptr<threadpool::ThreadPool> threadpool(new threadpool::ThreadPool(thread_num));

    return Init(base, threadpool);
}

bool AsyncInvoke::Init(struct event_base* base, std::shared_ptr<threadpool::ThreadPool> threadpool)
{
    if (base == NULL) {
        return false;
    }

    event_base_ = base;

    result_watcher_.reset(new PipedEventWatcher(base, std::tr1::bind(&AsyncInvoke::HandleTask, this)));
    if (!result_watcher_->Init() || !result_watcher_->Watch()) {
        return false;
    }

    threadpool_ = threadpool;
    if (!threadpool_->Init()) {
        return false;
    }

    return true;
}

void AsyncInvoke::HandleTask()
{
    boost::mutex::scoped_lock lock(results_mutex_);

    while (!results_.empty()) {
        std::shared_ptr<DelegateResult> result = results_.front();
        results_.pop();
        result->CompleteHandler();
    }
}

void AsyncInvoke::CompleteHandler(std::shared_ptr<IDelegate> delegate, const DelegateCompleteHandler& handler)
{
    if (handler) {
        boost::mutex::scoped_lock lock(results_mutex_);
        std::shared_ptr<DelegateResult> result(new DelegateResult(delegate, handler));
        results_.push(result);
        if (results_.size() == 1) {
            result_watcher_->Notify();
        }
    } 
}

bool AsyncInvoke::Invoke(std::shared_ptr<IDelegate> delegate, const DelegateCompleteHandler& handler)
{
    std::shared_ptr<DelegateTask> task(
            new DelegateTask(delegate, 
                std::tr1::bind(&AsyncInvoke::CompleteHandler, this, delegate, handler)));
    return threadpool_->AddTask(task);
}

bool AsyncInvoke::Invoke(std::shared_ptr<IDelegate> delegate)
{
    DelegateCompleteHandler handler;
    return Invoke(delegate, handler);
}

} // namespace cloud_ucs
