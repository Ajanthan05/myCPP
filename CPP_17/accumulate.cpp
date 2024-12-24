// it is redudent algorithm

#include <iostream>
#include <numeric>
#include <vector>

int main() {

    std::vector<int> v {1,2,3,4,5,6,7,8};

    std::cout << "Accumulate: " << std::accumulate(v.begin(), v.end(), 0) << std::endl; // (begin, end, initial)
    std::cout << "Multiplay: " << std::accumulate(v.begin(), v.end(), 1, std::multiplies<>()) << std::endl; // (begin, end, initial)

    std::vector<double> v2 {1.1,2.1,3.1,4,5,6,7,8};
    std::cout << "Accumulate v2: " << std::accumulate(v2.begin(), v2.end(), 0.0) << std::endl; // 0.0 NOTE

    return 0;
}