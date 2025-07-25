#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <map>
#include <string>
#include <condition_variable>
#include <chrono>

#include <semaphore>
#include <latch>
#include <barrier>
// #include <latch>
#include <optional>
#include <shared_mutex>
using namespace std;

template <typename T>
class LockFreeStack {
private:
    struct Node {
        std::shared_ptr<T> data;
        Node* next;
        Node(T const& value) : data(std::make_shared<T>(value)), next(nullptr) {}
    };

    std::atomic<Node*> head;

public:
    void push(T const& value) {
        Node* new_node = new Node(value);
        new_node->next = head.load();

        while (!head.compare_exchange_weak(new_node->next, new_node)) {
            // retry until successful
        }
    }

    std::shared_ptr<T> pop() {
        Node* old_head = head.load();

        while (old_head && !head.compare_exchange_weak(old_head, old_head->next)) {
            // retry until successful
        }

        return old_head ? old_head->data : std::shared_ptr<T>();
    }
};

/*  ⚠️ Common Pitfalls
ABA Problem: A value changes from A → B → A again, and compare_exchange can be tricked.

Fix: Use tagged pointers or std::atomic<std::uintptr_t>.

Memory Reclamation: How to safely delete nodes when multiple threads may access them.

Solutions: Hazard pointers, epoch-based reclamation.*/

/*  
    std::atomic<int> counter{0};
counter.fetch_add(1);  // Atomically increments
Wait-free	Every thread makes progress in a bounded number of steps. (Strongest)
Lock-free	Some thread makes progress (system-wide progress guaranteed).
Obstruction-free	Progress is guaranteed only if a thread executes in isolation.

In C++ (<atomic>):

compare_exchange_strong(expected, desired)

compare_exchange_weak(expected, desired)

✅ compare_exchange_strong
More reliable (less likely to fail spuriously).

Used in loops that don’t tolerate unnecessary retries.

⚠️ compare_exchange_weak
May fail spuriously (even if expected == actual).

Preferred in tight loops (usually slightly faster on some architectures).
*/

void T() {
    std::atomic<int> val{10};
    int expected = 15;
    bool result = val.compare_exchange_strong(expected, 20);
    cout <<"expected: " << expected << " Val: " << val << " result: " << result << "\n"; // expected: 10 Val: 10 result: 0
    // If val == expected (10), then val is set to 20, and returns true.
    // Otherwise, val is not modified, expected is updated with the actual val, and returns false.
}

struct Counter {
    bool increment_if_not_zero() {
        auto current = counter.load();
        while (current > 0 && !counter.compare_exchange_weak(current, current + 1)) { }
        return current > 0;
    }

    // This is typically used to detect last reference, last user, or shutdown condition.
    bool decrement() {
        return counter.fetch_sub(1) == 1;
    }

    uint64_t read() { return counter.load(); }
    std::atomic<uint64_t> counter{1};
};

/*  The so-called “CAS loop” (compare-and-swap loop) is the bread and butter of lock-free
algorithms and data structures
• Read the current state of the data structure
• Compute the new desired state from the current state
• Commit the change only if no one else has already changed it (compare-exchange)
• If someone else changed it, try again
Daniel Anderson -- danielanderson.net
• Progress is lock free because if an operation fails to make progress (the compare-exchange
returns false) it can only be because a different operation made progress
• Progress is not wait free because a particular operation can fail the CAS loop forever because
of competing operations succeeding

*/


int main() {
    T();
}