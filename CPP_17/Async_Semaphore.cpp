#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>

using namespace std;
using namespace std::chrono;
typedef long int ull;

/*  std::async
1. It runs a function asynchronously (Potentially in a new thread) and return 
std::future that will hold the result.
2. There are 3 lounch policies for creating task
    a. std::launch::async
    b. std::launch::deferred
    c. std::launch::async | std::launch::deferred
*/

ull findOdd(ull start, ull end) {
    ull oddSum = 0;
    for(ull i=start; i<+end; ++i) {
        if (i & 1) {
            oddSum += i;
        }
    }
    return oddSum;
}

int main() {
    ull start = 0, end =1900000000;
    std::future<ull> OddSum = std::async(std::launch::deferred, findOdd, start, end);
    end = 10;
    cout << "Waiting for the result\n";
    cout << "OddSum: " << OddSum.get() << " Completed\n";

    return 0;
}