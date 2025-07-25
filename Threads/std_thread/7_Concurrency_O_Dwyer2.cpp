#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <map>
#include <string>
#include <condition_variable>
#include <chrono>

#include <semaphore>
#include <latch>
#include <barrier>
#include <optional>
#include <shared_mutex>
using namespace std;

class ThreadSafeConfig {
    std::map<std::string, int> settings_;
    mutable std::shared_mutex rw;

public:
    void set(const std::string& name, int val) {
        std::unique_lock<std::shared_mutex> lk(rw);
        std::cout << "[Writer] Setting " << name << " = " << val << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // simulate work
        settings_[name] = val;
        std::cout << "[Writer] Done setting " << name << "\n";
    }

    int get(const std::string& name) const {
        std::shared_lock<std::shared_mutex> lk(rw);
        std::cout << "  [Reader] Reading " << name << "...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // simulate work
        int val = settings_.at(name);
        std::cout << "  [Reader] Got " << name << " = " << val << "\n";
        return val;
    }
};

// Writer thread function
void writer(ThreadSafeConfig& config, std::string name, int value) {
    config.set(name, value);
}

// Reader thread function
void reader(const ThreadSafeConfig& config, std::string name) {
    try {
        config.get(name);
    } catch (const std::out_of_range& e) {
        std::cout << "  [Reader] " << name << " not set yet.\n";
    }
}

void T() {
    ThreadSafeConfig config;

    // Start a writer thread
    std::thread t1(writer, std::ref(config), "volume", 10);

    // Start multiple reader threads (may run while writer is working or after)
    std::vector<std::thread> readers;
    for (int i = 0; i < 5; ++i) {
        readers.emplace_back(reader, std::cref(config), "volume");
    }

    // Start another writer (for conflict testing)
    std::thread t2(writer, std::ref(config), "brightness", 50);

    t1.join();
    t2.join();
    for (auto& r : readers) r.join();
}



/*  C++20 counting_semaphore */
// class AnonymousTokenPool {
//     std::counting_semaphore<256> sem_{100};// here 256 is max 100 initial
//     void getToken() {
//         sem_.acquire();  // may block
//     }

//     void requireToken() {
//         sem_.release();
//     }
// };

// using Sem = std::counting_semaphore<256>;
// struct SemReleaser {
//     bool operator()(Sem *s) const { s->release(); }
// };
// class UniwuePtrTokenPool {
//     Sem sem_{100};
//     using Token = std::unique_ptr<Sem, SemReleaser>;
//     Token borrowToken() {
//         sem_.acquire(); // may bolock
//         return Token(&sem_);
//     }
// };

using Sem = std::counting_semaphore<256>;

struct SemReleaser {
    bool operator()(Sem *s) const { 
        s->release();
        return true;
    }
};

class UniwuePtrTokenPool {
    Sem sem_{100}; // semaphore initialized with 100 tokens

    using Token = std::unique_ptr<Sem, SemReleaser>;

    Token borrowToken() {
        sem_.acquire(); // blocks if no tokens
        return Token(&sem_); // releases token automatically when destroyed
    }
};



/*  C++20 STD::LATCH
    1. A latch is kind of like a semaphore, in that it has an integer counter that
starts positive and counts down towarda zero.
    2. latch.wait() block until the counter reach zero
    3. latch.count_down() decrements the counter.
        if the counter reach zero then this unblocks all the waiters.
    3. latch.arrive_and_wait() decrements and begin waiting.
    4. latch is like once_flag in that there is no way to "reset" its counter.
*/

void LATCH() {
    std::latch myLatch(2);
    std::thread tb = std::thread([&](){
        myLatch.arrive_and_wait();
        cout << "B\n";
    });
    cout << "A\n";
    myLatch.arrive_and_wait();
    tb.join();
    cout << "Again A\n";
}
/*  NOTE:- no synchronization primitives could ever be copyable
eg mutex, condition variable, latch
    we use synchronizatoin to avoid dangling in myLatch=> b destroied before join
before destorying the latch we join them
*/
void LATCH() {
    std::latch myLatch(1); // note
    std::thread tb = std::thread([&](){
        myLatch.wait(); // Note
        cout << "B\n";
    });
    cout << "A\n";
    myLatch.arrive(); // note
    tb.join();
    cout << "Again A\n";
}

/*  C++20 STD::BARRIER<>
    1. A barrier is essentially a resettable latch
    2. barrier.wait() blocks until the counter reach zero as before
    3. barrier.arrive() decrements the counter
        if the counter reach zero then this unblocks all the waiters.
        and begins a new phase with the counter reset to its initial value.
    4. berrier.arrive_and_wait() decrements and begin waiting as before.

    5. Once every one caught up  unblock every one and automatically refresh 
    the barrier ti stop thme on their next trip arround the loop.
*/

void LATCH() {
    // barrier is  a template fun allow u to put completion function
    // Notify cone one else when barrier comes down
    std::barrier b(2, [] { puts("Green flag to Go");}); // CTAD alearts
    std::thread tb = std::thread([&](){
        b.arrive_and_wait();
        cout << "B\n";
    });
    cout << "A\n";
    b.arrive_and_wait();
    tb.join();
    cout << "Again A\n";
}




/*
    Blue /green deployment is an application release model that gradually
    transfers user trafic from a previous version of an app or microservice to a
    nearly identical new release -> both of which are running in production

    The old version can be called the blue environmentwhile new version is 
    called the green environment
*/





int main() {
    

    return 0;
}


// https://www.youtube.com/watch?v=F6Ipn7gCOsY&t=517s&ab_channel=CppCon




// Destroying a Token now automatically returns it to the pool.


/*  C++20 std::latch
* A latch is kind of like a semaphore, in that it has an integer counter that
starts positive and counts down towards zero.
* latch.wait() block until the counter release zero.
* latch.count_down() decrements the counter.
    If the counter reaches zero the this unblock all the writers.
latch.arive_and_wait() decrements and begins waiting.`
Use a std::latch as a one-shot "starting gate" mechanism: "Wait for
everyone to arrive at this point, then unblock everyone simultaneously."
latch is like one_flag in that there is no way to "reset" its counter.
*/

/*  Would you like a multithreaded example that uses this Logger class in action? 
std::promise, std::future*/

// CPAD hallway track