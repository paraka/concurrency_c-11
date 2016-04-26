#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <mutex>

template <class T>
class Monitor
{
private:
    // no mutable because cannot define a mutable reference
    // so ostream& example does not compile!
    T t_;
    std::mutex m_; 

public:
    Monitor(T t = T{}) : t_{t} { }
    
    // not const because we can't define members mutable
    template <typename F>
    auto operator()(F f) -> decltype(f(t_))
    {
        std::lock_guard<std::mutex>{m_};
        return f(t_);
    }
};

#endif // _MONITOR_H_
