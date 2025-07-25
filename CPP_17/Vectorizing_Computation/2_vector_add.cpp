// #include <immintrin.h>
// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>

// // Function to perform vector addition using AVX, AVX2, or AVX-512
// void vector_add(const float* a, const float* b, float* c, size_t size) {
//     size_t i = 0;

// #if defined(__AVX512F__)
//     // AVX-512: Process 16 floats at a time
//     for (; i + 15 < size; i += 16) {
//         __m512 va = _mm512_loadu_ps(a + i);
//         __m512 vb = _mm512_loadu_ps(b + i);
//         __m512 vc = _mm512_add_ps(va, vb);
//         _mm512_storeu_ps(c + i, vc);
//     }
// #elif defined(__AVX__)
//     // AVX/AVX2: Process 8 floats at a time
//     for (; i + 7 < size; i += 8) {
//         __m256 va = _mm256_loadu_ps(a + i);
//         __m256 vb = _mm256_loadu_ps(b + i);
//         __m256 vc = _mm256_add_ps(va, vb);
//         _mm256_storeu_ps(c + i, vc);
//     }
// #else
//     #error "AVX or AVX-512 support is required."
// #endif

//     // Process remaining elements
//     for (; i < size; ++i) {
//         c[i] = a[i] + b[i];
//     }
// }

// int main() {
//     const size_t size = 1024;
//     std::vector<float> a(size), b(size), c(size);

//     // Initialize input arrays with random values
//     std::srand(static_cast<unsigned>(std::time(nullptr)));
//     for (size_t i = 0; i < size; ++i) {
//         a[i] = static_cast<float>(std::rand()) / RAND_MAX;
//         b[i] = static_cast<float>(std::rand()) / RAND_MAX;
//     }

//     // Perform vector addition
//     vector_add(a.data(), b.data(), c.data(), size);

//     // Output the first 10 results
//     std::cout << "First 10 results of vector addition:\n";
//     for (size_t i = 0; i < 10; ++i) {
//         std::cout << "c[" << i << "] = " << c[i] << '\n';
//     }

//     return 0;
// }



#include <immintrin.h>
#include <iostream>
#include <vector>

void vector_add_avx2(const float* a, const float* b, float* c, size_t size) {
    size_t i = 0;
    for (; i + 7 < size; i += 8) {
        __m256 va = _mm256_loadu_ps(a + i);
        __m256 vb = _mm256_loadu_ps(b + i);
        __m256 vc = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(c + i, vc);
    }

    /*  Remaining elements
    After the SIMD loop, there might be 1–7 remaining elements (if size isn’t a multiple of 8).
This scalar loop handles those leftover elements one by one.*/ 
    for (; i < size; ++i) {
        c[i] = a[i] + b[i];
    }
}


int main() {
    const size_t size = 1024;
    std::vector<float> a(size, 1.0f);
    std::vector<float> b(size, 2.0f);
    std::vector<float> c(size);

    vector_add_avx2(a.data(), b.data(), c.data(), size);

    std::cout << "First 10 results:\n";
    for (size_t i = 0; i < 10; ++i)
        std::cout << "c[" << i << "] = " << c[i] << '\n';
}


/*  std::vector<float> a = {1.0f, 2.0f, 3.0f};
float* ptr = a.data();
Now, ptr points to &a[0].

std::vector<float> a = {1.0f, 2.0f, 3.0f};
float* ptr = a.data();
Now, ptr points to &a[0].
void process(float* data, size_t size) {
    data[1] = 5;
    for (size_t i = 0; i < size; ++i)
        std::cout << data[i] << ' ';
    std::cout << '\n';
}

int main() {
    std::vector<float> a = {1, 2, 3};
    process(a.data(), a.size());
    std::cout << a[1] << ' ';
}
*/