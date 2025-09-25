/*
extern "C" {
#include <rte_eal.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_malloc.h>
#include <rte_errno.h>
}

#include <iostream>
#include <cstring>
#include <cstdlib>

#define NUM_OBJS       8192
#define CACHE_SIZE     256
#define NUM_MBUFS      8192
#define MBUF_CACHE_SIZE 256
#define MBUF_PRIV_SIZE sizeof(struct Data)
#define MBUF_DATA_SIZE RTE_MBUF_DEFAULT_BUF_SIZE

// Custom struct
// struct Data {
//     uint32_t id;
//     char payload[128];
// };
struct Data {
    uint32_t id;
    char payload[128];
    char padding[4]; // pad to 132 + 4 = 136 → next multiple of 64 is 192
} __attribute__((aligned(64)));

// Global mempool
struct rte_mempool* data_pool = nullptr;
struct rte_mempool* mbuf_pool = nullptr;

// Initialize custom mempool for Data objects
void init_data_pool() {
    data_pool = rte_mempool_create(
        "DATA_POOL",              // pool name
        NUM_OBJS,                 // number of objects
        sizeof(Data),             // object size
        CACHE_SIZE,               // per-core cache
        0,                        // private area size
        nullptr, nullptr,         // obj ctor
        nullptr, nullptr,         // obj dtor
        rte_socket_id(),          // NUMA socket
        0                         // flags
    );

    if (!data_pool) {
        rte_exit(EXIT_FAILURE, "Cannot create DATA_POOL: %s\n", rte_strerror(rte_errno));
    }
}

// Allocate one Data object
Data* alloc_data() {
    Data* obj = nullptr;
    if (rte_mempool_get(data_pool, (void**)&obj) < 0) {
        std::cerr << "Data pool exhausted!\n";
        return nullptr;
    }
    return obj;
}

// Free one Data object
void free_data(Data* obj) {
    rte_mempool_put(data_pool, obj);
}

// Initialize mbuf pool with private Data space
void init_mbuf_pool() {
    mbuf_pool = rte_pktmbuf_pool_create(
        "MBUF_POOL",
        NUM_MBUFS,
        MBUF_CACHE_SIZE,
        MBUF_PRIV_SIZE,   // per-mbuf private size (Data)
        MBUF_DATA_SIZE,
        rte_socket_id()
    );

    mbuf_pool = rte_pktmbuf_pool_create(
        "MBUF_POOL",
        8192,           // number of mbufs
        256,            // per-core cache
        0,              // no private data for now
        RTE_MBUF_DEFAULT_BUF_SIZE,
        0               // socket 0
    );

    if (!mbuf_pool) {
        rte_exit(EXIT_FAILURE, "Cannot create MBUF_POOL: %s\n", rte_strerror(rte_errno));
    }
}

// Example: attach Data struct to mbuf
void example_mbuf_usage() {
    struct rte_mbuf* mbuf = rte_pktmbuf_alloc(mbuf_pool);
    if (!mbuf) {
        std::cerr << "Failed to alloc mbuf!\n";
        return;
    }

    // Access private area
    Data* d = reinterpret_cast<Data*>(rte_mbuf_to_priv(mbuf));
    d->id = 1234;
    strcpy(d->payload, "Hello from mbuf private data!");

    std::cout << "MBUF private Data id=" << d->id
              << " payload=" << d->payload << "\n";

    rte_pktmbuf_free(mbuf);
}

int main(int argc, char** argv) {
    // Initialize EAL
    if (rte_eal_init(argc, argv) < 0) {
        rte_exit(EXIT_FAILURE, "Failed to init EAL\n");
    }

    // Init mempools
    init_data_pool();
    init_mbuf_pool();

    // Example with standalone Data mempool
    Data* d1 = alloc_data();
    if (d1) {
        d1->id = 42;
        strcpy(d1->payload, "Hello DPDK Data Pool!");
        std::cout << "Allocated Data id=" << d1->id
                  << " payload=" << d1->payload << "\n";
        free_data(d1);
    }

    // Example with mbuf + private Data
    example_mbuf_usage();

    return 0;
}
*/

// /*
extern "C" {
#include <rte_eal.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_errno.h>
}

#include <iostream>
#include <cstring>
#include <cstdlib>

#define NUM_MBUFS       8192            // number of mbufs
#define MBUF_CACHE_SIZE 256             // per-core cache
#define MBUF_PRIV_SIZE  sizeof(struct Data)  // private data per mbuf
#define MBUF_DATA_SIZE  RTE_MBUF_DEFAULT_BUF_SIZE

// Custom struct to attach to mbuf private area
// struct Data {
//     uint32_t id;
//     char payload[128];
// };
struct Data {
    uint32_t id;
    char payload[128];
    char padding[4]; // pad to 132 + 4 = 136 → next multiple of 64 is 192
} __attribute__((aligned(64)));

// Global mbuf pool
struct rte_mempool* mbuf_pool = nullptr;

// Initialize mbuf pool
void init_mbuf_pool() {
    mbuf_pool = rte_pktmbuf_pool_create(
        "MBUF_POOL",
        NUM_MBUFS,
        MBUF_CACHE_SIZE,
        MBUF_PRIV_SIZE,    // private data per mbuf
        MBUF_DATA_SIZE,    // packet buffer size
        rte_socket_id()    // NUMA node
    );

    if (!mbuf_pool) {
        rte_exit(EXIT_FAILURE, "Cannot create MBUF_POOL: %s\n", rte_strerror(rte_errno));
    }
    std::cout << "MBUF_POOL created successfully!\n";
}

// Example: allocate mbuf + use private Data
void example_mbuf_usage() {
    struct rte_mbuf* mbuf = rte_pktmbuf_alloc(mbuf_pool);
    if (!mbuf) {
        std::cerr << "Failed to allocate mbuf!\n";
        return;
    }

    // Access private area
    Data* d = reinterpret_cast<Data*>(rte_mbuf_to_priv(mbuf));
    d->id = 42;
    strcpy(d->payload, "Hello from mbuf private data!");

    std::cout << "MBUF private Data id=" << d->id
              << " payload=" << d->payload << "\n";

    // Free mbuf
    rte_pktmbuf_free(mbuf);
}

int main(int argc, char** argv) {
    // Initialize EAL
    if (rte_eal_init(argc, argv) < 0) {
        rte_exit(EXIT_FAILURE, "Failed to init EAL\n");
    }

    // Initialize mbuf pool
    init_mbuf_pool();

    // Example usage
    example_mbuf_usage();

    return 0;
}
// */