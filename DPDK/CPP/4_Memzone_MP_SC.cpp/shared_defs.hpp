// shared_defs.hpp
#ifndef SHARED_DEFS_HPP
#define SHARED_DEFS_HPP

#include <rte_ring.h>
#include <rte_memzone.h>

#define RING_NAME      "my_mp_ring"
#define MEMZONE_NAME   "my_mp_memzone"
#define MEMZONE_COUNT  64

struct MyStruct {
    int id;
    char name[32];
};

#endif // SHARED_DEFS_HPP
