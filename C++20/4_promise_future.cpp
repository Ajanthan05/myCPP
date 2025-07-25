#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>
using namespace std;
using namespace std::chrono;
typedef long int ull;

void findOdd(std::promise<ull> &&OddsumPromis, ull start, ull end) {
    ull OddSum = 0;
    for(ull i=start; i<=end; ++i) {
        if (i & 1) OddSum += i;
    }
    OddsumPromis.set_value(OddSum);
}
/*
promise:- Used to set values or exceptions.
feature:- Used to get values from promis
          Ask promise of the value is availabel.
          wait for the promise.
*/

int main() {
    ull start = 0, end = 190000000;

    std::promise<ull> OddSum;
    std::future<ull> OddFeature = OddSum.get_future();

    std::thread t1(findOdd, std::move(OddSum), start, end);

    cout << "Waitting For Result\n";

    cout << "OddSum: " << OddFeature.get() << "\n";

    cout << "Completed\n";

    t1.join();
}