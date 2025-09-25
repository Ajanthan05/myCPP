extern "C" {
#include <rte_eal.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_errno.h>   // <-- needed for rte_errno and rte_strerror
}
#include <unistd.h>  // for getopt, optind
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {
    // Step 1: Initialize DPDK EAL
    if (rte_eal_init(argc, argv) < 0) {
        std::cerr << "Failed to initialize EAL\n";
        return 1;
    }

    /*  optind is a global variable used by the standard library function getopt() (POSIX).
It tells you the index of the next argument to process in argv.
After getopt() finishes parsing the recognized options, optind points to the first non-option argument.*/
    for (int i = optind; i < argc; i++) {
        std::cout << "App arg: " << argv[i] << "\n";
    }

    // Step 2: Create a mempool for mbufs
    constexpr unsigned NUM_MBUFS = 8192;
    constexpr unsigned MBUF_CACHE_SIZE = 256;
    constexpr unsigned MBUF_PRIV_SIZE = 0;
    constexpr unsigned MBUF_DATA_SIZE = RTE_MBUF_DEFAULT_BUF_SIZE;

    rte_mempool* mbuf_pool = rte_pktmbuf_pool_create(
        "MBUF_POOL",        // pool name
        NUM_MBUFS,          // number of mbufs
        MBUF_CACHE_SIZE,    // per-core cache size
        MBUF_PRIV_SIZE,     // private data size
        MBUF_DATA_SIZE,     // data buffer size
        rte_socket_id()     // NUMA socket
    );

    if (mbuf_pool == nullptr) {
        std::cerr << "Failed to create mempool: " << rte_strerror(rte_errno) << "\n";
        return 1;
    }
    std::cout << "Mempool created successfully!\n";

    // Step 3: Allocate an mbuf from the pool
    rte_mbuf* mbuf = rte_pktmbuf_alloc(mbuf_pool);
    if (mbuf == nullptr) {
        std::cerr << "Failed to allocate mbuf\n";
        return 1;
    }
    std::cout << "Allocated mbuf, buf_len=" << mbuf->buf_len << "\n";

    // Simulate writing packet data
    char* data = rte_pktmbuf_mtod(mbuf, char*);
    const char* msg = "Hello, DPDK!";
    size_t msg_len = strlen(msg);

    rte_memcpy(data, msg, msg_len);
    mbuf->data_len = msg_len;
    mbuf->pkt_len  = msg_len;
    
    std::cout << "Wrote data: " << data << " (len=" << mbuf->data_len << ")\n";

    // Step 4: Free the mbuf back to the pool
    rte_pktmbuf_free(mbuf);
    std::cout << "Freed mbuf back to pool\n";

    return 0;
}
// g++ -o mempool_example mempool_example.cpp $(pkg-config --cflags --libs libdpdk)
// sudo ./mempool_example -l 0-1 -n 4 -- --example
/*  rte_pktmbuf_pool_create: Creates a pool of packet buffers (rte_mbuf objects).

rte_pktmbuf_alloc: Allocates an mbuf from the pool.

rte_pktmbuf_mtod: Converts mbuf data pointer to the requested type (char*).

rte_pktmbuf_free: Returns mbuf to the pool.*/