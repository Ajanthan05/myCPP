#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

#include <chrono>
using namespace std;

/* 
std::try_lock() tries to lock all the locable objects passed in it one by one in given order.
    std::try_lock(m1,m2,.....mn);

On success it returns -1 otherwise it will return 0-based mutex index number which it could not lock
*/

// Frequency protected data
class Token {
public:
    static Token create() {
        return Token();
    }
    // Other members of Token if needed
};

class TokenPool_old {
    std::mutex mtx;
    std::vector<Token> tokens_;

    Token getToken() {
        mtx.lock();
        if (tokens_.empty()) {
            tokens_.push_back(Token::create());
        }
        Token t = std::move(tokens_.back());
        tokens_.pop_back();
        mtx.unlock();
        return t;
    }
};
/*  The code on this slide still has 
a potential bug. What 
happens if Token::create() 
or push_back() throws an 
exception?
 We’ve locked the mutex, but 
the exception aborts 
execution of this function, so 
we never execute the line that 
would have unlocked it.
 We should look for a way to 
follow RAII principles: Every 
“cleanup” action, including 
unlocking mutexes, should be 
done inside a destructor.*/

class TokenPool {
    std::mutex mtx;
    std::vector<Token> tokens_;

public:
    Token getToken() {
        std::lock_guard<std::mutex> lock(mtx);
        if (tokens_.empty()) {
            tokens_.push_back(Token::create());
        }
        Token t = std::move(tokens_.back());
        tokens_.pop_back();
        return t;
    }

    /*  In C++17 and later, lock_guard can be used with 
    CTAD, as shown in numTokensAvailable()*/
    size_t numTokensAvailable() const {
        std::lock_guard lock(mtx);
        return tokens_.size();
    }

    void returnToken(Token&& token) {
        std::lock_guard<std::mutex> lock(mtx);
        tokens_.push_back(std::move(token));
    }
};


void worker(TokenPool& pool, int id) {
    Token t = pool.getToken();
    std::cout << "Thread " << id << " got a token." << std::endl;
    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    pool.returnToken(std::move(t));
    std::cout << "Thread " << id << " returned a token." << std::endl;
}

void Test() {
    TokenPool pool;
    std::vector<std::thread> threads;

    // Launch 5 worker threads
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, std::ref(pool), i);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads finished." << std::endl;
}


/*  lock guard is not moveable. It can be little bit more efficient
It doesn't have empty state

Unique_lock is moveable not copyable*/
unique_lock<mutex> foo(unique_lock<mutex> lk) {
    if (rand()) {
        lk.unlock();  
        std::cout << "Unlocked early!" << std::endl;
    }
   // prematurely clean up
    return lk;        
   // the resource
}
void Test_unique_lock() {
    std::mutex mtx;
    std::unique_lock<std::mutex> lk(mtx); // lock at construction

    lk = foo(std::move(lk));

    if (!lk.owns_lock()) {
        std::cout << "Lock is not owned anymore." << std::endl;
    } else {
        std::cout << "Still owning the lock." << std::endl;
    }
}
/* like
unique_ptr<int> foo(unique_ptr<int> p) {
    if (rand())
        p = nullptr;  // prematurely clean up
    return p;         
}
std::lock_guard	                                std::unique_lock
Simple and very fast	                        Slightly heavier
No manual control (auto lock/unlock only)	    Full manual control (lock, unlock, try_lock)
Not movable	                                    Movable
Can't transfer ownership	                    Can transfer ownership (move into functions or classes)
Use when simple locking is enough	            Use when you need flexibility (e.g., unlock early, deferred lock, timed lock)

std::lock_guard	std::unique_lock
Simple and very fast	Slightly heavier
No manual control (auto lock/unlock only)	Full manual control (lock, unlock, try_lock)
Not movable	Movable
Can't transfer ownership	Can transfer ownership (move into functions or classes)
Use when simple locking is enough	Use when you need flexibility (e.g., unlock early, deferred lock, timed lock)*/

/*  Why was scoped_lock introduced?
If you had multiple mutexes, you had to manually lock them in the right order ➔ risky ➔ deadlock.
std::lock_guard<std::mutex> lock1(mtx1);
std::lock_guard<std::mutex> lock2(mtx2); 
// Maybe another thread locks in reverse => DEADLOCK
🔵 Solution: std::scoped_lock: It locks multiple mutexes together safely, using a deadlock-avoidance algorithm internally.
✅ No need to worry about lock ordering.
✅ No risk of deadlock between m1, m2, m3.
*/
// class TokenPool2 {
//     std::mutex mtx_;
//     std::vector<Token> tokens_;

// public:
// size_t numTokensAvailable() const {
//     std::scoped_lock lk(mtx_);
// return tokens_.size();
// }
// void mergeTokensFrom(TokenPool& rhs) {
//     std::scoped_lock lk(mtx_, rhs.mtx_);
//     tokens_.insert(rhs.tokens_.begin(),
//                    rhs.tokens_.end());
//     rhs.tokens_.clear()
// }
// };
class TokenPool2 {
    std::mutex mtx_;
    std::vector<Token> tokens_;

public:
    // Get number of available tokens (thread-safe)
    size_t numTokensAvailable() const {
        std::scoped_lock lk(mtx_);
        return tokens_.size();
    }

    // Merge tokens from another pool into this one (thread-safe)
    void mergeTokensFrom(TokenPool2& rhs) {
        std::scoped_lock lk(mtx_, rhs.mtx_);
        tokens_.insert(tokens_.end(), rhs.tokens_.begin(), rhs.tokens_.end());
        rhs.tokens_.clear();
    }

    // Other parts (example: getToken) can be added here
};



std::mutex mtx_1, mtx_2;

void taskA() {
    std::lock_guard<std::mutex> lock1(mtx_1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
    std::lock_guard<std::mutex> lock2(mtx_2); // Deadlock risk
    std::cout << "Task A finished" << std::endl;
}

void taskB() {
    std::lock_guard<std::mutex> lock2(mtx_2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
    std::lock_guard<std::mutex> lock1(mtx_1); // Deadlock risk
    std::cout << "Task B finished" << std::endl;
}

void DeadLock() {
    std::thread t1(taskA);
    std::thread t2(taskB);

    t1.join();
    t2.join();
}


//////////////////////
// std::mutex Mtx_1, Mtx_2;

// void task_A() {
//     std::scoped_lock lock(Mtx_1, Mtx_2); // lock both together safely
//     std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
//     std::cout << "Task A finished" << std::endl;
// }

// void task_B() {
//     std::scoped_lock lock(Mtx_1, Mtx_2); // lock both together safely
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     std::cout << "Task B finished" << std::endl;
// }

// void Scoped_lock() {
//     std::thread t1(task_A);
//     std::thread t2(task_B);

//     t1.join();
//     t2.join();
// }


// CPAD hallway track
int main() {
    /*
    thread tb = thread([&](){
        WaitUntilUnBlock();
        cout << "Hello B\n";
    });
    cout << "Hello A\n";
    unblockThreadB();
    tb.join();
    cout << "Hello Again A\n";
    */

    /*  A non solution: Busy wait*/
    // std::atomic<bool> ready = false;
    // thread tb = thread([&](){
    //     while(!ready) {}; // compiler might hoist it out of the loop
    //     cout << "Hello B\n";
    // });
    // cout << "Hello A\n";
    // ready = true;
    // tb.join();
    // cout << "Hello Again A\n";


    // // A real solution : std::mutex
    // std::mutex mtx;
    // mtx.lock();
    // thread tb2 = thread([&](){
    //     mtx.lock(); mtx.unlock();
    //     cout << "Hello B\n";
    // });
    // cout << "Hello A\n";
    // mtx.unlock();
    // tb2.join();
    // cout << "Hello Again A\n";
    return 0;

    
}