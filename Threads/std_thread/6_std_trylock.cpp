#include <iostream>
#include <thread>
#include <mutex>
using namespace std;


/*  std::lock(m1,m2,m3,m4)
1. All arguments are locked via a sequence of calls to lock(), try_lock(), or unlock() each argument
2. Order of lock is not defined (it will try to lock provided mutex in any order and ensure that
there is no dead lock)
3. It's is a blocking call.

eg  1, 2, 3, Not achive lock in 4  => Then ti release 3,2,1 and try to start locking from 4
    4, 1, 2, 3

[Example:1] --> No deadlock
    Thread 1                Thread 2
    std::lock(m1,m2);       std::lock(m1,m2);

[Example:2] --> No deadlock
    Thread 1                Thread 2
    std::lock(m1,m2);       std::lock(m2,m1);

[Example:3] --> No deadlock
    Thread 1                        Thread 2
    std::lock(m1,m2,m3,m4);         std::lock(m3,m4);
                                    std::lock(m1,m2);

[Example:1] --> Yes, deadlock
    Thread 1                Thread 2
    std::lock(m1,m2);       std::lock(m3,m4);
    std::lock(m3,m4);       std::lock(m1,m2);
*/


/* 
std::try_lock() tries to lock all the locable objects passed in it one by one in given order.
    std::try_lock(m1,m2,.....mn);

On success it returns -1 otherwise it will return 0-based mutex index number which it could 
not lock

If it fails to lock any of the mutex then it will release all the mutex it lock before

If a call to try_lock results in an exception, unlock is called for any locked object before rethrowing
*/

/*
It is same as mutex but,  same thread can lck one mutex multiple times using recursive_mutex
 (stack limit execied)
 and of we are calling 
How meny times we can lock recursive_mutex is not defined but when the number reach (stack limit execied)
and of we are calling lock() it will return std::system_error or if we were calllint try_lock() 
then it will return false
*/

std::recursive_mutex m1;
int buffer = 0;

void recursion(char c, int loopFor) {
    if (loopFor<0) return;

    m1.lock();
    cout << "It: " << c << " buffer: " << buffer++ << "\n";
    recursion(c, --loopFor);
    m1.unlock();
}


/*  LOCK_GUARD
It is very light weight wrapper for owning mutex on scoped basis
It aquires mutex lock the moment u create the object of lock_guard.
can't copy lock guard
can't explicitly unlock lock_guard.
*/

int count = 0;
std::timed_mutex m;

void timeIncrement(int i) {
    auto now = std::chrono::steady_clock::now();

    if (m.try_lock_until(now + std::chrono::seconds(2))) {
        ++count;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "Thread: " << i << " Entered\n";
        m.unlock();
    }
    else {
        cout << "Thread: " << i << " Coluden't Enter\n";
    }
}


int main() {
    std::thread t1(recursion, '1', 10);
    std::thread t2(recursion, '2', 10);

    t1.join();
    t2.join();
    return 0;
}