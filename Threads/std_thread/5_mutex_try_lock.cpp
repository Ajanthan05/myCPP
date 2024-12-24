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

int main() {
    std::thread t1(counter);
    std::thread t2(counter);

    t1.join();
    t2.join();

    cout << "Count: " << count << endl;
    return 0;
}