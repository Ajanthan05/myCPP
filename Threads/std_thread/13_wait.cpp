#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include <queue>

#include <sstream> //stringstream
#include <condition_variable>
using namespace std;
#include <future>

// Non-Solution : Busy wait
void fun() {
    std::atomic<bool> ready = false;
    std::thread tb =std::thread([&]() {
        while(!ready) {}
        cout << "Hello from B\n";
    });
    cout << "Hello from A\n";
    ready = true;
    tb.join();
    cout << "Hello Again from A\n";
}

// Mutex is mutual execlution mechanism
void fun2() {
    std::mutex mtx;
    mtx.lock();
    std::thread tb =std::thread([&]() {
        mtx.lock(); mtx.unlock();
        cout << "Hello from B\n";
    });
    cout << "Hello from A\n";
    mtx.unlock();
    tb.join();
    cout << "Hello Again from A\n";
}

// Logical Synchronization
void fun3() {
    std::thread tb =std::thread([&]() {
        // waitUntilUnblock();
        cout << "Hello from B\n";
    });
    cout << "Hello from A\n";

    tb.join();
    cout << "Hello Again from A\n";
}

// class TokenPool {
//     std::mutex mtx_;
//     std::vector<Token> tokens_;

//     Token getToken() {
//         mtx_.lock();
//         if(tokens_.empty()) 
//             tokens_.push_back(Token::create());
//         Token t = std::move(tokens_.back());
//         tokens_.pop_back();
//         mtx_.unlock();
//         return t;
//     }
// };

std::mutex mu;
std::condition_variable cond;
deque<int> buffer;
const unsigned int maxBufferSize = 50;

void producer(int val) {
    while (val) {
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker, []() { return buffer.size() < maxBufferSize; });
        buffer.push_back(val);
        cout << "Produced: " << val << "\n";
        val--;
        locker.unlock();
        cond.notify_one();
    }
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mu);
        cond.wait(lock, []() { return buffer.size() > 0; });
        int val = buffer.front();
        buffer.pop_front();
        cout << "Consumed: " << val << "\n";
        lock.unlock();
        cond.notify_one();
    }
}
int main() {

    std::thread p(producer, 100);
    std::thread c(consumer);

    p.join();
    c.join();

    return 0;
}