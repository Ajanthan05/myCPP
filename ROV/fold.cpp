#include <iostream>

int main() {
    return 121212 + 212121;
}

// g++ -S -O0 -std=c++11 fold.cpp && cat fold.s | grep 333333