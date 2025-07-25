#include <iostream>
#include <rte_eal.h>
#include <rte_ring.h>
#include <rte_malloc.h>
#include <rte_launch.h>
#include <rte_lcore.h>

const char* RING_NAME = "my_ring";

struct MyStruct {
    int id;
    char name[32];
};

int main(int argc, char** argv) {
    if (rte_eal_init(argc, argv) < 0) {
        std::cerr << "Failed to init EAL\n";
        return -1;
    }

    rte_ring* ring = rte_ring_lookup(RING_NAME);
    if (!ring) {
        std::cerr << "Ring not found (did you run producer first?)\n";
        return -1;
    }

    while (true) {
        MyStruct* obj = nullptr;
        if (rte_ring_dequeue(ring, (void**)&obj) == 0) {
            std::cout << "Dequeued: " << obj->name << " (id=" << obj->id << ")\n";
            rte_free(obj);
        } else {
            rte_delay_ms(100);
        }
    }
}
