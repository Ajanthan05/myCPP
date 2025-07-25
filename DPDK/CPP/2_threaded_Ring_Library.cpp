#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

#include <rte_eal.h>
#include <rte_ring.h>
#include <rte_malloc.h>

const unsigned RING_SIZE = 1024;
const char* RING_NAME = "multi_thread_ring";
const int NUM_ITEMS = 100;

// Global control flag
std::atomic<bool> keep_running{true};

void producer_thread(rte_ring* ring) {
    for (int i = 0; i < NUM_ITEMS; ++i) {
        int* data = static_cast<int*>(rte_malloc("data", sizeof(int), 0));
        *data = i;

        /*  rte_ring_enqueue(ring, data) tries to insert data into the ring buffer.
It returns 0 on success and -ENOBUFS (typically -1) if the ring is full.
If the ring is full, the enqueue fails, so the loop retries.*/
        while (rte_ring_enqueue(ring, data) < 0) {
            // Wait if ring is full
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::cout << "[Producer] Enqueued: " << *data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    keep_running = false;
}

void consumer_thread(rte_ring* ring) {
    // The producer thread sets keep_running = false; after it finishes producing.
    // The consumer thread keeps checking keep_running in its loop condition:

    while (keep_running || !rte_ring_empty(ring)) {
        void* ptr = nullptr;
        if (rte_ring_dequeue(ring, &ptr) == 0) {
            int* value = static_cast<int*>(ptr);
            std::cout << "      [Consumer] Dequeued: " << *value << std::endl;
            rte_free(value);
        } else {
            // Ring is empty, wait
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

int main(int argc, char** argv) {
    if (rte_eal_init(argc, argv) < 0) {
        std::cerr << "Failed to initialize DPDK EAL\n";
        return -1;
    }

    // Create ring
    rte_ring* ring = rte_ring_create(RING_NAME, RING_SIZE, rte_socket_id(),
                                     RING_F_SP_ENQ | RING_F_SC_DEQ);
    if (!ring) {
        std::cerr << "Failed to create ring\n";
        return -1;
    }

    // Launch producer and consumer threads
    std::thread producer(producer_thread, ring);
    std::thread consumer(consumer_thread, ring);

    // Wait for both to finish
    producer.join();
    consumer.join();

    std::cout << "Processing complete." << std::endl;
    return 0;
}
