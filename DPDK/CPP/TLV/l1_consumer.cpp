// l1_consumer.cpp
extern "C" {
#include <rte_eal.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_ring.h>
#include <rte_errno.h>
}
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <arpa/inet.h>

#define MBUF_POOL_NAME "MBUF_POOL"
#define RING_NAME      "TLV_RING"

#pragma pack(push,1)
typedef struct {
    uint16_t tag;
    uint16_t length;
} tlv_t;
#pragma pack(pop)

int main(int argc, char** argv) {
    // Secondary process should pass same EAL args (file-prefix, huge-dir). Use --proc-type=secondary.
    if (rte_eal_init(argc, argv) < 0) {
        std::cerr << "Consumer: Failed to init EAL\n";
        return 1;
    }

    // lookup shared ring and pool created by primary
    struct rte_ring* ring = rte_ring_lookup(RING_NAME);
    if (!ring) {
        std::cerr << "Consumer: rte_ring_lookup failed\n";
        return 1;
    }

    struct rte_mempool* mbuf_pool = rte_mempool_lookup(MBUF_POOL_NAME);
    if (!mbuf_pool) {
        std::cerr << "Consumer: rte_mempool_lookup failed\n";
        return 1;
    }

    std::cout << "Consumer: attached to ring and mbuf pool\n";

    while (true) {
        void* obj = nullptr;
        int ret = rte_ring_sc_dequeue(ring, &obj); // single-consumer optimized dequeue
        if (ret == 0 && obj != nullptr) {
            struct rte_mbuf* m = reinterpret_cast<struct rte_mbuf*>(obj);
            // parse TLV
            uint8_t* data = rte_pktmbuf_mtod(m, uint8_t*);
            if (m->data_len >= sizeof(tlv_t)) {
                uint16_t tag_be, len_be;
                memcpy(&tag_be, data, sizeof(tag_be));
                memcpy(&len_be, data + sizeof(tag_be), sizeof(len_be));
                uint16_t tag = ntohs(tag_be);
                uint16_t len = ntohs(len_be);

                std::cout << "Consumer: Got TLV tag=0x" << std::hex << tag
                          << std::dec << " len=" << len << " data=";

                // print value (careful: not NUL-terminated)
                for (uint16_t i = 0; i < len && (i + sizeof(tlv_t)) < m->data_len; ++i)
                    std::cout << data[sizeof(tlv_t) + i];
                std::cout << "\n";
            } else {
                std::cerr << "Consumer: truncated TLV, data_len=" << m->data_len << "\n";
            }

            // free mbuf back to pool
            rte_pktmbuf_free(m);
        } else {
            // nothing to read -> sleep a bit
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    return 0;
}
