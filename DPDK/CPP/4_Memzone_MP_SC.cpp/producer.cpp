#include "shared_defs.hpp"
#include <iostream>
#include <rte_eal.h>
#include <rte_ring.h>
#include <rte_memzone.h>
#include <cstring>

int main(int argc, char** argv) {
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) {
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

    auto* array = static_cast<MyStruct*>(mz->addr);
    int start_idx = rand() % (MEMZONE_COUNT / 2);

    for (int i = 0; i < 5; ++i) {
        int idx = (start_idx + i) % MEMZONE_COUNT;
        array[idx].id = idx;
        snprintf(array[idx].name, sizeof(array[idx].name), "Producer_%d", idx);
        rte_ring_enqueue(ring, &array[idx]);
        std::cout << "Enqueued: " << array[idx].name << "\n";
    }

    return 0;
}


/*
g++ -o consumer consumer.cpp $(pkg-config --cflags --libs libdpdk)
g++ -o producer producer.cpp $(pkg-config --cflags --libs libdpdk)

sudo ./consumer -l 0 -n 4 --proc-type=primary

sudo ./producer -l 1 -n 4 --proc-type=secondary
sudo ./producer -l 2 -n 4 --proc-type=secondary

cat /proc/meminfo | grep -i huge

*/