#include <concepts>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>

#include <type_traits>
#include <ranges>
#include <cassert>
#include <cstring>
#include <memory_resource>
#include <cstdint>

#include <execution>  // execution policies
// #include <tbb/info.h>
#include <tbb/task_arena.h>
#include <latch>
#include <barrier>
#include <future>

using namespace std;

void TBB() {
    int cores = tbb::this_task_arena::max_concurrency();

    if (cores > 1) {
        std::cout << "Parallel execution available on "
                  << cores << " cores\n";
    } else {
        std::cout << "Falling back to sequential\n";
    }
}


/*  Parallel Algorithms
C++17 added support for parallel execution of standard algorithms.
This means you can run operations like std::for_each, std::sort, std::transform 
using multiple CPU threads, without manually managing threads or locks.

Execution Policies

1. std::execution::seq
    Default (sequential execution).
    Same as old algorithms.

2. std::execution::par
    Parallel execution (multiple threads).
    Order of execution is unspecified.

3. std::execution::par_unseq
    Parallel + Vectorized execution.
    Uses CPU vector instructions (SIMD).
    Maximum performance but less predictable ordering.

4. C++20 adds std::execution::unseq
    Sequential vectorized execution (no threading, but SIMD allowed).

std::for_each
std::transform

std::sort, std::stable_sort

std::reduce, std::transform_reduce

std::count, std::count_if

std::find, std::find_if

std::copy, std::move

std::min_element, std::max_element

std::all_of, std::any_of, std::none_of

🔹 C++20 Additions
    std::ranges::for_each, std::ranges::sort, etc., also support execution policies.
    std::unseq introduced for pure SIMD optimization.
*/

void Execution() {
    std::vector<int> v(1'000'000, 1);

    // Sequential
    std::for_each(std::execution::seq, v.begin(), v.end(), [](int& n) {
        n *= 2;
    });

    // Parallel
    std::for_each(std::execution::par, v.begin(), v.end(), [](int& n) {
        n *= 2;
    });

    // Parallel + Vectorized
    std::for_each(std::execution::par_unseq, v.begin(), v.end(), [](int& n) {
        n *= 2;
    });

    std::cout << "First element: " << v[0] << "\n";

// -----------------------------------------------
// 1. std::transform:  Applies a function to each element and writes result somewhere.
    std::vector<int> in = {1,2,3,4,5,6};
    std::vector<int> out(in.size());

    std::transform(std::execution::par, in.begin(), in.end(), out.begin(), 
                    [](int x) { return x*x; });

    std::for_each(out.begin(), out.end(), [](int& n) { std::cout<< n <<" ";});
    std::cout << "\n";
}
/* 2. sort is faster but not stable.
stable_sort keeps equal elements in original order.
25 - Bob
25 - Carol   (Carol stays after Bob because she was after in input)
30 - Alice
30 - Dave   */
struct Person {
    std::string name;
    int age;
};
void Stable_sort() {
    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob",   25},
        {"Carol", 25},
        {"Dave",  30}
    };

    // Sort by age
    std::stable_sort(people.begin(), people.end(),
                     [](const Person& a, const Person& b){
                         return a.age < b.age;
                     });

    for (auto& p : people) {
        std::cout << p.age << " - " << p.name << "\n";
    }
    std::cout << "\n";
}

/*  std::executionn::par  :- g++ --std=c++20 my.cpp -ltbb -pthread && ./a.out
Intel oneTBB (Threading Building Blocks)
*/

/*  🔹 std::thread (C++11)

Represents a thread of execution.

Must be joined or detached, otherwise std::terminate() will be called in destructor.

No built-in cancellation mechanism.

🔹 std::jthread (C++20)

Joining thread (the “j” stands for join).

Destructor automatically joins if still joinable.

Supports cancellation via stop_token / stop_source.

Much safer to use than std::thread.*/
void Work(std::stop_token st) {
    while (!st.stop_requested()) {
        std::cout << "Working...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "Stopped.\n";
}void T_jthread() {
    std::jthread jt(Work); // auto-join at distruction.
    std::this_thread::sleep_for(std::chrono::seconds(1));

    jt.request_stop(); // coprative cancellation
}
/*
| Feature          | `std::thread` (C++11)                   | `std::jthread` (C++20)                                                                     |
| ---------------- | --------------------------------------- | ------------------------------------------------------------------------------------------ |
| **Auto-join**    | ❌ No (must call `join()` or `detach()`)| ✅ Yes (joins on destruction)                                                              |
| **Stop request** | ❌ No                                   | ✅ Yes (`stop_token`, `stop_source`, `request_stop()`)                                     |
| **Safety**       | Risk of forgetting `join` → terminate   | Safe RAII cleanup                                                                          |
| **Constructor**  | `std::thread(f, args...)`               | `std::jthread(f, args...)` (automatically passes `std::stop_token` if function accepts it) |
🔹 When to Use
Use std::jthread whenever possible (modern C++20).
Use std::thread only if you need backward compatibility with C++11/14/17,
 or when working in environments that don’t support std::jthread.

🔹 The Problem Before C++20:
With std::thread, once a thread starts, you cannot stop it safely from outside.
You could only kill the process or use flags with shared variables → messy and unsafe.

C++20 introduced cooperative cancellation with:

std::stop_source

Owned by whoever wants to request cancellation (the controller).

Can call .request_stop().

std::stop_token

Passed to the worker task (the running thread).

Allows the task to check for stop requests (stop_requested()).

✨ Cooperative means: thread stops only if it checks the token. If the thread ignores it → nothing happens.


🔹 How It Works
std::jthread automatically provides a stop_token to the thread function if it accepts one.
You can also manually create stop_source + stop_token and share them.*/

void Work2(std::stop_token st, int id) {
    while (!st.stop_requested()) {
        std::cout << "Worker " << id << " running\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "Worker " << id << " stoped\n";
}
void stop_source_multiple_workers() {
    std::stop_source src;
    std::vector<std::jthread> pool;
    for (int i=0; i<3; ++i) {
        pool.emplace_back(Work2, src.get_token(), i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    src.request_stop(); // stop all workers
}
/*
🔹 Why it happens

std::thread is not copyable, only movable.

std::jthread is the same: movable but not copyable.

std::vector requires reallocation when capacity grows → moves all elements.

But moving a std::jthread into new memory leaves the old object in a "not running" state.

Then the old object’s destructor runs → which joins the thread prematurely.

🔹 Solutions
✅ 1. Reserve capacity upfront

If you know the number of threads:

std::vector<std::jthread> pool;
pool.reserve(N);  // prevent reallocations
for (int i = 0; i < N; ++i)
    pool.emplace_back(worker, src.get_token(), i);


This is the simplest and safest fix.

✅ 2. Use std::list or std::deque

These containers don’t invalidate/move elements on growth:

std::list<std::jthread> pool;
for (int i = 0; i < N; ++i)
    pool.emplace_back(worker, src.get_token(), i);


std::list → stable references, no reallocation.

std::deque → amortized stable storage, also safe.

✅ 3. Use std::unique_ptr<std::jthread>

Keep heap-allocated threads in a vector, so only pointers move:

std::vector<std::unique_ptr<std::jthread>> pool;
for (int i = 0; i < N; ++i)
    pool.emplace_back(std::make_unique<std::jthread>(worker, src.get_token(), i));

🔹 Which Should You Use?

If thread count is known upfront → use reserve(N).

If dynamic but not huge → use std::deque<std::jthread>.

If ownership semantics matter → use std::unique_ptr<std::jthread>.

✅ So the main rule:
Avoid reallocating containers that store std::jthread directly, or else threads might join prematurely during move.*/


using namespace std::chrono_literals;

// Worker function: runs until stop is requested
void worker3(std::stop_token st, int id) {
    while (!st.stop_requested()) {
        std::cout << "Worker " << id << " is working...\n";
        std::this_thread::sleep_for(500ms);
    }
    std::cout << "Worker " << id << " stopping gracefully.\n";
}

// Stopper function: does some task, then signals stop
void stopper3(std::stop_source source) {
    std::cout << "Stopper running...\n";
    std::this_thread::sleep_for(2s);  // simulate some condition being reached
    std::cout << "Stopper: requesting stop!\n";
    source.request_stop();
}

void T_stopper() {
    // Create a stop_source
    std::stop_source source;

    // Distribute stop_token to workers
    std::stop_token token = source.get_token();

    // Launch two workers with jthread
    std::jthread t1(worker3, token, 1);
    std::jthread t2(worker3, token, 2);

    // Run stopper in main thread
    stopper3(source);

    // Workers will automatically stop when they see stop requested
    // jthread destructor joins automatically, so we’re done

    std::cout << "All threads stopped.\n";
}


//----------------------------- CALL BACK--------------------------------------------
void blocking_task(std::stop_token st) {
    bool cancelled = false;

    // Register callback
    std::stop_callback cb(st, [&] {
        std::cout << "Cancellation requested!\n";
        cancelled = true;
    });
/*  When you construct cb, it registers itself with the stop_state that lives inside the std::stop_source / std::stop_token.

That stop_state keeps a list of callbacks.

The callback object (cb) stays linked in that list as long as it is alive.

Somewhere else, another thread (or the same thread) calls: source.request_stop();
What happens then:
The stop_source sets the stop state to "stop requested".
It immediately walks through all registered callbacks and executes them.
Each stop_callback you registered (like your lambda) is invoked synchronously inside the request_stop() call.
If multiple callbacks are registered, they all run before request_stop() returns.*/

    std::cout << "Task started, simulating blocking work...\n";
    for (int i = 0; i < 10 && !cancelled; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Working...\n";
    }
    std::cout << "Task finished or cancelled.\n";
}

void Call_back() {
    std::stop_source src;
    std::jthread t(blocking_task, src.get_token());

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Main thread: requesting stop\n";
    src.request_stop();

    // jthread auto-joins
}


//--------------------LATCHES & BARRAIER---------------------------------------------------
/*   std::latch

A latch is a one-time synchronization point:
It starts with a counter (set at construction).
Threads call count_down() to decrement the counter.
Other threads can call wait() (or arrive_and_wait()) to block until 
the counter reaches zero.
Once the latch is released (count reaches zero), it can’t be reused.

One-shot: can only release once.
Used for scenarios like “wait until N tasks have completed”. */
void worker4(int id, std::latch& ready) {
    std::cout << "Worker " << id << " starting\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Worker " << id << " finished\n";
    ready.count_down();   // signal completion
}

void Latches() {
    std::latch ready(3);  // local latch, initialized with count = 3
    std::vector<std::thread> threads;

    for (int i = 1; i <= 3; ++i)
        threads.emplace_back(worker4, i, std::ref(ready)); // pass by reference

    ready.wait(); // main thread waits until latch is released
    std::cout << "All workers finished, continuing...\n";

    for (auto& t : threads) t.join();
}

void onBarrierLifted() noexcept {
    cout << "Barrier Lifted\n";
}
int req_threads = 2;
std::barrier<void(*)(void) noexcept> b(req_threads, onBarrierLifted);
/*  Function pointer type

void(*)(void) noexcept means:

void → return type: the function returns nothing.

(*)(void) → pointer to a function that takes no arguments.

noexcept → the function guarantees it won’t throw exceptions.

So it’s a pointer to a function with signature:

void f() noexcept;*/


void act(int num) {
    cout << "Thread: " << num << "\n";

    // Wait until the thread specified in the barraier reaches this point.
    b.arrive_and_wait();

    cout << "Thread End: " << num << "\n";
}
void LachesAndBarraier() {
    std::thread t1(act,1);
    std::thread t2(act,2);

    t1.join();
    t2.join();
}


//------------------------
void Test_Barraier() {
    const int num_threads = 3;
    int phase = 0;

    std::barrier sync_point(num_threads, [&] {
        ++phase;
        std::cout << ">>> Phase " << phase << " completed by all threads\n";
    });

    auto worker = [&] (int id) {
        for (int p=1; p<=3; ++p) {
            std::cout << "Thread " << id << " working in phase " << p << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));
            sync_point.arrive_and_wait(); // Barrier wait
        }
    };

    std::vector<std::jthread> threads;
    for (int i = 1; i <= num_threads; ++i)
        threads.emplace_back(worker, i);
}

/*  🔹 1. auto arrival_token = x.arrive();

Decrements the barrier’s arrival count (i.e. “this thread has arrived”).

If this decrement makes the count reach zero, the completion phase is triggered (completion function runs once).

Returns an arrival_token, which you can later pass to wait() to block until the completion phase is finished.

👉 Think: “I’ve arrived, but I may not wait immediately.”

🔹 2. x.wait(arrival_token);

Waits until the current phase is complete (i.e. all participants have arrived and the completion function has finished).

Requires an arrival_token from a previous arrive().

👉 Think: “I arrived earlier, now I’ll wait for the others.”

🔹 3. x.arrive_and_wait();

Convenience function that combines arrive() + wait().

Most common usage.

👉 Equivalent to:

x.wait(x.arrive());

🔹 4. x.arrive_and_drop();

Decrements the expected number of participants permanently.

Example: if a thread is leaving the computation early, it tells the barrier “don’t expect me in future phases.”

If this call brings the count to zero, the completion phase is triggered immediately.

👉 Think: “I’m done with this barrier forever.”

Option 1: split arrive + wait

auto token = sync_point.arrive();
sync_point.wait(token);


Equivalent to: I’m here → now I’ll wait.
Good if you want to do something between arrival and waiting.

Option 2: single call

sync_point.arrive_and_wait();


Equivalent to: I’m here and I’ll wait right away.
This is just shorthand for Option 1.

Option 3: drop out

sync_point.arrive_and_drop();


This means: I’m done participating forever.
After this, the barrier expects one less participant in future phases.*/


/* ---------------------- std::packaged_task ---------------------------

Wraps a callable (function, lambda, etc.) so that when you call it, its result is 
stored in an associated future.

You don’t explicitly call set_value — it’s automatic when the function finishes.

packaged_task is useful when you want to schedule work (like in a thread pool).

Use when: you want a function’s result to be asynchronously available through a future.*/
void Packes_task() {
    std::packaged_task<int(int,int)> task([](int a, int b) { return a+b; });

    std::future<int> f = task.get_future();

    std::thread t(std::move(task), 2, 4);
    // std::cout << f.get() << "\n";

    t.join();
    std::cout << f.get() << "\n";  // Once you .get(), the future becomes invalid.
}
/*  std::thread t(std::move(task), 2, 3);

std::packaged_task is not copyable, only movable.

That’s why you must std::move(task) when passing it into the thread constructor.

After the move, the original task object in the calling scope is empty (moved-from state).

You cannot call it again.

If you want to run the same callable again, you’d need to create a new packaged_task.

==>>> So: after std::move(task), the task in the main thread is no longer usable.
The ownership of the packaged task is now inside the new thread.

.get()

Blocks until the value is ready.

Returns the result (or rethrows the stored exception).

Consumes the value → you can only call get() once.

Example:

auto val = f.get();   // blocks, then gives result
auto again = f.get(); // ERROR: future already consumed


.wait()

Blocks until the value is ready.

Does not return the value, just unblocks.

You can call it multiple times.

Example:

f.wait();             // block until ready
std::cout << f.get(); // now safe to consume

✅ Summary

std::move(task) → transfers the packaged task into the thread; you can’t reuse the old one.

.get() → block and take the result (one-time use).

.wait() → just block until ready (non-consuming, can be called many times).*/

/*  stdȂ::future<T> — Basic API
stdȂ::future<T> default constructor
Create an empty object with no state

f.valid() :- Check if the future has state. 
Returns false if it’s empty or already consumed (get() has been called).

f.wait() :- Wait for the data to be ready
Does not consume the result; you still call .get() later.
Often used when you just need synchronization.

f.wait_for(duration) :- Wait for the data to be ready for the specified duration
if (f.wait_for(std::chrono::seconds(2)) == std::future_status::ready) {
    std::cout << f.get();
}
Waits up to a specified time span.
Returns an enum std::future_status:
    1. ready → value available
    2. timeout → not ready in given duration
    3. deferred → task hasn’t started yet (lazy start)

f.wait_until(time_point) :- Wait for the data to be ready until the specified time
if (f.wait_until(std::chrono::steady_clock::now() + std::chrono::seconds(5))
      == std::future_status::ready) {
    std::cout << f.get();
}
Similar to wait_for, but waits until a specific clock time instead of a relative 
duration.

x.get() :- Wait for the data and retrieve it*/


//  -------------------------   pipelining  ----------------------------------------
// In C++20 ranges, you can chain range adaptors together using the | operator (pipe).
// This creates a pipeline of transformations on your data.
void PipeLining() {
    std::vector<int> v{1,2,3,4,5,6};
    std::vector<int> evens;

    std::copy_if(v.begin(), v.end(), std::back_inserter(evens), 
                [](int x) { return x%2 == 0; });

    vector<int> doubled;
    std::transform(evens.begin(), evens.end(), std::back_inserter(doubled),
                    [](int x) { return x*2; });

    for (int x : doubled) {
        std::cout << x << " ";
    }
    cout << "\n";

    for (int x : v | std::views::filter([](int x) { return x%2 == 0; })
                   | std::views::transform([](int x) { return x*2; })
                   | std::views::take(2) ) {

        std::cout << x << " ";
    }
    cout << "\n";
/*
No intermediate vectors allocated.
Each stage is lazy → computed only when iterated.

    ===>>>  Why pipelining is powerful
Composability: You can stack any adaptors (filter, transform, drop, take, reverse, etc.).
Lazy evaluation: Values are computed on demand, not stored unless you ask.
Efficiency: Avoids unnecessary copies/allocations.
Expressiveness: Code reads like a data-processing pipeline.*/
}

int main() {
    // TBB();
    // Execution();
    // T_jthread();
    // stop_source_multiple_workers();
    // T_stopper();
    // LachesAndBarraier();

    // Call_back();

    // Latches();

    // Test_Barraier();

    // Packes_task();

    PipeLining();
}