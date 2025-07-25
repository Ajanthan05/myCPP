#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include <cassert>

/*  safe memory reclamation in a lock-free stack:

✅ Goal:
Prevent use-after-free when a node is removed from a lock-free stack.

🔧 Setup:
We’ll use:

A lock-free stack using std::atomic<Node*>.

A hazard pointer array, one per thread.

A retire list, to defer deletion until it's safe.

*/
constexpr int MAX_THREADS = 2;

struct Node {
    int value;
    Node* next;
    Node(int v) : value(v), next(nullptr) {}
};

// One hazard pointer per thread
std::atomic<Node*> hazard_pointers[MAX_THREADS];

// Lock-free stack head
std::atomic<Node*> head = nullptr;

// Retire list (per-thread)
std::vector<Node*> retired_nodes[MAX_THREADS];

// Thread-local ID
thread_local int thread_id = -1;

bool is_hazard(Node* ptr) {
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (hazard_pointers[i].load() == ptr)
            return true;
    }
    return false;
}

void try_reclaim(Node* node, int tid) {
    if (is_hazard(node)) {
        retired_nodes[tid].push_back(node);  // defer deletion
    } else {
        delete node;
    }

    // Attempt to free deferred nodes
    auto& retire_list = retired_nodes[tid];
    for (auto it = retire_list.begin(); it != retire_list.end(); ) {
        if (!is_hazard(*it)) {
            delete *it;
            it = retire_list.erase(it);
        } else {
            ++it;
        }
    }
}



/*      */
void push(int value) {
    Node* new_node = new Node(value);
    do {
        new_node->next = head.load();
    } while (!head.compare_exchange_weak(new_node->next, new_node));
}

bool pop(int& value, int tid) {
    Node* old_head;
    do {
        old_head = head.load();
        if (!old_head) return false;
        hazard_pointers[tid].store(old_head);  // Mark it as "in use"
    } while (old_head != head.load() || 
             !head.compare_exchange_weak(old_head, old_head->next));

    value = old_head->value;
    hazard_pointers[tid].store(nullptr);  // Clear hazard after use
    try_reclaim(old_head, tid);
    return true;
}

void thread_func(int tid) {
    thread_id = tid;
    for (int i = 0; i < 5; ++i) {
        push(i + tid * 100);
    }

    int value;
    while (pop(value, tid)) {
        std::cout << "[Thread " << tid << "] Popped: " << value << std::endl;
    }
}

int main() {
    std::thread t1(thread_func, 0);
    std::thread t2(thread_func, 1);
    t1.join();
    t2.join();
    return 0;
}





