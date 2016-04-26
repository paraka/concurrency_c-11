#include <future>
#include <vector>
#include <string>
#include <iostream>
#include "ConcurrentQueue.h"
#include "Monitor.h"

using FutureVector = std::vector<std::future<void>>;
using FutureStringVector = std::vector<std::future<std::string>>;

static void monitor_string_test(FutureVector & v)
{
    Monitor<std::string> s("start\n");

    for(int i=0; i<5; ++i)
        v.push_back(std::async([&,i] {  
            s([=](std::string &s) {
                s += std::to_string(i) + " " + std::to_string(i);
                s += "\n";
            });
            s([](std::string &s) {
                std::cout << s;
            });
        }));


    for (auto &f : v) f.wait(); // and join
    std::cout << "Done\n";
}

static void monitor_cout_test(FutureVector & v)
{
    Monitor<std::ostream &>sync_cout{std::cout}; // capture cout

    for(int i=0; i<5; ++i)
        v.push_back(std::async([&,i] {  
            sync_cout([=](std::ostream &cout) {
                cout << std::to_string(i) << " " << std::to_string(i);
                cout << "\n";
            });
            sync_cout([=](std::ostream &cout) {
                cout << "Hi from " << i << std::endl;
            });
        }));


    for (auto &f : v) f.wait(); // and join
    sync_cout([](std::ostream &cout) { cout << "Done\n"; });
}

static void concurrent_queue_test(FutureStringVector &v)
{
    ConcurrentQueue<std::string> s("start\n");

    // Set off the calculations in a worker thread, 
    // storing future return values
    for (int i = 0; i < 5; ++i )
        v.push_back(  
            s.execute([=](std::string &s) {
                s += std::to_string(i) + " " + std::to_string(i);
                s += "\n";
                return std::string("Set: ") + s;
            }));
}

static void show_debug_queue_elements(FutureStringVector &v)
{
    std::for_each(v.begin(), v.end(), [](std::future<std::string>& f)
    {
        std::cout << f.get() << std::endl;
    });
}

int main()
{
    FutureVector v;
    std::cout << "Monitor string test: " << std::endl;
    monitor_string_test(v);
    std::cout << "\nMonitor cout test: " << std::endl;
    monitor_cout_test(v);
    std::cout << "\nConcurrent Queue string test: " << std::endl;
    FutureStringVector v2;
    concurrent_queue_test(v2);
    show_debug_queue_elements(v2);

    return 0;
}
