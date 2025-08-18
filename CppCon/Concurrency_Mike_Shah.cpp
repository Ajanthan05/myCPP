#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <chrono>
#include <thread>
#include <future>

#include <mutex>
#include <shared_mutex>
#include <syncstream>
using namespace std;

/*
yield
geat_id
sleep_for
sleep_until

data races
starvation 
deadlocks

static local variable are guaranteed by the c++11 standard to only be initialized once

std::call_once

condition_variable

unique_lock: implement moveable mutex ownership mutex
    it more powerfull than lock_guard and scoped_guard
    condition_variable
    all member fun???

    try _lock_for
    try_lock_until
    swap
    release
    
    unltex
    _owns_lock
    operator bool
*/

/*
ASYNCHRONOUS PROGRAMING:-   Another form of concurancy where execution can happen 
independently of the main program flow
*/
int square(int x) {
    return x*x;
}
void T() {
    std::future<int> asyncFun = std::async(&square, 12);
    int res = asyncFun.get();
}

bool bufferedFileLoading() {
    size_t bytesLoaded = 0;
    while(bytesLoaded < 20000) {
        cout << "Loading Files...\n";
        std::this_thread::sleep_for(std::chrono::microseconds(250));
        bytesLoaded += 1000; 
    }
    return true;
}

/*
Thread Sanitizer

*/

void ASYNC() {
    std::future<bool> backgroundthread = std::async(std::launch::async, bufferedFileLoading);

    std::future_status status;

    while(true) {
        cout << "Main thread running...\n";
        std::this_thread::sleep_for(std::chrono::microseconds(250));

        status = backgroundthread.wait_for(std::chrono::milliseconds(1));

        if (status == std::future_status::ready) {
            cout << "Data ready...\n";
            break;
        }
    }  
}

class ThreadSafeCounter
{
public:
    ThreadSafeCounter() = default;
 
    // Multiple threads/readers can read the counter's value at the same time.
    unsigned int get() const
    {
        std::shared_lock lock(mutex_);
        return value_;
    }
 
    // Only one thread/writer can increment/write the counter's value.
    void increment()
    {
        std::unique_lock lock(mutex_);
        ++value_;
    }
 
    // Only one thread/writer can reset/write the counter's value.
    void reset()
    {
        std::unique_lock lock(mutex_);
        value_ = 0;
    }
 
private:
    mutable std::shared_mutex mutex_;
    unsigned int value_{};
};
 



int main()
{
    ThreadSafeCounter counter;
 
    auto increment_and_print = [&counter]()
    {
        for (int i{}; i != 3; ++i)
        {
            counter.increment();
            std::osyncstream(std::cout)
                << std::this_thread::get_id() << ' ' << counter.get() << '\n';
        }
    };
 
    std::thread thread1(increment_and_print);
    std::thread thread2(increment_and_print);
 
    thread1.join();
    thread2.join();
}