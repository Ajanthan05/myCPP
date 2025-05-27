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
#include <optional>
#include <shared_mutex>
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







/*  Important: You can't copy it, move it, or pass it around between functions.
➔ Because that would create multiple owners of the same lock, which would be unsafe.
Because std::lock_guard disables copy and move.

This behavior is intentional:
It forces the lock to be tied to a scope — exactly like RAII.


lock guard is not moveable. It can be little bit more efficient
It doesn't have empty state

Unique_lock is moveable not copyable
You can unlock and lock again manually.
*/
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

unique_lock?
When you want to unlock manually inside a function.

When you want to lock later (not at constructor).

When you want to try_lock with timeout (lock for a limited time).

When you want to move a lock across scopes (e.g., hand a lock to another thread or object).*/



/*  Why was scoped_lock introduced?
If you had multiple mutexes, you had to manually lock them in the right order ➔ risky ➔ deadlock.
std::lock_guard<std::mutex> lock1(mtx1);
std::lock_guard<std::mutex> lock2(mtx2); 
// Maybe another thread locks in reverse => DEADLOCK
🔵 Solution: std::scoped_lock: It locks multiple mutexes together safely, using a deadlock-avoidance algorithm internally.
✅ No need to worry about lock ordering.
✅ No risk of deadlock between m1, m2, m3.


✅ When you need to lock more than one mutex.
✅ When you want deadlock safety automatically.
✅ When you want RAII locking (automatic unlock).

🔵 If you just have one mutex, you can still use it — it's simple and safe.

***Quick memory trick
lock_guard ➔ one lock, simple.
unique_lock ➔ flexible lock, manual control.
scoped_lock ➔ multiple locks, deadlock-safe.*/




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






class TokeN {
public:
    // [Caller] --calls--> [create()] --creates--> [Token] --returns--> [Caller gets Token]
// ✅ You call Token::create().
// ✅ A new Token object is constructed inside the function.
// ✅ It is returned back to you.
    static TokeN create() {
        return TokeN();
    }
    // Other members of TokeN if needed
};

class TokenPool {
    std::mutex mtx;
    std::vector<TokeN> tokens_;

public:
// Locks mtx (with RAII lock_guard) so only one thread can access tokens_ at a time.
// If no tokens available ➔ it creates one and pushes it into tokens_.
// Takes the last token, moves it into t (no copy! efficient).
    TokeN getToken() {
        std::lock_guard<std::mutex> lock(mtx);
        if (tokens_.empty()) {
            tokens_.push_back(TokeN::create());
        }
        TokeN t = std::move(tokens_.back());
        tokens_.pop_back();
        return t;
    }

    /*  In C++17 and later, lock_guard can be used with 
    CTAD, as shown in numTokensAvailable()*/
    size_t numTokensAvailable() const {
        std::lock_guard lock(mtx);
        return tokens_.size();
    }

    void returnToken(TokeN&& token) {
        std::lock_guard<std::mutex> lock(mtx);
        tokens_.push_back(std::move(token));
    }
};


void worker(TokenPool& pool, int id) {
    TokeN t = pool.getToken();
    std::cout << "Thread " << id << " got a TokeN." << std::endl;
    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    pool.returnToken(std::move(t));
    std::cout << "Thread " << id << " returned a TokeN." << std::endl;
}

void Test() {
    TokenPool pool;
    std::vector<std::thread> threads;

    // Launch 5 worker threads
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, std::ref(pool), i); 
        // Passes reference to pool (std::ref) so all threads use same pool.
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads finished." << std::endl;
}


/////////////   MODIFIED TOKEN  ///////////////////////
/*  In multi-threaded programs, multiple threads may call Token::create() at the same time.

Using fetch_add(1) ensures each thread gets a unique ID, without using a mutex.

It’s faster than locking and safe.*/
class Token_MODIFIED {
    static std::atomic<size_t> next_id_;
    size_t id_;
    std::chrono::steady_clock::time_point created_at_;

    Token_MODIFIED(size_t id)
        : id_(id), created_at_(std::chrono::steady_clock::now()) {
    }

public:
    static Token_MODIFIED create() {
        size_t id = next_id_.fetch_add(1);
        return Token_MODIFIED(id);
    }

    void print() const {
        std::cout << "Token_MODIFIED id: " << id_ << "\n";
    }
};

// Initialize static member
std::atomic<size_t> Token_MODIFIED::next_id_{0};


/////////////////   Speed up the system by pre-filling TokenPool with a batch of Tokens using Token::create(). /////////
class TokenPool_preallocation {
    std::mutex mtx;
    std::vector<Token> tokens_;

public:
    // Constructor that pre-allocates N tokens
    TokenPool_preallocation(size_t preallocate = 0) {
        for (size_t i = 0; i < preallocate; ++i) {
            tokens_.push_back(Token::create());
        }
    }

    Token getToken() {
        std::lock_guard<std::mutex> lock(mtx);
        if (tokens_.empty()) {
            tokens_.push_back(Token::create());
        }
        Token t = std::move(tokens_.back());
        tokens_.pop_back();
        return t;
    }

    void returnToken(Token&& token) {
        std::lock_guard<std::mutex> lock(mtx);
        tokens_.push_back(std::move(token));
    }

    size_t numTokensAvailable() const {
        std::scoped_lock lock(mtx);
        return tokens_.size();
    }
};
void batch_preallocation() {
    // Preallocate 10 tokens
    TokenPool_preallocation pool(10);
    std::cout << "Preallocated " << pool.numTokensAvailable() << " tokens.\n";

    std::vector<std::thread> threads;

    // Launch 5 worker threads
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, std::ref(pool), i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Remaining tokens: " << pool.numTokensAvailable() << "\n";
}


/*  We have std::unique_ptr to help us manage unique ownership of heap alocation
unique_lock help to manage unique ownership of mutex locks*/


struct TokenPool_condition_variable {
    std::mutex mtx;
    std::vector<Token> tokens_;
    std::condition_variable cv_;

    void returnToken(Token&& t) {
        std::unique_lock lk(mtx);
        tokens_.push_back(t);
        lk.unlock();
        cv_.notify_one();
    }
/* Remember, every access (read or write) to token
mustt still be done under a mtx_lock, so as to
avoid physical date races(UB)*/

    Token getToken() {
        std::unique_lock lk(mtx);
        while(tokens_.empty()) {
            cv_.wait(lk);
        }
        Token t = std::move(tokens_.back());
        tokens_.pop_back();
        return t;
    }
};

/*  std::defer_lock?
It tells std::unique_lock not to lock the mutex immediately.
You get full control over when to lock and unlock.*/
void Defer_lock() {
    std::mutex mtx;

    // Construct but don’t lock yet
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);

    // Do something first...
    // prepare_data();  

    // Now lock manually
    lock.lock();
    // do_work_safely();

    // Unlock when needed
    lock.unlock();

    // Re-lock if needed
    lock.lock();

}

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void wait_for_work() {
    std::unique_lock<std::mutex> lk(mtx, std::defer_lock);

    // Some work before locking
    // prepare();

    lk.lock();  // Lock only when needed
    cv.wait(lk, [] { return ready; });  // Wait safely
}

// You can lock multiple mutexes safely without deadlocks:
std::mutex m1, m2;
void example() {
    std::unique_lock<std::mutex> lk1(m1, std::defer_lock);
    std::unique_lock<std::mutex> lk2(m2, std::defer_lock);

    std::lock(lk1, lk2);  // Deadlock-safe!
}
/*  | Feature              | `std::lock_guard` | `std::unique_lock` |
| -------------------- | ----------------- | ------------------ |
| Automatic locking    | ✅                 | ✅ (by default)     |
| Can defer locking    | ❌                 | ✅ (`defer_lock`)   |
| Manual lock/unlock   | ❌                 | ✅                  |
| Movable              | ❌                 | ✅                  |
| Works with cond vars | ⚠️ Limited         | ✅                  |
*/

// ✅ 2. Conditionally lock:-   Lock only if some condition is met
void tryLockIfReady(bool should_lock) {
    std::unique_lock<std::mutex> lk(mtx, std::defer_lock);
    
    if (should_lock) {
        lk.lock();
        std::cout << "Locked and processing...\n";
    } else {
        std::cout << "Skipped locking.\n";
    }
}
// ✅ 3. Move lock between functions:- Unlike lock_guard, you can move unique_lock into other functions
void doLockedWork(std::unique_lock<std::mutex> lk) {
    std::cout << "In locked function.\n";
}
void caller() {
    std::mutex m;
    std::unique_lock<std::mutex> lk(m, std::defer_lock);

    // ... prepare work
    lk.lock();

    doLockedWork(std::move(lk));  // Transfer ownership
}

// ✅ 4. Use with std::lock() for multiple mutexes:-   Deadlock-free locking of multiple mutexes
std::mutex m1, m2;
void lockTwo() {
    std::unique_lock<std::mutex> lk1(m1, std::defer_lock);
    std::unique_lock<std::mutex> lk2(m2, std::defer_lock);

    std::lock(lk1, lk2);  // Avoids deadlock

    // Now both are safely locked

    /*  std::lock() — a special function in the C++ Standard Library designed to lock multiple mutexes atomically and safely.
// Thread 1:
lock(m1);
lock(m2);

// Thread 2:
lock(m2);
lock(m1);
If Thread 1 locks m1 and Thread 2 locks m2 at the same time, both will wait forever trying to lock the other → ❌ Deadlock.

✅ The Solution:
std::lock(lk1, lk2);
    1.Uses an internal deadlock-avoidance algorithm.
    2.Attempts to acquire both mutexes without risk of deadlock.
    3.Locks them in a defined consistent order, regardless of how you wrote them.

If it can’t lock both immediately, it releases what it locked and tries again safely.

🔁 std::defer_lock
This is important:
std::unique_lock<std::mutex> lk1(m1, std::defer_lock);
Means:
    1.Don’t lock the mutex now.
    2.Just associate the lock with the mutex.
    3.This lets std::lock() manage the actual locking later.*/
}

// ✅ 5. With std::condition_variable:-    Often used for waiting on a condition
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void waitUntilReady() {
    std::unique_lock<std::mutex> lk(mtx, std::defer_lock);

    // prepare();

    lk.lock();  // lock before waiting
    cv.wait(lk, [] { return ready; });
}

// ✅ 6. Retryable try-lock logic:-    Useful when you don’t want to block if the mutex is already held
void tryLockLoop() {
    std::unique_lock<std::mutex> lk(mtx, std::defer_lock);

    for (int i = 0; i < 3; ++i) {
        if (lk.try_lock()) {
            std::cout << "Acquired lock on attempt " << i << "\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



class TokenPool_DEFER_LOCK {
    std::mutex mtx;
    std::vector<Token> tokens_;

/*  ✅ Why this is useful:
✅ You can delay locking until you really need it.
✅ Useful when lock acquisition depends on pre-checks or other resources.
✅ Still RAII: lock is released automatically when unique_lock goes out of scope.*/
    Token getToken() {
        std::unique_lock<std::mutex> lock(mtx, std::defer_lock);  // don't lock yet

        // Simulate condition check or preparation
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // preparation phase

        lock.lock();  // manually lock when ready

        if (tokens_.empty()) {
            tokens_.push_back(Token::create());
        }
        Token t = std::move(tokens_.back());
        tokens_.pop_back();
        return t;
    }
};

/*  When would you use std::defer_lock then?
Use it only when:
    1.You want to create the unique_lock, but delay locking.
    2.You're going to lock it later manually with lk.lock().
    3.you're passing multiple locks into std::lock(...) to avoid deadlocks.

❌ Incorrect Example (what not to do here):
std::unique_lock<std::mutex> lk(mtx, std::defer_lock);  // ❌ Defer locking
cv.wait(lk);  // ❌ Undefined behavior: lk is not locked!*/

// Token pool with condition variable
struct TokenPool_condition_variable {
    std::mutex mtx;
    std::vector<Token> tokens_;
    std::condition_variable cv_;

    void returnToken(Token&& t) {
        std::unique_lock<std::mutex> lk(mtx);
        tokens_.push_back(std::move(t));
        lk.unlock();
        cv_.notify_one();
    }

    Token getToken() {
        std::unique_lock<std::mutex> lk(mtx);
        while (tokens_.empty()) {
            cv_.wait(lk);
        }
        Token t = std::move(tokens_.back());
        tokens_.pop_back();
        return t;
    }

    void addInitialTokens(size_t count) {
        std::lock_guard<std::mutex> lk(mtx);
        for (size_t i = 0; i < count; ++i) {
            tokens_.push_back(Token::create());
        }
    }
};
// Worker function
void worker_condition_variable(TokenPool_condition_variable& pool, int id) {
    Token t = pool.getToken();
    std::cout << "Thread " << id << " got a token\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // simulate work
    pool.returnToken(std::move(t));
    std::cout << "Thread " << id << " returned a token\n";
}
int Test_condition_variable() {
    TokenPool_condition_variable pool;
    pool.addInitialTokens(2); // Start with 2 tokens

    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker_condition_variable, std::ref(pool), i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads done.\n";
    return 0;
}


// inline auto& SingletonFoo::getInstance() {
//     static SingletonFoo instance;
//     return instance;
// }
/* The first thread arive start initialzing the staric insatance.
Any more that arrive will block and wait until the first thread eather succeeds
(unblocking them all) or fails with an exception (unblocking one of them)
✅ Best Practice
This pattern is now the recommended way to write a Singleton in modern C++:
class SingletonFoo {
public:
    static SingletonFoo& getInstance() {
        static SingletonFoo instance; // thread-safe since C++11
        return instance;
    }

private:
    SingletonFoo() = default;
    SingletonFoo(const SingletonFoo&) = delete;
    SingletonFoo& operator=(const SingletonFoo&) = delete;
};
*/


class SingletonFoo {
public:
        /*  Thread Safety: How?
        Under C++11 and later:

        If multiple threads call getInstance() at the same time on first access:
        Only one thread will enter the initialization of instance.

        The others will block and wait until:

        Initialization succeeds → all waiting threads proceed.

        Initialization throws an exception → only one other thread retries.

        This is part of what's called magic statics in C++11 and beyond.*/
    // Thread-safe Singleton accessor
    static SingletonFoo& getInstance() {
        static SingletonFoo instance; // Initialized only once, thread-safe in C++11+
        return instance;
    }

    void doWork(int thread_id) {
        std::lock_guard<std::mutex> lock(output_mtx_);
        std::cout << "Thread " << thread_id << " using SingletonFoo at address " << this << std::endl;
    }

private:
    SingletonFoo() {
        std::lock_guard<std::mutex> lock(output_mtx_);
        std::cout << "[SingletonFoo constructor called]\n";
    }

    // Disallow copy/move
    SingletonFoo(const SingletonFoo&) = delete;
    SingletonFoo& operator=(const SingletonFoo&) = delete;

    static std::mutex output_mtx_;
};

// Define the static mutex
std::mutex SingletonFoo::output_mtx_;

void Test_SingletonFoo() {
    const int num_threads = 5;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([i]() {
            // All threads call getInstance(), only the first initializes it
            SingletonFoo::getInstance().doWork(i);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads finished.\n";
    // Note: The constructor message appears once, no matter how many threads called getInstance() — proving that it's thread-safe and initialized just once.
}





struct NetworkConnection{};
class Logger {
    std::mutex mtx_;
    std::optional<NetworkConnection> conn_;

    NetworkConnection& getCon() {
        std::lock_guard<std::mutex> lk(mtx_);
        // We could add mutex_lock every conn_ . Code is safe but slower
        if (!conn_.has_value()) {
            conn_ = NetworkConnection();
        }
        return *conn_;
    }
};

class Logger {
    std::once_flag once_;
    std::optional<NetworkConnection> conn_;

    NetworkConnection& getCon() {
        /*  This mimics C++ does static initialization, but for a non-static.
        Each Logger has its own conn_, protected by its own once_.
            mimicking "magic statics" (function-local static variable behavior), but for non-static data members (i.e., per-instance rather than per-program).*/
        std::call_once(once_, [&]() {
            conn_ = NetworkConnection();
        });
        return *conn_;
        /*  This access to conn_ doesn't need to be protected boc
        it is definitely not the first access. We knowv that conn_ must be 
        initialize by now*/
    }
};


class THreadSafeConfig {
    std::map<std::string, int> settings_;
    mutable std::shared_mutex rw;

    void set(const std::string& name, int val) {
        std::unique_lock<std::shared_mutex> lk(rw);
        settings_.insert_or_assign(name, val);
    }
    int get(const std::string& name) const {
        std::shared_lock<std::shared_mutex> lk(rw);
        return settings_.at(name);
    }
};
/*  std::shared_mutex 
Multiple concurrent readers (shared access)
One writer (exclusive access)
Readers can access simultaneously as long as no writer holds the lock.

std::shared_lock
Works with std::shared_mutex
Acquires the lock in shared (read) mode
Multiple shared_locks can coexist
Cannot be used to modify the shared data


*/



/*  C++20 counting_semaphore */
class AnonymousTokenPool {
    std::counting_semaphore<256> sem_{100};// here 256 is max 100 initial
    void getToken() {
        sem_.acquire();  // may block
    }

    void requireToken() {
        sem_.release();
    }
};

using Sem = std::counting_semaphore<256>;
struct SemReleaser {
    bool operator()(Sem *s) const { s->release(); }
};
class UniwuePtrTokenPool {
    Sem sem_{100};
    using Token = std::unique_ptr<Sem, SemReleaser>;
    Token borrowToken() {
        sem_.acquire(); // may bolock
        return Token(&sem_);
    }
};
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