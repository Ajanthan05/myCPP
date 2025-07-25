#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;


#define ulc unsigned long long const

void worker(int number) {
    cout << "Thread NO: " << number << "\n";
}

int main() {
    vector<std::thread> workers;

    ulc hardware_threads = std::thread::hardware_concurrency();

    cout << hardware_threads << endl;

    for(int i=0; i<hardware_threads; ++i) {
        workers.emplace_back(std::thread(worker, i));
    }

    std::for_each(workers.begin(), workers.end(), [](std::thread &t) {
        assert(t.joinable());
        t.join();
    });
}