#include <iostream>
#include <shared_mutex>
#include <string>
#include <vector>

// guarded from above (assume included)
#include "Guarded_API.h"

int main() {
    guarded<std::vector<int>, std::shared_timed_mutex> gvec;

    // writer
    {
        auto h = gvec.lock();
        if (h) h->push_back(100);
    } // lock released here

    // reader (shared)
    {
        auto sh = gvec.lock_shared();
        if (sh) {
            std::cout << "size: " << sh->size() << '\n';
        }
    }

    // try timed exclusive lock (only available because we used shared_timed_mutex)
    using namespace std::chrono_literals;
    {
        auto th = gvec.try_lock_for(10ms);
        if (th) th->push_back(200);
        else std::cout << "couldn't get exclusive lock\n";
    }
}
