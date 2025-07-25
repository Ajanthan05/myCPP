#include <iostream>
#include <cstring>
#include <rte_eal.h>
#include <rte_ring.h>
#include <rte_malloc.h>
#include <rte_launch.h>
#include <rte_lcore.h>

const char* RING_NAME = "my_ring";
const int RING_SIZE = 1024;

struct MyStruct {
    int id;
    char name[32];
};

int main(int argc, char** argv) {
    if (rte_eal_init(argc, argv) < 0) {
        std::cerr << "Failed to init EAL\n";
        return -1;
    }

    rte_ring* ring = rte_ring_create(RING_NAME, RING_SIZE,
                                     rte_socket_id(), RING_F_SP_ENQ | RING_F_SC_DEQ);
    if (!ring) {
        std::cerr << "Ring creation failed\n";
        return -1;
    }

    // Allocate and enqueue multiple structs
    for (int i = 0; i < 10; ++i) {
        MyStruct* obj = static_cast<MyStruct*>(
            rte_malloc("my_struct_type", sizeof(MyStruct), alignof(MyStruct))
        );
        if (!obj) continue;

        obj->id = i;
        snprintf(obj->name, sizeof(obj->name), "Item_%d", i);

        while (rte_ring_enqueue(ring, obj) < 0) {
            rte_delay_ms(1);
        }

        std::cout << "Enqueued: " << obj->name << std::endl;
    }

    std::cout << "Producer finished. Keep ring alive." << std::endl;
    while (1) rte_delay_ms(1000); // Keep running
}

/*  
g++ 3_producer.cpp -o producer $(pkg-config --cflags --libs libdpdk)
sudo ./producer -l 0 -n 4 --proc-type=primary

g++ 3_consumer.cpp -o consumer $(pkg-config --cflags --libs libdpdk)
sudo ./consumer -l 1 -n 4 --proc-type=secondary

DPDK rings are shared between processes using --proc-type and memory-mapping.
rte_malloc allocates memory in DPDK’s managed heap, accessible from both processes.
Use rte_ring_lookup() in secondary processes instead of rte_ring_create().
*/