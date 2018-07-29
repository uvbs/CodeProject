#pragma once

#include <sys/time.h>
#include <tr1/functional>
#include <boost/noncopyable.hpp>
#include <event.h>

namespace cloud_ucs {

class EventWatcher : private boost::noncopyable
{
public:
    typedef std::tr1::function<void()> Handler;

    EventWatcher(struct event_base *event_base, const Handler& handler);
    virtual ~EventWatcher();

    bool Init();
    bool Watch(const struct timeval *timeout = NULL);
    void Cancel();
    void Close();

private:
    struct event_base *event_base_;

protected:
    virtual bool DoInit() = 0;
    virtual void DoClose() {}

    struct event event_;
    Handler handler_;
};

class PipedEventWatcher : public EventWatcher
{
public:
    PipedEventWatcher(struct event_base *event_base,
                      const Handler& handler);

    void Notify();

private:
    virtual bool DoInit();
    virtual void DoClose();
    static void HandlerFn(int fd, short which, void *v);

    int pipe_[2];
};

class TimedEventWatcher : public EventWatcher
{
public:
    TimedEventWatcher(struct event_base *event_base,
                      const Handler& handler);

private:
    virtual bool DoInit();
    static void HandlerFn(int fd, short which, void *v);
};

class SignalEventWatcher : public EventWatcher
{
public:
    SignalEventWatcher(int signo,
                       struct event_base *event_base,
                       const Handler& handler);

private:
    virtual bool DoInit();
    static void HandlerFn(int sn, short which, void *v);

    int signo_;
};

} // namespace cloud_ucs
