#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

/* try_lock function
1. std::try_lock
2. std::mutex::try_lock
3. std::shared_lock::try_lock
4. std::time_mutex::try_lock
5. std::unique_lock::try_lock
6. std::shared_mutex::try_lock
7. std::recursive_mutex::try_lock
8. std::shared_timed_mutex::try_lock
9. std::recursive_timed_mutex::try_lock
*/


// std::mutex::try_lock
// 1. try_lock() tries to lock the mutex. Teturns imediately. On successful lock acquisition return true otherwise return false.
// 2. If try_lock() is not able to lock the mutex, then it"s doesn't get blocked that's why it is called non-blocking.
// 3. If try_lock() is again called by the same thread which owns the mutex, the behavior is undefined.
//    It's the dead lock situation with the undefine behaviour. ( if u want to be able to lock the same mutex by the same thread 
//    more than one time, go for recursive_mutex)


int count = 0;
std::mutex mtx;

void counter() {
    for (int i=0; i<100000; ++i) {
        if( mtx.try_lock()) {
            ++count;
            mtx.unlock();
        }

        if( mtx.try_lock()) {
            ++count;
            mtx.unlock();
        }
    }
}

/*  A mutex (mutual exclusion) is a synchronization primitive that allows only one thread to access a resource at a time.

std::unique_lock::try_lock()
Works on a unique_lock that was created with std::defer_lock.

Returns true if it got the lock.

std::mutex m;
std::unique_lock<std::mutex> lock(m, std::defer_lock);
if (lock.try_lock()) {
    // locked successfully
}

4. std::shared_lock::try_lock() (for shared_mutex)
Attempts to acquire a shared (reader) lock.

Used when multiple threads can read concurrently.

std::shared_mutex sm;
std::shared_lock lock(sm, std::defer_lock);
if (lock.try_lock()) {
    // got shared access
}

5. std::timed_mutex::try_lock()
Tries to acquire the lock without blocking.

Also supports try_lock_for(duration) and try_lock_until(time_point) for timed waits.

std::timed_mutex tm;
if (tm.try_lock()) {
    // locked
}
6. std::recursive_mutex::try_lock()
Like std::mutex, but allows same thread to lock multiple times.

Useful in recursive functions.

std::recursive_mutex rm;
if (rm.try_lock()) {
    rm.try_lock();  // okay — same thread
    rm.unlock();
    rm.unlock();
}
7. std::recursive_timed_mutex::try_lock()
Recursive + timed locking.

Supports try_lock_for and try_lock_until.

std::recursive_timed_mutex rtm;
if (rtm.try_lock()) {
    // locked
}
8. std::shared_mutex::try_lock() and try_lock_shared()
try_lock() → exclusive (write) access

try_lock_shared() → shared (read) access


std::shared_mutex sm;
if (sm.try_lock()) {
    // got write lock
}
if (sm.try_lock_shared()) {
    // got read lock
}
9. std::shared_timed_mutex::try_lock()
Like shared_mutex, but adds timed locking versions.

For both shared and exclusive access with timeouts.

std::shared_timed_mutex stm;
if (stm.try_lock()) {
    // write lock
}
if (stm.try_lock_shared()) {
    // read lock
}


*/

int main() {
    std::thread t1(counter);
    std::thread t2(counter);

    t1.join();
    t2.join();

    cout << "Count: " << count << endl;
    return 0;
}