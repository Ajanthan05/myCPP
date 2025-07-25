#include <stdio.h>
#include <stdint.h>

#include <rte_eal.h>
#include <rte_mempool.h>
#include <rte_lcore.h>

#define NUM_ELEMENTS 1024
#define CACHE_SIZE 256
#define OBJ_SIZE sizeof(uint64_t)
#define MEMPOOL_NAME "my_pool"

int main(int argc, char **argv) {
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        printf("Failed to init EAL\n");
        return -1;
    }

    // Create mempool
    struct rte_mempool *mp = rte_mempool_create(
        MEMPOOL_NAME,         // name
        NUM_ELEMENTS,         // number of elements
        OBJ_SIZE,             // size of each element
        CACHE_SIZE,           // per-core cache size
        0,                    // no private data
        NULL, NULL,           // no init handlers
        NULL, NULL,
        SOCKET_ID_ANY,        // any NUMA node
        0                     // flags
    );

    if (mp == NULL) {
        rte_panic("Failed to create mempool\n");
    }

    printf("Mempool '%s' created with %d elements of size %lu bytes\n",
           MEMPOOL_NAME, NUM_ELEMENTS, OBJ_SIZE);

    // Allocate an object from the pool
    void *obj;
    if (rte_mempool_get(mp, &obj) == 0) {
        *(uint64_t *)obj = 12345;
        printf("Allocated object value: %lu\n", *(uint64_t *)obj);

        // Return object to pool
        rte_mempool_put(mp, obj);
    } else {
        printf("Failed to allocate object\n");
    }

    return 0;
}
