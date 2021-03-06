#ifndef CLOUD_UCS_ASYNC_INVOKE_H_
#define CLOUD_UCS_ASYNC_INVOKE_H_

#include <stdio.h>
#include <queue> 
#include <tr1/functional>
//#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <event.h>
#include "threadpool.h"

namespace cloud_ucs {

class DelegateResult;
class PipedEventWatcher;

class IDelegate {
public:
    IDelegate() {}
    virtual ~IDelegate() {}
    virtual void operator()() = 0;
};

typedef std::tr1::function<void(std::shared_ptr<IDelegate>)> DelegateCompleteHandler;

class AsyncInvoke {
public:
    AsyncInvoke();
    ~AsyncInvoke();

    bool Init(struct event_base* base, unsigned int thread_num);
    bool Init(struct event_base* base, std::shared_ptr<threadpool::ThreadPool> threadpool);
    bool Invoke(std::shared_ptr<IDelegate> delegate, const DelegateCompleteHandler& handler);
    bool Invoke(std::shared_ptr<IDelegate> delegate);
    void CompleteHandler(std::shared_ptr<IDelegate> delegate, const DelegateCompleteHandler& handler);

private:
    void HandleTask();

private:
    struct event_base* event_base_;

    std::queue<std::shared_ptr<DelegateResult> > results_;
    boost::mutex results_mutex_;
    boost::scoped_ptr<PipedEventWatcher> result_watcher_;
    std::shared_ptr<threadpool::ThreadPool> threadpool_;
};

} // namespace cloud_ucs

#endif
