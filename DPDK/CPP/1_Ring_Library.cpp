#include <iostream>
#include <rte_eal.h>
#include <rte_ring.h>
#include <rte_malloc.h>

int main(int argc, char** argv) {
    // Initialize DPDK EAL
    if (rte_eal_init(argc, argv) < 0) {
        std::cerr << "Failed to initialize DPDK EAL\n";
        return -1;
    }

    const unsigned RING_SIZE = 1024;
    const char* RING_NAME = "my_ring";

    // Create a ring
    rte_ring* ring = rte_ring_create(RING_NAME, RING_SIZE, rte_socket_id(), RING_F_SP_ENQ | RING_F_SC_DEQ);
    if (!ring) {
        std::cerr << "Failed to create ring\n";
        return -1;
    }

    // Allocate memory for an integer
    int* data = static_cast<int*>(rte_malloc("data", sizeof(int), 0));
    *data = 42;

    // Enqueue the pointer into the ring
    if (rte_ring_enqueue(ring, data) < 0) {
        std::cerr << "Failed to enqueue\n";
        rte_free(data);  // This pointer must have been returned by a previous call to rte_malloc(), rte_zmalloc(), rte_calloc() or rte_realloc()
        return -1;
    } else {
        std::cout << "Enqueued: " << *data << std::endl;
    }

    // Dequeue the pointer from the ring
    /*  int rte_ring_dequeue(struct rte_ring *r, void **obj_p);
r is the ring buffer.
obj_p is a pointer to a pointer that will be used to store the dequeued object.*/

    int* received_data = nullptr;
    if (rte_ring_dequeue(ring, (void**)&received_data) < 0) {
        std::cerr << "Failed to dequeue\n";
        return -1;
    } else {
        std::cout << "Dequeued: " << *received_data << std::endl;
        rte_free(received_data); // Free the memory
    }

    return 0;
}
