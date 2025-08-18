#include "Implementation.h"
#include <chrono>

template <typename T, typename S>
void Benchmark(T& object, S function, const std::string& name) {
    auto start = std::chrono::high_resolution_clock::now();

    function(&object);

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    IC(name, duration.count());
}

int main() {
    CRTPDrived obj1;
    Benchmark(obj1, RunCRTP<CRTPDrived>, "CRTP Interface");

    return 0;
}