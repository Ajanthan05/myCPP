#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include <queue>

#include <sstream> //stringstream
#include <condition_variable>
using namespace std;
#include <future>

#include <type_traits>  // Trivial and Trivially Copyable – Clarification


/*  Atomic operation is an operation that is guaranteed to eexecute
as single translation

Other threads will see the state of the system before the operation start
or after it finished. But cannot see any intermediate state.

At the low level Atomic operations are hardware instructions
(hardware gurentee atomicity)

INCREMENT
    - read x from memory
    - add 1 to x
    - write new x to memory

Eead-Modify-Write is not atomic
This is a datarace (Undefine behaviour)
*/

void t() {
    std::atomic<int> x = 5;
    int expected = 5;
    x.compare_exchange_strong(expected, 10);
    /*  ✅ If x == expected, it sets x = 10 and returns true.
    ❌ If x != expected, it updates expected = x and returns false.*/


// Example: Lock-Free Increment:  Thread-safe increment without locks.
// If another thread changes counter, CAS fails and retries.
}


std::atomic<int> counter = 0;

void increment() {
    int expected;
    do {
        expected = counter.load();
    } while (!counter.compare_exchange_strong(expected, expected + 1));
}
/*  
compare_exchange_weak()	Faster, may fail spuriously (use in loops)
compare_exchange_strong()	Safer, doesn’t fail spuriously*/



// Lock-free queues, stacks, linked lists, and reference counters.

// Compare-and-Swap (CAS)
/*  std::lock_guard<std::mutex> lock(mtx);
But:

Locks cause blocking
Risk of deadlocks
Performance bottlenecks under contention
Lock-free linked lists solve these using non-blocking algorithms.*/

struct Node {
    int value;
    std::atomic<Node*> next;

    Node(int val) : value(val), next(nullptr) {}
};

/*  not memory reclamation yet*/
class LockFreeList {
    std::atomic<Node*> head;

public:
    LockFreeList() : head(nullptr) {}

    void insert(int value) {
        Node* new_node = new Node(value);
        Node* old_head;

        do {
            old_head = head.load();
            new_node->next.store(old_head);
            std::this_thread::sleep_for(std::chrono::microseconds(rand() % 100));

        } while (!head.compare_exchange_weak(old_head, new_node));
    }

    void insert_back_(int value) {
        Node* new_node = new Node(value);
        new_node->next.store(nullptr);  // explicitly set next to null

        while (true) {
            Node* curr = head.load();

            // Case 1: list is empty
            if (!curr) {
                if (head.compare_exchange_weak(curr, new_node))
                    return;
                continue;
            }

            // Case 2: traverse to tail
            Node* prev = nullptr;
            while (curr) {
                prev = curr;
                Node* next = curr->next.load();
                if (!next) break;  // found last node
                curr = next;
            }

            // Try to link new_node at the end
            Node* expected = nullptr;
            if (prev->next.compare_exchange_weak(expected, new_node)) {
                return;
            }

            // If another thread inserted at the tail first, retry
        }
    }

    bool find(int value) {
        Node* curr = head.load();
        while (curr) {
            if (curr->value == value)
                return true;
            curr = curr->next.load();
        }
        return false;
    }

    void print() {
        Node* curr = head.load();
        while (curr) {
            std::cout << curr->value << " -> ";
            curr = curr->next.load();
        }
        std::cout << "null\n";
    }

    // No memory reclamation implemented here
    ~LockFreeList() {
        Node* curr = head.load();
        while (curr) {
            Node* next = curr->next.load();
            delete curr;
            curr = next;
        }
    }
};

void insert_values(LockFreeList& list, int base) {
    for (int i = 0; i < 10; ++i) {
        list.insert(base + i);
    }
}
/*  109 -> 108 -> 107 -> 106 -> 105 -> 104 -> 103 -> 102 -> 101 -> 100 -> 9 -> 8 -> 7 -> 6 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0 -> null
Find 5: 1
Find 200: 0*/

void insert_back(LockFreeList& list, int base) {
    for (int i = 0; i < 10; ++i) {
        list.insert_back_(base + i);
    }
}

void T_LL() {
    LockFreeList list;

    std::thread t1(insert_back, std::ref(list), 0);
    std::thread t2(insert_back, std::ref(list), 100);
    t1.join();
    t2.join();

    list.print();

    std::cout << "Find 5: " << list.find(5) << "\n";
    std::cout << "Find 200: " << list.find(200) << "\n";
}


/*  🔸 Delete Example (Using Marked Pointers)
To delete safely in concurrent environments, mark a node as logically deleted first, then remove it physically later.

You often:

Use pointer marking to indicate logical deletion (e.g., use LSB of a pointer as a flag).

Use hazard pointers or reference counting to safely free memory.

This is needed to avoid race conditions where one thread deletes a node while another is still accessing it.

🔸 Challenges
Memory reclamation: Can’t safely free memory unless you're sure no thread is using it.

Solutions: Hazard pointers, RCU, or epoch-based GC.

ABA Problem: CAS may succeed even though the value changed in between.

Solution: use version tagging or pointer + counter.

    Memory Reclamation:-	Requires special techniques to avoid use-after-free bugs
    ABA Problem:-       	Solved via pointer tagging or hazard pointers

*/

// Memory Reclamation in Lock-Free Data Structures
/*  When using lock-free data structures (like linked lists, queues, or stacks), you can't 
immediately free memory after a node is logically removed — because another thread might still be accessing it.

✅ Safe Memory Reclamation Techniques
Here are the most common solutions:

🔸 1. Hazard Pointers
Each thread declares which pointer it’s currently accessing (called a hazard pointer).

Before freeing memory, other threads' hazard pointers are checked.

Only non-hazardous nodes are freed.

✅ Pros:
Precise and safe.

❌ Cons:
More complex implementation.

Slight performance overhead.

🔸 2. Epoch-Based Reclamation
Time is divided into epochs (logical time periods).

Threads declare which epoch they’re working in.

Nodes removed in epoch n can be freed once no thread is in epoch ≤ n.

✅ Pros:
Easy to implement.

Great performance under load.

❌ Cons:
Needs coordination of epoch advancement.

Latency in reclaiming memory.

🔸 3. Reference Counting
Each node has a counter.

Increment when pointer is shared, decrement when done.

Free only when counter reaches zero.

✅ Pros:
Easy to understand.

❌ Cons:
Performance hit due to frequent atomic operations.

Can cause memory leaks with cycles.*/


/*  Atomic operation is an operation that is guaranteed to
execute as a single transaction:
– Other threads will see the state of the system before the
operation started or after it finished, but cannot see any
intermediate state
– At the low level, atomic operations are special hardware
instructions (hardware guarantees atomicity)
– This is a general concept, not limited to hardware
instructions (example: database transactions)

Any trivially copyable type can be made atomic
*** What is trivially copyable?
– Continuous chunk of memory
– Copying the object means copying all bits (memcpy)
– No virtual functions, noexcept constructor
std::atomic<int> i; // OK
std::atomic<double> x; // OK
struct S { long x; long y; };
std::atomic<S> s; // OK
*/


struct A {
    int x;
    float y;
};
struct B {
    virtual void foo() {}  // NOT trivially copyable
};
/*  Trivial constructor: a constructor that does nothing (compiler-provided, no user-defined body, and no base/virtual classes doing complex logic).
What Disqualifies a Type?
Having virtual functions

Having a non-trivial destructor

Having non-trivial copy/move constructors

Having base classes or members that are not trivially copyable*/
void Trivial_and_Trivially_Copyable_Clarification() {
    std::cout << std::boolalpha;
    std::cout << "A is trivially copyable: " << std::is_trivially_copyable_v<A> << "\n";  // true
    std::cout << "B is trivially copyable: " << std::is_trivially_copyable_v<B> << "\n";  // false
}

// — Uniform Initialization
std::atomic<int> x{0}; 
// This uses direct initialization via brace-init-list (C++11 and beyond)
/*  Internally calls std::atomic(int desired) constructor.

This constructor is marked explicit, so only direct initialization (not copy-init) works.

❌ std::atomic<int> x = 0; — Copy Initialization
But std::atomic<T> is non-copyable and non-movable (both copy and move constructors are deleted).

So this fails with a compilation error.
Syntax	Works?	Reason
std::atomic<int> x(0);	✅	Calls explicit constructor


++x;         // Atomic pre-increment
x++;         // Atomic post-increment
x += 1;      // Atomic fetch_add
x |= 2;      // Atomic fetch_or (bit set)
All of the above are atomic read-modify-write operations — they map to hardware atomic instructions (e.g., LOCK ADD on x86).

❌ Non-Atomic Expressions
These look atomic, but they're not fully atomic:
x = x + 1;
x = x * 2;
These are two separate operations:

Atomic read of x

Then normal arithmetic

Then atomic write to x

Problem: Between the read and write, another thread might have modified x. This introduces a race condition.

std::atomic<int> x{0};
++x; // Atomic pre-increment
x++; // Atomic post-increment
x += 1; // Atomic increment
x |= 2; // Atomic bit set
x *= 2; // No atomic multiplication!
int y = x * 2; // Atomic read of x
x = y + 1; // Atomic write of x
x = x + 1; // Atomic read followed by atomic write!
x = x * 2; // Atomic read followed by atomic write!

Increment and decrement for raw pointers
 Addition, subtraction, and bitwise logic operations for
integers (++, +=, –, -=, |=, &=, ^=)
 std::atomic<bool> is valid, no special operations
 std::atomic<double> is valid, no special operations
– No atomic increment for floating-point numbers!
*/

class Spinlock {
    std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) {
            // busy wait
        }
    }
    
    void unlock() {
        locked.clear(std::memory_order_release);
    }
};
// Or with exchange:
std::atomic<bool> locked{false};

void lock() {
    while (locked.exchange(true)) {
        // someone else already had it
    }
}

/*  std::atomic_flag — The Simplest Atomic Lock Primitive
std::atomic_flag is the only atomic type guaranteed to be lock-free on all platforms. It’s specifically designed for use cases like spinlocks, and:

It supports just two operations:

test_and_set()

clear()

No copy/move, no load/store — just binary state.

🔐 Why Still Use atomic_flag for Spinlocks?
1. Minimal Overhead
atomic_flag is the smallest and fastest atomic type.

Ideal for spinlocks and other low-level primitives where you just care about “locked” vs “unlocked”.

2. Portable and Guaranteed Lock-Free
std::atomic_flag locked = ATOMIC_FLAG_INIT;
Works across all standard-compliant compilers and platforms

Lock-free guarantee makes it safe for tight loops (like spinlocks)

    * test_and_set() sets the flag to true and returns the previous value
    * If it was already true, another thread has the lock → spin
    * clear() releases it

Why Use Memory Ordering?
memory_order_acquire ensures:   All reads/writes after the lock stay after the lock

memory_order_release ensures:   All writes before the unlock are visible before the unlock

There a Modern Alternative?
Yes — C++ now has:

std::mutex (high-level, blocks thread — preferred in general)

std::atomic<bool> (for simple cases)

std::scoped_lock, std::lock_guard (RAII style locking)

But atomic_flag spinlocks are still used when:
| Use Case                                | Use atomic\_flag spinlock? |
| --------------------------------------- | -------------------------- |
| Low-level systems programming           | ✅ Yes                      |
| Embedded, no standard threading support | ✅ Yes                      |
| Lock-free data structures               | ✅ Sometimes                |
| General-purpose app logic               | ❌ Prefer `std::mutex`      |

*/

void T() {
    std::atomic<int> flag{0};

    // Thread 1
    int old = flag.exchange(1);  // Sets flag = 1, gets old value
    /*  T z = x.exchange(y);
This does two operations atomically:

z = x.load(); → Read the current value of x

x.store(y); → Replace it with y

But both happen as a single atomic operation — no thread can intervene in between.*/
}

// Simple CAS Loop
std::atomic<int> counter = 0;

void increment() {
    int expected, desired;
    do {
        expected = counter.load();
        desired = expected + 1;
    } while (!counter.compare_exchange_weak(expected, desired));
}
/*  When to Use compare_exchange?
Implementing lock-free data structures
Writing custom spinlocks
Ensuring safe updates in concurrent environments
Avoiding mutex or lock_guard when performance matters

| Property                         | `compare_exchange_weak` | `compare_exchange_strong` |
| -------------------------------- | ----------------------- | ------------------------- |
| May fail spuriously              | ✅ Yes                   | ❌ No                      |
| More efficient on some platforms | ✅                       | ❌                         |
| Use in loops                     | ✅ Recommended           | ✅ Optional                |
*/

// Would you like help writing a custom atomic multiply using CAS (compare_exchange)?
// Would you like to see a std::atomic<bool> version of the spinlock or a scoped version with RAII?
int main() {

    T_LL();
    return 0;
}