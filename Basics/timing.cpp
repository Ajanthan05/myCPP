#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

struct Timer
{
    // std::chrono::steady_clock::time_point start;
    std::chrono::high_resolution_clock::time_point start, end;
    // std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration <float> duration;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        // duration = end - start;
        // float ms
        // std::cout << "Timer took " << (duration.count() * 1000.0f) << "ms \n";
        // auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Timer took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ms \n";
    }
};

void Function()
{
    Timer time1;
    for (int i=0; i<100; i++)
        std::cout << "Hello\n";
}

int main() {

    Function();

    return 0;
}