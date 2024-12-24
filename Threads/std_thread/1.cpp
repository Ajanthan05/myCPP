
/*
WAYS TO CREATE THREADS IN C++11
1. Function Pointers
2. Lambda Functions
3. Functors
4. Member Functions
5. Static Member Functions
*/

// REQUIREMENT
// Find the addition of all odd numbers from 1-1900000000 adf even numbers from 1-1900000000

#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;
typedef unsigned long long ull;

ull OddSum = 0;
ull EvenSum = 0;

void findEven(ull start, ull end) {
    for (ull i=start; i<=end; ++i) {
        if((i & 1) == 0) EvenSum += i;
    }
}

void findOdd(ull start, ull end) {
    for (ull i=start; i<=end; ++i) {
        if((i & 1) == 1) OddSum += i;
    }
}

int main() {
    ull start = 0, end = 1900000000;

    auto startTime = high_resolution_clock::now();

    // findOdd(start, end);
    // findEven(start, end);

    std::thread t1(findOdd, start, end);
    std::thread t2(findEven, start, end);

    t1.join();
    t2.join();

    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);

    cout << "OddSum : " << OddSum << endl;
    cout << "EvenSum : " << EvenSum << endl; 

    cout << "Sec: " << duration.count()/1000000 << endl;

    return 0;
}