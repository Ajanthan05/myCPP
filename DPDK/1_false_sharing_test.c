#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

// #define _GNU_SOURCE
#include <time.h>

#define NUM_THREADS 4
#define NUM_ITER 100000000

// ❌ Potentially causes false sharing
volatile uint64_t shared_vars[NUM_THREADS];

// ✅ Avoid false sharing by padding to 64 bytes (cache line size)
struct __attribute__((aligned(64))) PaddedVar {
    volatile uint64_t val;
};

struct PaddedVar aligned_vars[NUM_THREADS];

void *false_sharing_thread(void *arg) {
    int idx = *(int *)arg;
    for (uint64_t i = 0; i < NUM_ITER; i++) {
        shared_vars[idx]++;
    }
    return NULL;
}

void *no_false_sharing_thread(void *arg) {
    int idx = *(int *)arg;
    for (uint64_t i = 0; i < NUM_ITER; i++) {
        aligned_vars[idx].val++;
    }
    return NULL;
}

void run_test(const char *label, void *(*func)(void *)) {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, func, &ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%s: %.6f seconds\n", label, elapsed);
}

int main() {
    run_test("❌ False sharing", false_sharing_thread);
    run_test("✅ No false sharing (aligned)", no_false_sharing_thread);
    return 0;
}
