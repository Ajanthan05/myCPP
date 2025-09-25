// l2_producer.cpp
extern "C" {
#include <rte_eal.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_ring.h>
#include <rte_version.h>
#include <rte_errno.h>
}
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <arpa/inet.h> // htons/ntohs

#define MBUF_POOL_NAME "MBUF_POOL"
#define RING_NAME      "TLV_RING"
#define NUM_MBUFS      4096
#define MBUF_CACHE     128
#define MBUF_DATA_SIZE RTE_MBUF_DEFAULT_BUF_SIZE
#define RING_SIZE      1024

#pragma pack(push,1)
typedef struct {
    uint16_t tag;
    uint16_t length;
    // uint8_t value[] follows in the stream
} tlv_t;
#pragma pack(pop)

int main(int argc, char** argv) {
    if (rte_eal_init(argc, argv) < 0) {
        std::cerr << "Failed to init EAL\n";
        return 1;
    }

    unsigned socket = rte_socket_id();

    // create mbuf pool (primary creates)
    struct rte_mempool* mbuf_pool = rte_pktmbuf_pool_create(
        MBUF_POOL_NAME,
        NUM_MBUFS,
        MBUF_CACHE,
        0,
        MBUF_DATA_SIZE,
        socket
    );
    if (!mbuf_pool) {
        std::cerr << "rte_pktmbuf_pool_create failed: " << rte_strerror(rte_errno) << "\n";
        return 1;
    }
    std::cout << "Producer: created mbuf pool\n";

    // create ring (single-producer single-consumer optimized)
    struct rte_ring* ring = rte_ring_create(RING_NAME, RING_SIZE,
                                           socket,
                                           RING_F_SP_ENQ | RING_F_SC_DEQ);
    if (!ring) {
        std::cerr << "rte_ring_create failed: " << rte_strerror(rte_errno) << "\n";
        return 1;
    }
    std::cout << "Producer: created ring\n";

    uint16_t tag = 0x1001;
    uint32_t counter = 0;
    while (true) {
        // Create a TLV payload every second
        const char* text = "This is TLV payload from L2";
        uint16_t value_len = static_cast<uint16_t>(strlen(text));
        uint16_t total_len = sizeof(tlv_t) + value_len;

        // Allocate mbuf
        struct rte_mbuf* m = rte_pktmbuf_alloc(mbuf_pool);
        if (!m) {
            std::cerr << "Producer: Failed to allocate mbuf\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        // Ensure contiguous space in first segment for TLV (typical for small payloads)
        uint8_t* ptr = rte_pktmbuf_mtod(m, uint8_t*);
        // write TLV in network byte order for portability
        uint16_t tag_be = htons(tag);
        uint16_t len_be = htons(value_len);

        memcpy(ptr, &tag_be, sizeof(tag_be));
        memcpy(ptr + sizeof(tag_be), &len_be, sizeof(len_be));
        memcpy(ptr + sizeof(tlv_t), text, value_len);

        m->data_len = total_len;
        m->pkt_len  = total_len;

        // Enqueue mbuf pointer into ring (single-producer flag used, so use sp enqueue)
        int ret = rte_ring_sp_enqueue(ring, m);
        if (ret == 0) {
            std::cout << "Producer: enqueued TLV tag=0x" << std::hex << tag
                      << std::dec << " len=" << value_len << " cnt=" << counter << "\n";
        } else {
            // ring full -> drop/free
            std::cerr << "Producer: ring full, dropping packet\n";
            rte_pktmbuf_free(m);
        }

        ++counter;
        ++tag; // change tag each second for demo
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // never reached in this demo
    return 0;
}
