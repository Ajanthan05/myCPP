#include "shared_defs.hpp"
#include <iostream>
#include <rte_eal.h>
#include <rte_ring.h>
#include <rte_memzone.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (rte_eal_init(argc, argv) < 0) {
        std::cerr << "Failed to initialize EAL\n";
        return 1;
    }



    auto* ring = rte_ring_lookup(RING_NAME);
    if (!ring) {
        std::cerr << "Ring not found, creating as primary...\n";
        ring = rte_ring_create(RING_NAME, 1024, rte_socket_id(), RING_F_SP_ENQ | RING_F_SC_DEQ);
        if (!ring) {
            std::cerr << "Ring creation failed\n";
            return 1;
        }
    }

    const rte_memzone* mz = rte_memzone_lookup(MEMZONE_NAME);
    if (!mz) {
        std::cerr << "Creating shared memzone...\n";
        mz = rte_memzone_reserve(MEMZONE_NAME, sizeof(MyStruct) * MEMZONE_COUNT, rte_socket_id(), 0);
        if (!mz) {
            std::cerr << "Memzone creation failed\n";
            return 1;
        }
    }




    // auto* ring = rte_ring_lookup(RING_NAME);
    // if (!ring) {
    //     std::cerr << "Ring lookup failed\n";
    //     return 1;
    // }

    // const rte_memzone* mz = rte_memzone_lookup(MEMZONE_NAME);
    // if (!mz) {
    //     std::cerr << "Memzone lookup failed\n";
    //     return 1;
    // }

    MyStruct* obj = nullptr;

    while (true) {
        if (rte_ring_dequeue(ring, (void**)&obj) == 0) {
            std::cout << "Dequeued: ID=" << obj->id << ", Name=" << obj->name << "\n";
        } else {
            usleep(100000); // 100ms
        }
    }

    return 0;
}
