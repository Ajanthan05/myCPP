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
/*  Trivial constructor: a constructor that does nothing (compiler-provided, 
no user-defined body, and no base/virtual classes doing complex logic).
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
std::atomic<int> x{0};  // Not x = 0;   x(0); Is Ok 
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
All of the above are atomic read-modify-write operations — they map to hardware 
atomic instructions (e.g., LOCK ADD on x86).

❌ Non-Atomic Expressions
These look atomic, but they're not fully atomic:
x = x + 1;
x = x * 2;
These are two separate operations:
    1. Atomic read of x
    2. Then normal arithmetic
    3. Then atomic write to x
Problem: Between the read and write, another thread might have modified x. 
This introduces a race condition.

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
std::atomic_flag is the only atomic type guaranteed to be lock-free on all platforms. 
It’s specifically designed for use cases like spinlocks, and:

It supports just two operations:
    1. test_and_set()   // it return previous value
    2. clear()

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
std::atomic<int> Counter = 0;

void Increment() {
    int expected, desired;
    do {
        expected = Counter.load();
        desired = expected + 1;
    } while (!Counter.compare_exchange_weak(expected, desired));
}
/*  When to Use compare_exchange?
Implementing lock-free data structures
Writing custom spinlocks
Ensuring safe updates in concurrent environments
Avoiding mutex or lock_guard when performance matters

| Property                         | `compare_exchange_weak` | `compare_exchange_strong` |
| -------------------------------- | ----------------------- | ------------------------- |
| May fail spuriously              | ✅ Yes                  | ❌ No                     |
| More efficient on some platforms | ✅                      | ❌                        |
| Use in loops                     | ✅ Recommended          | ✅ Optional               |

x.compare_exchange_strong(old_x, new_x):
if (x == old_x) { x = new_x; return true; }
else { old_x = x; return false; }
 x.compare_exchange_weak(old_x, new_x): same thing
but can “spuriously fail” and return false even if x==old_x

 What is the value of old_x if this happens? Must be old_x!
 If weak CAS correctly returns x == old_x, why would it fail?

 CAS, conceptually (pseudo-code):
bool compare_exchange_strong(T& old_v, T new_v) {
 Lock L; // Get exclusive access
 T tmp = value; // Current value of the atomic
 if (tmp != old_v) { old_v = tmp; return false; }
 value = new_v;
 return true;
}
 Lock is not a real mutex but some form of exclusive access
implemented in hardware

 If exclusive access is hard to get, let someone else try:
bool compare_exchange_weak(T& old_v, T new_v) {
 T tmp = value; // Current value of the atomic
 if (tmp != old_v) { old_v = tmp; return false; }
 TimedLock L; // Get exclusive access or fail
 if (!L.locked()) return false; // old_v is correct
 tmp = value; // value could have changed!
 if (tmp != olv_v) { old_v = tmp; return false; }
 value = new_v;
 return true;
}
 Double-checked locking pattern is back!
*/


void Other() {
    std::atomic<float> a{3.5f};
    float y = 9.5f;
    float Prev =  a.fetch_add(y); // It return old value
    cout << "Prev: " << Prev << " Cur: " << a.load() << "\n";
    /*  fetch_sub(), . fetch_and(), .fetch_or(), .fetch_xor() */
}


/*  
| Feature               | `std::lock_guard` | `std::unique_lock` |
| --------------------- | ----------------- | ------------------ |
| Automatically locks   | ✅ Yes             | ✅ Optional         |
| Can unlock manually   | ❌ No              | ✅ Yes              |
| Can be deferred       | ❌ No              | ✅ Yes              |
| Can be moved          | ❌ No              | ✅ Yes (move-only)  |
| Supports `try_lock()` | ❌ No              | ✅ Yes              |
| More overhead         | ❌ No              | ✅ Slightly more    |

*/
 
struct A_ { long x; };
struct B_ { long x; long y; };
struct C_ { long x; long y; long z; };
struct D { int x; int y; int z; };

void is_LogFree() {
    std::atomic<A_> a;
    std::cout << a.is_lock_free() << "\n";
    std::cout << std::atomic<B_>::is_always_lock_free << "\n";

/*  What does lock-free mean?
An atomic type is lock-free if it uses hardware atomic instructions (like LOCK CMPXCHG) and does not use a mutex or spinlock internally.
Lock-free ⇒ fast, low-overhead, no blocking
Not lock-free ⇒ fallback to heavier synchronization (like futex, pthread_mutex, etc.)

std::atomic<T> requires that T be trivially copyable

If T is too large or misaligned, the compiler will use internal locking

| Struct | Size (typically) | Lock-Free?                                     |
| ------ | ---------------- | ---------------------------------------------- |
| `A_`   | 8 bytes          | ✅ Often lock-free (fits in a register)        |
| `B_`   | 16 bytes         | ⚠️ Maybe lock-free (on x86-64 with CMPXCHG16B) |
| `C_`   | 24 bytes         | ❌ Usually not lock-free (too big)             |*/

    if constexpr (std::atomic<D>::is_always_lock_free) {
        std::cout << "D is always lock-free\n";
    } else {
        std::cout << "D may not be lock-free\n";
    }
/* Trivially copyable	✅ Yes (this is true for D)
Hardware supports it	✅ e.g., x86-64 with CMPXCHG16B supports 16-byte lock-free
    BUT

❌ Why D is not lock-free
12 bytes is not a common atomic width:

Most platforms support atomic operations on 1, 2, 4, 8 bytes.

Some CPUs support 16 bytes (128-bit atomics, e.g., with CMPXCHG16B).

12 bytes has no native hardware support.

Atomic types require full-width atomicity:

The CPU must be able to read/write the entire 12-byte object atomically.

No common CPU supports 96-bit atomic instructions.

Fallback to internal locks:

If the hardware doesn’t support atomic ops on that size/type, std::atomic<D> will use a mutex or spinlock internally.

This makes it not lock-free.

*/
}

// Atomic queue:
constexpr int N = 5;
int q[N];
std::atomic<size_t> front;  //  Atomic variable is an index to (non-atomic) memory
void push(int x) {
    size_t my_slot = front.fetch_add(1);
    q[my_slot] = x;
}

// ✅ Full Code: Lock-Free Stack with Tagged Pointer
struct Node_s {
    int value;
    Node_s* next;
    Node_s(int v) : value(v), next(nullptr) {}
};
/*  Then store std::atomic<TaggedPtr> head. Every time we update the head, we bump the tag so even if ptr is reused, the version mismatch prevents ABA.

Even if a Node_s* gets reused at the same memory address, the tag ensures the pointer is not seen as "unchanged".

This eliminates classic ABA issues in lock-free lists.*/

// Tagged pointer to avoid ABA problem
struct TaggedPtr {
    Node_s* ptr;
    uint64_t tag;

    bool operator==(const TaggedPtr& other) const {
        return ptr == other.ptr && tag == other.tag;
    }
};

class LockFreeStack {
    std::atomic<TaggedPtr> head;

public:
    LockFreeStack() {
        head.store({nullptr, 0});
    }

    void push(int value) {
        Node_s* new_node = new Node_s(value);
        TaggedPtr old_head = head.load(std::memory_order_acquire);
        TaggedPtr new_head;

        do {
            new_node->next = old_head.ptr;
            new_head.ptr = new_node;
            new_head.tag = old_head.tag + 1;
        } while (!head.compare_exchange_weak(
            old_head, new_head,
            std::memory_order_release,
            std::memory_order_acquire));
    }

    bool pop(int& out_value) {
        TaggedPtr old_head = head.load(std::memory_order_acquire);
        TaggedPtr new_head;

        do {
            if (!old_head.ptr)
                return false; // empty stack

            Node_s* node = old_head.ptr;
            new_head.ptr = node->next;
            new_head.tag = old_head.tag + 1;

            out_value = node->value;

        } while (!head.compare_exchange_weak(
            old_head, new_head,
            std::memory_order_release,
            std::memory_order_acquire));

        delete old_head.ptr;
        return true;
    }

    ~LockFreeStack() {
        int val;
        while (pop(val)) {}
    }
};

void Test_LockFreeStack() {
    LockFreeStack stack;

    stack.push(10);
    stack.push(20);
    stack.push(30);

    int value;
    while (stack.pop(value)) {
        std::cout << value << "\n";
    }
}



/*  What is Memory Ordering?
CPUs and compilers can reorder instructions for optimization. 
This can break correctness in multithreaded code unless properly synchronized.

C++ Atomic Memory Orders
cpp
Copy
std::memory_order_relaxed
std::memory_order_acquire
std::memory_order_release
std::memory_order_acq_rel
std::memory_order_seq_cst


memory_order_acquire ensures:   All reads/writes after the lock stay after the lock

memory_order_release ensures:   All writes before the unlock are visible before the unlock*/




/*  Low-level memory control (allocators, embedded)
You can manually align memory:

alignas(16) int buffer[4]; // align the buffer on a 16-byte boundary
Used in:
    SIMD (e.g., SSE, AVX)
    Custom allocators
    Hardware programming

    Template metaprogramming

template<typename T>
void print_alignment() {
    std::cout << alignof(T) << "\n";
}
Useful for writing portable, alignment-aware code.

⚠️ Alignment and Atomics
Atomic types require specific alignment to be lock-free.

struct alignas(16) MyAtomicData {
    long long a;
    long long b;
};
std::atomic<MyAtomicData> data; // More likely to be lock-free if aligned
If the alignment is wrong, even if the size is supported (like 16 bytes), atomic operations may fall back to locks.

*/



template<typename T>
class RingBuffer {
public:
    explicit RingBuffer(size_t size) : buffer(size), head(0), tail(0), count(0) {}

    void push(const T& item) {
        std::unique_lock<std::mutex> lock(mtx);
        not_full.wait(lock, [this]() { return count < buffer.size(); });

        buffer[head] = item;
        head = (head + 1) % buffer.size();
        ++count;

        lock.unlock();
        not_empty.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [this]() { return count > 0; });

        T item = buffer[tail];
        tail = (tail + 1) % buffer.size();
        --count;

        lock.unlock();
        not_full.notify_one();
        return item;
    }

private:
    std::vector<T> buffer;
    size_t head, tail, count;
    std::mutex mtx;
    std::condition_variable not_empty, not_full;
};






/*  Spin-wait loop used for thread synchronization without blocking, and here's 
exactly what it does and when you might use it (and when not to).

while (!done) std::this_thread::yield();
🔷 Explanation
done:
A shared flag, usually volatile or std::atomic<bool>, that indicates a task is complete.

std::this_thread::yield();
Tells the CPU:

"I'm busy-waiting. Let another thread run now."

This avoids hogging the CPU while still staying in a tight loop.

Without yield(), the loop becomes:
while (!done); // BAD: tight spin loop — burns CPU

✅ When to Use Spin-Wait + yield()
When wait time is expected to be short (microseconds or milliseconds).
You want low latency: as soon as done becomes true, you're ready.
You cannot afford blocking overhead (like mutex or condition_variable).
E.g., lock-free queues, real-time control loops, some embedded scenarios.

⚠️ When Not to Use It
If the wait time could be longer (hundreds of ms or seconds) → wastes CPU.
If many threads are waiting like this → bad for scalability.
Better alternatives in those cases:
    std::condition_variable
    std::future::get() (waits automatically)
    std::this_thread::sleep_for(...) (adds delay between checks)


*/

std::atomic<bool> done = false;
void worker() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    done = true;
}
void Spin_Wait() {
    std::thread t(worker);

    // Spin-wait
    while (!done) std::this_thread::yield();

    std::cout << "Worker finished!\n";
    t.join();
}

/*
All attomic operations
    1.  Nither COPY ASSIGNABLE nor COPY CONSTRUCTABLE
    2. Can assign non atomic booleans and can be constructed using non atomic booleans
*/
void atomicBool() {
    std::atomic<bool> flag_1;
    cout << "Flag1: " << flag_1 << "\n";

    // std::atomic<bool> flag_2(flag_1);
    // std::atomic<bool> flag_3 = flag_1;

    // Construct using non atomic boolean value
    bool non_atomic_bool = true;
    std::atomic<bool> flag_4(non_atomic_bool);
    cout << "Flag4: " << flag_4 << "\n";

    std::atomic<bool> flag_5 = non_atomic_bool;
    cout << "Flag5: " << flag_5 << "\n";
}


/*  You’re using std::atomic<TaggedPtr> where TaggedPtr is 16 bytes (128 bits).

On most x86_64 systems, hardware only supports lock-free atomics up to 8 bytes.

When the compiler sees a request for 16-byte atomic operations, it emits calls to GCC’s libatomic helpers (like __atomic_store_16, __atomic_load_16).

If libatomic is not linked, you get undefined reference at link time.

If TaggedPtr can be made ≤ 8 bytes, hardware can usually handle it lock-free without libatomic.

Example:

cpp
Copy
Edit
struct TaggedPtr {
    void* ptr;
    uint32_t tag;
    // This will usually be 12 bytes + padding → try packing
} __attribute__((packed));
3️⃣ Use a mutex for large atomics
If 128-bit lock-free operations aren’t essential:

cpp
Copy
Edit
#include <mutex>

struct SafeTaggedPtr {
    TaggedPtr value;
    std::mutex m;
    void store(TaggedPtr v) {
        std::lock_guard lg(m);
        value = v;
    }
    TaggedPtr load() {
        std::lock_guard lg(m);
        return value;
    }
};
🔍 Why __atomic_store_16 exists
It’s part of GCC’s __atomic builtins for 16-byte atomic operations, used when:

Target CPU supports it (like some ARMv8, POWER, or x86 with CMPXCHG16B)

Or GCC emulates it via libatomic

If your CPU lacks native 128-bit atomics but you want them, GCC falls back to library calls.

*/
// is_always_log_free
// Would you like help writing a custom atomic multiply using CAS (compare_exchange)?
// Would you like to see a std::atomic<bool> version of the spinlock or a scoped version with RAII?
int main() {

    T_LL();

    Other();

    is_LogFree();

    atomicBool();
    return 0;
}