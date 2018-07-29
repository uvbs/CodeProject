#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "event_watcher.h"

namespace cloud_ucs {

EventWatcher::EventWatcher(struct event_base *event_base, 
        const Handler& handler)
    : event_base_(event_base)
    , handler_(handler)
{
    memset(&event_, 0, sizeof(event_));
}

EventWatcher::~EventWatcher() 
{
    Cancel();
}

bool EventWatcher::Init() 
{
    if (!DoInit()) 
    {
        return false;
    }

    event_base_set(event_base_, &event_);
    return true;
}

bool EventWatcher::Watch(const struct timeval *timeout) 
{
    if (event_add(&event_, timeout) != 0) 
    {
        return false;
    }
    return true;
}

void EventWatcher::Cancel() 
{
    if (event_initialized(&event_))
        event_del(&event_);

    memset(&event_, 0, sizeof(event_));
}

void EventWatcher::Close() 
{
    Cancel();
    DoClose();
}

PipedEventWatcher::PipedEventWatcher(struct event_base *event_base,
            const Handler& handler)
    : EventWatcher(event_base, handler)
{
    memset(pipe_, 0, sizeof(pipe_[0] * 2));
}

bool PipedEventWatcher::DoInit() 
{

    if (evutil_socketpair(AF_LOCAL, SOCK_STREAM, 0, pipe_) < 0) 
    {
        goto failed;
    }

    if (evutil_make_socket_nonblocking(pipe_[0]) < 0 ||
        evutil_make_socket_nonblocking(pipe_[1]) < 0) 
    {
        goto failed;
    }

    event_set(&event_, pipe_[1], EV_READ | EV_PERSIST,
              PipedEventWatcher::HandlerFn, this);
    return true;

failed:
    Close();
    return false;
}

void PipedEventWatcher::DoClose() 
{
    if (pipe_[0]) 
    {
        close(pipe_[0]);
        close(pipe_[1]);
        memset(pipe_, 0, sizeof(pipe_[0]) * 2);
    }
}

void PipedEventWatcher::HandlerFn(int fd, short which, void *v) 
{
    PipedEventWatcher *h = (PipedEventWatcher*)v;
    char buf[128];
    ssize_t n = 0;
    if ((n = recv(h->pipe_[1], buf, sizeof(buf), 0)) < 0) 
    {
        printf("Recv notify failed: %d\n", strerror(errno));
    } 
    else if (n == 0) 
    {
        printf("Notify fd closed.");
    } 
    else 
    {
        h->handler_();
    }
}

void PipedEventWatcher::Notify() 
{
    char buf[1] = {};
    if (send(pipe_[0], buf, sizeof(buf), 0) < 0) 
    {
        return;
    }
}

TimedEventWatcher::TimedEventWatcher(struct event_base *event_base,
        const Handler& handler)
    : EventWatcher(event_base, handler)
{
}

bool TimedEventWatcher::DoInit() 
{
    evtimer_set(&event_, TimedEventWatcher::HandlerFn, this);
    return true;
}

void TimedEventWatcher::HandlerFn(int fd, short which, void *v) 
{
    TimedEventWatcher *h = (TimedEventWatcher*)v;
    h->handler_();
}

SignalEventWatcher::SignalEventWatcher(int signo, struct event_base *event_base,
            const Handler& handler)
    : EventWatcher(event_base, handler)
    , signo_(signo)
{
}

bool SignalEventWatcher::DoInit() 
{
    signal_set(&event_, signo_, SignalEventWatcher::HandlerFn, this);
    return true;
}

void SignalEventWatcher::HandlerFn(int sn, short which, void *v)
{
    SignalEventWatcher *h = (SignalEventWatcher*)v;
    h->handler_();
}

} // namespace cloud_ucs
