#ifndef _CONCURRENT_QUEUE_H
#define _CONCURRENT_QUEUE_H

#include <memory>
#include <thread>
#include <future>
#include <algorithm>
#include "Queue.h"

template <typename T>
class ConcurrentQueue
{
    mutable T t_;
    using Functor = std::function<void()>;
    mutable Queue<Functor> queue_;
    bool done_ = false;
    std::thread worker_;

public:
    ConcurrentQueue(T t) 
        : t_{t}
        , worker_{[=](){ while (!this->done_) queue_.pop()();}}
        { }

    ~ConcurrentQueue()
    {
        queue_.push([=]{ this->done_ = true; });
        worker_.join();
    }

    // Avoid blocking using promises, futures and a worker thread
    // If we just return the value the caller would have to block!
    template<typename F>
    auto execute(F f) const -> std::future<decltype(f(t_))>
    {
        auto promise = std::make_shared<std::promise<decltype(f(t_))>>();
        auto return_value = promise->get_future();
        queue_.push([=]()
        { 
            try
            {
                SetValue(*promise, f, t_);
            }
            catch(...)
            { 
                promise->set_exception(std::current_exception()); 
            }
        });

        return return_value;
    }

private:
    // Assign value to promise non trivial return type
    template <typename Ret, typename Ftn, typename U>
    void SetValue(std::promise<Ret>& promise, Ftn& f, U& t)
    {
        promise.set_value(f(t));
    }

    // Assign value to promise trivial return type
    template <typename Ftn, typename U>
    void SetValue(std::promise<void>& promise, Ftn& f, U& t)
    {
        f(t);
        promise.set_value();
    }
};

#endif // _CONCURRENT_QUEUE_H
