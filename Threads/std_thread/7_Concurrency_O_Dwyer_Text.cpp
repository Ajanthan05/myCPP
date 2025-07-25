#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <map>
#include <string>
#include <condition_variable>
#include <chrono>

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
/*  The code on this slide still has a potential bug. What 
happens if Token::create() or push_back() throws an exception?
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
1.  When you want to unlock manually inside a function.
2.  When you want to lock later (not at constructor).
3.  When you want to try_lock with timeout (lock for a limited time).
4.  When you want to move a lock across scopes (e.g., hand a lock to another thread or object).*/



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
    mutable std::mutex mtx_;
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

std::mutex Mtx_1, Mtx_2;
void task_A() {
    std::scoped_lock lock(Mtx_1, Mtx_2); // lock both together safely
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    std::cout << "Task A finished" << std::endl;
}
void task_B() {
    std::scoped_lock lock(Mtx_1, Mtx_2); // lock both together safely
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Task B finished" << std::endl;
}
void Scoped_lock() {
    std::thread t1(task_A);
    std::thread t2(task_B);

    t1.join();
    t2.join();
}




////////////////////////////////////////////////////////////////////////////////////////////////////
/*  std::ref is a utility function that creates a reference wrapper — 
an object that behaves like a reference but can be copied and stored in standard containers.

It’s part of the <functional> header

Why Can't STL Containers Hold References Directly?
C++ does not allow storing references (T&) in STL containers because:

References must be bound at initialization and cannot be reseated.

STL containers need to copy or move elements internally (e.g., during reallocation or sorting).

References aren’t assignable — containers expect assignable elements.

std::thread t(print, std::ref(a));  // ✅ passes a by reference


Why Can't STL Containers Hold References Directly?
C++ does not allow storing references (T&) in STL containers because:

References must be bound at initialization and cannot be reseated.

STL containers need to copy or move elements internally (e.g., during reallocation or sorting).

References aren’t assignable — containers expect assignable elements.*/
void Tref() {
std::vector<std::reference_wrapper<int>> v;
int a = 10, b = 20;
v.push_back(std::ref(a));
v.push_back(std::ref(b));
v[0].get() = 99;  // modifies `a`
}


/*  Creating/destroying the object is expensive
Multiple threads need temporary ownership
Objects can be reused instead of recreated*/
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
    mutable std::mutex mtx;
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


///////////////////////////////////////////////////////////////////////////
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
std::mutex m_1, m_2;
void example() {
    std::unique_lock<std::mutex> lk1(m_1, std::defer_lock);
    std::unique_lock<std::mutex> lk2(m_2, std::defer_lock);

    std::lock(lk1, lk2);  // Deadlock-safe!
    // Do work...
    // Automatically unlocks when lk1 and lk2 go out of scope
}
/*  
| Feature              | `std::lock_guard`  | `std::unique_lock`  |
| -------------------- | -----------------  | ------------------  |
| Automatic locking    | ✅                 | ✅ (by default)     |
| Can defer locking    | ❌                 | ✅ (`defer_lock`)   |
| Manual lock/unlock   | ❌                 | ✅                  |
| Movable              | ❌                 | ✅                  |
| Works with cond vars | ⚠️ Limited         | ✅                  |


| Feature            | `std::lock`                                           | `std::scoped_lock`                                        |
| ------------------ | ----------------------------------------------------- | --------------------------------------------------------- |
| What it is         | A function                                            | A class (RAII-style)                                      |
| Purpose            | Locks multiple mutexes (but doesn't manage unlocking) | Locks multiple mutexes **and** automatically unlocks them |
| RAII (auto-unlock) | ❌ No — you must unlock manually                      | ✅ Yes — unlocks on scope exit                            |
| Locking mechanism  | `std::lock(m1, m2, ...)`                              | `std::scoped_lock l(m1, m2, ...)`                         |
| Usage style        | Use with `std::unique_lock` + `defer_lock`            | Directly with mutexes                                     |
| Introduced in      | C++11                                                 | C++17                                                     |

But What Can Go Wrong?
✅ Runtime correctness is guaranteed only if...:
| Risk                                                                                  | Explanation                                                                     |
| ------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| ❌ One of the mutexes is already locked by the **same thread**                         | `std::mutex` is not reentrant → leads to **deadlock** or **undefined behavior** |
| ❌ A mutex is **invalid or destroyed** before locking                                  | Will throw or crash                                                             |
| ❌ Code uses **mixed lock types** (e.g., `scoped_lock` and raw `lock()` on same mutex) | May cause data race or deadlock                                                 |
| ❌ A `std::lock()` is **not used with `std::defer_lock`** `unique_lock`s               | Causes undefined behavior or immediate deadlock                                 |


std::lock
std::lock() is a function that locks multiple mutexes safely (deadlock-free), but does not manage unlocking. It is used with std::unique_lock (with std::defer_lock) like this
Use when you need more control (e.g., timed wait, manual unlock).

std::scoped_lock
std::scoped_lock is a RAII-style locking class introduced in C++17 that locks multiple mutexes immediately in a deadlock-free way, and unlocks them automatically when it goes out of scope.
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
    // When this function ends, `lk` goes out of scope → mutex is unlocked here
}
void caller() {
    std::mutex m;
    std::unique_lock<std::mutex> lk(m, std::defer_lock);

    // ... prepare work
    lk.lock();

    doLockedWork(std::move(lk));  // ownership of lock (and mutex) is transferred
    // After this point, `lk` in caller is empty (no longer owns the lock)
}

// ✅ 4. Use with std::lock() for multiple mutexes:-   Deadlock-free locking of multiple mutexes
std::mutex m4_1, m4_2;
void lockTwo() {
    std::unique_lock<std::mutex> lk1(m4_1, std::defer_lock);
    std::unique_lock<std::mutex> lk2(m4_2, std::defer_lock);

    std::lock(lk1, lk2);  // Avoids deadlock

    // Now both are safely locked

    /*  std::lock() — a special function in the C++ Standard Library designed to lock multiple mutexes atomically and safely.
// Thread 1:
lock(m4_1);
lock(m4_2);

// Thread 2:
lock(m4_2);
lock(m4_1);
If Thread 1 locks m4_1 and Thread 2 locks m4_2 at the same time, both will wait forever trying to lock the other → ❌ Deadlock.

✅ The Solution:
std::lock(lk1, lk2);
    1.Uses an internal deadlock-avoidance algorithm.
    2.Attempts to acquire both mutexes without risk of deadlock.
    3.Locks them in a defined consistent order, regardless of how you wrote them.

If it can’t lock both immediately, it releases what it locked and tries again safely.

🔁 std::defer_lock
This is important:
std::unique_lock<std::mutex> lk1(m4_1, std::defer_lock);
Means:
    1.Don’t lock the mutex now.
    2.Just associate the lock with the mutex.
    3.This lets std::lock() manage the actual locking later.*/
}

// ✅ 5. With std::condition_variable:-    Often used for waiting on a condition
std::mutex mtx1;
std::condition_variable cv1;
bool ready1 = false;

void waitUntilReady() {
    std::unique_lock<std::mutex> lk(mtx1, std::defer_lock);

    // prepare();

    lk.lock();  // lock before waiting
    cv1.wait(lk, [] { return ready1; });
}

// ✅ 6. Retryable try-lock logic:-    Useful when you don’t want to block if the mutex is already held
void tryLockLoop() {
    std::unique_lock<std::mutex> lk(mtx1, std::defer_lock);

    for (int i = 0; i < 3; ++i) {
        if (lk.try_lock()) {
            std::cout << "Acquired lock on attempt " << i << "\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



/*  std::shared_mutex?
It boosts performance when:

Reads are frequent and safe to parallelize.

| Function          | Description                        |
| ----------------- | ---------------------------------- |
| `lock()`          | Acquire **exclusive** (write) lock |
| `unlock()`        | Release exclusive lock             |
| `lock_shared()`   | Acquire **shared** (read) lock     |
| `unlock_shared()` | Release shared lock                |
*/
std::shared_mutex sm;
int shared_data = 0;

void reader(int id) {
    std::shared_lock lock(sm);
    std::cout << "Reader " << id << " reads: " << shared_data << "\n";
}

void writer(int id) {
    std::unique_lock lock(sm);
    ++shared_data;
    std::cout << "Writer " << id << " updated to: " << shared_data << "\n";
}
// Readers can run in parallel.
// Writers are serialized — they block both other writers and all readers.

// defer_lock, try_lock, etc. std::shared_timed_mutex for try_lock_for() and try_lock_until():
int main() {
    Test_unique_lock();
    // DeadLock();
    Scoped_lock();
    Test();
}