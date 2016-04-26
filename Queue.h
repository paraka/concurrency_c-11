#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <deque>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue
{
public:
    void push(const T & item)
    {
        std::unique_lock<std::mutex> lock(m_);
        queue_.push_back(item);
        wait_for_not_empty_.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(m_);
        while (queue_.empty()) wait_for_not_empty_.wait(lock);
        T tmp(queue_.front());
        queue_.pop_front();
        return tmp;
    }

private:
    std::deque<T> queue_;
    std::mutex m_;
    std::condition_variable wait_for_not_empty_;
};

#endif // _QUEUE_H_
