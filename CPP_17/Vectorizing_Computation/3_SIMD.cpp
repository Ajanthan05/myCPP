#include <immintrin.h>
#include <iostream>

void add_int_vectors(const int* a, const int* b, int* c, size_t size) {
    size_t i = 0;
    for (; i + 7 < size; i += 8) {
        __m256i va = _mm256_loadu_si256((__m256i const*)(a + i));
        __m256i vb = _mm256_loadu_si256((__m256i const*)(b + i));
        __m256i vc = _mm256_add_epi32(va, vb);
        _mm256_storeu_si256((__m256i*)(c + i), vc);
    }

    // Handle remainder
    for (; i < size; ++i)
        c[i] = a[i] + b[i];
}

void Integers() {
    const size_t size = 16;
    int a[size] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    int b[size] = {16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    int c[size];

    add_int_vectors(a, b, c, size);

    std::cout << "Result:\n";
    for (size_t i = 0; i < size; ++i)
        std::cout << c[i] << ' ';
    std::cout << '\n';
}

void multiply_float_vectors(const float* a, const float* b, float* c, size_t size) {
    size_t i = 0;
    for (; i + 7 < size; i += 8) {
        __m256 va = _mm256_loadu_ps(a + i);
        __m256 vb = _mm256_loadu_ps(b + i);
        __m256 vc = _mm256_mul_ps(va, vb);
        _mm256_storeu_ps(c + i, vc);
    }

    for (; i < size; ++i)
        c[i] = a[i] * b[i];
}

int Test_multiply_float_vectors() {
    const size_t size = 8;
    float a[size] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0};
    float b[size] = {2.0,2.0,2.0,2.0,2.0,2.0,2.0,2.0};
    float c[size];

    multiply_float_vectors(a, b, c, size);

    for (size_t i = 0; i < size; ++i)
        std::cout << c[i] << ' ';
    std::cout << '\n';
}

/*  Horizontal Sum of a Vector
If you have an 8-element vector and want to sum them together */

float horizontal_sum_avx2(const float* data) {
    __m256 vec = _mm256_loadu_ps(data);

    __m128 low  = _mm256_castps256_ps128(vec);       // lower 128 bits
    __m128 high = _mm256_extractf128_ps(vec, 1);     // upper 128 bits

    __m128 sum128 = _mm_add_ps(low, high);           // sum lower and upper parts

    // Now sum within the 128-bit lane
    sum128 = _mm_hadd_ps(sum128, sum128);
    sum128 = _mm_hadd_ps(sum128, sum128);

    return _mm_cvtss_f32(sum128);
}

int main() {
    float a[8] = {1,2,3,4,5,6,7,8};
    std::cout << "Sum: " << horizontal_sum_avx2(a) << '\n';
}


//  4 Element-wise Maximum of 8 floats
void max_float_vectors(const float* a, const float* b, float* c, size_t size) {
    size_t i = 0;
    for (; i + 7 < size; i += 8) {
        __m256 va = _mm256_loadu_ps(a + i);
        __m256 vb = _mm256_loadu_ps(b + i);
        __m256 vc = _mm256_max_ps(va, vb);
        _mm256_storeu_ps(c + i, vc);
    }

    for (; i < size; ++i)
        c[i] = std::max(a[i], b[i]);
}

/*
| Operation | Float (`__m256`)   | Int (`__m256i`)                  |
| --------- | ------------------ | -------------------------------- |
| Load      | `_mm256_loadu_ps`  | `_mm256_loadu_si256`             |
| Store     | `_mm256_storeu_ps` | `_mm256_storeu_si256`            |
| Add       | `_mm256_add_ps`    | `_mm256_add_epi32`               |
| Subtract  | `_mm256_sub_ps`    | `_mm256_sub_epi32`               |
| Multiply  | `_mm256_mul_ps`    | No direct int multiply in AVX2   |
| Divide    | `_mm256_div_ps`    | N/A                              |
| Max       | `_mm256_max_ps`    | `_mm256_max_epi32` (signed ints) |
| Min       | `_mm256_min_ps`    | `_mm256_min_epi32` (signed ints) |
    */

/*  ???
🟦 In Practice
It’s safe if:
✅ The memory is properly aligned for the instruction (or if using _mm256_loadu_* which works for unaligned).
✅ The memory has at least 32 bytes available at the address.

Let me know if you want me to show how to handle proper alignment (_mm256_load_si256) or to see what happens if the alignment is wrong (crash/performance penalty)!

Tips for aligning memory for faster loads?
*/