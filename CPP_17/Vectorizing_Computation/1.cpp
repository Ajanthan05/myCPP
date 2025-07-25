// #include <iostream>
// #include <vector>
// #include <climits>
// #include <queue>
// #include <utility>
// #include <algorithm>
// #include <stack>

// #include <x86intrin.h>
// #include <immintrin.h> //<simd>
// using namespace std;

// // SIMD_ACCESS library

// // int main() {


// //     return 0;
// // }

// int main() {
//     using namespace std::experimental;

//     // Input arrays
//     alignas(64) float a[16] = {1, 2, 3, 4, 5, 6, 7, 8,  9, 10, 11, 12, 13, 14, 15, 16};
//     alignas(64) float b[16] = {16, 15, 14, 13, 12, 11, 10, 9,  8, 7, 6, 5, 4, 3, 2, 1};
//     alignas(64) float c[16];

//     // Use float simd vector
//     for (size_t i = 0; i < 16; i += simd<float>::size()) {
//         simd<float> va(&a[i], element_aligned);
//         simd<float> vb(&b[i], element_aligned);
//         simd<float> vc = va + vb;
//         vc.copy_to(&c[i], element_aligned);
//     }

//     // Print result
//     for (float x : c) {
//         std::cout << x << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }


// 1.cpp
// #include "xsimd/xsimd.hpp"
// #include <iostream>

// int main() {
//     alignas(64) float a[16] = {1,2,3,4,5,6,7,8, 9,10,11,12,13,14,15,16};
//     alignas(64) float b[16] = {16,15,14,13,12,11,10,9, 8,7,6,5,4,3,2,1};
//     alignas(64) float c[16];

//     using batch_type = xsimd::batch<float, 16>;
//     for (size_t i = 0; i < 16; i += batch_type::size) {
//         batch_type va = xsimd::load_aligned(&a[i]);
//         batch_type vb = xsimd::load_aligned(&b[i]);
//         batch_type vc = va + vb;
//         vc.store_aligned(&c[i]);
//     }

//     for (auto x : c) std::cout << x << " ";
//     std::cout << std::endl;
// }

#include <immintrin.h>
#include <iostream>

int main() {
    alignas(32) float a[8] = {1,2,3,4,5,6,7,8};
    alignas(32) float b[8] = {8,7,6,5,4,3,2,1};
    alignas(32) float c[8];

    __m256 va = _mm256_load_ps(a);
    __m256 vb = _mm256_load_ps(b);
    __m256 vc = _mm256_add_ps(va, vb);
    _mm256_store_ps(c, vc);

    for (auto x : c) std::cout << x << " ";
    std::cout << std::endl;

    return 0;
}
