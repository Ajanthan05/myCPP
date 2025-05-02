/*  https://chatgpt.com/c/680f5b3f-2370-8000-8e5c-8ae667a2e789
Perfect — you are asking about **real advanced C++ concurrency**.  
Let’s explain this **step-by-step**, very clear:

---

# 🚀 First: Quick Recap of
### `std::future`, `std::promise`, `std::packaged_task`

| Concept | What it does |
|:---|:---|
| `std::future<T>` | Represents a result **not ready yet**, to be available **later**. |
| `std::promise<T>` | A **manual producer** of a `future`. You set the value from somewhere else. |
| `std::packaged_task<T>` | Wraps a **function** and produces a `future` for its result. |

They all give you **asynchronous** results.

---

# 🧩 Example of all three:

```cpp
#include <future>
#include <iostream>
#include <thread>

int compute() { return 42; }

int main() {
    // promise & future
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t([&]() {
        prom.set_value(compute());
    });

    std::cout << "Result from promise: " << fut.get() << "\n";
    t.join();

    // packaged_task
    std::packaged_task<int()> task(compute);
    std::future<int> fut2 = task.get_future();

    std::thread t2(std::move(task)); // task runs in thread
    std::cout << "Result from packaged_task: " << fut2.get() << "\n";
    t2.join();
}
```

✅ Both give you a `future` you can wait on.

---

# 🎯 The **Problem** with basic C++ futures:

- Futures are **one-shot** (you can't combine many easily).
- No easy `.then()`, `.when_all()`, `.when_any()` in C++11/14/17/20.
- You had to **manually chain** things — messy code.

---

# ⚡ Now: C++23 introduces **nice combinators**:

✅ **`std::when_all()`**  
✅ **`std::when_any()`**  
✅ **`.then()` on future**  

→ making **future composition** easy.

---

# 🧩 Basic Concept

| Function | Purpose |
|:---|:---|
| `when_all(futures...)` | Wait until **all** futures are ready |
| `when_any(futures...)` | Wait until **any one** future is ready |
| `.then(func)` | Attach a **continuation** — run `func` when future is ready |

---

# ✨ C++23 `when_all` simple example:

```cpp
#include <future>
#include <iostream>
#include <vector>

int async_add(int x, int y) {
    return x + y;
}

int main() {
    auto f1 = std::async(std::launch::async, async_add, 1, 2);
    auto f2 = std::async(std::launch::async, async_add, 3, 4);
    auto f3 = std::async(std::launch::async, async_add, 5, 6);

    auto all = std::when_all(std::move(f1), std::move(f2), std::move(f3));

    auto final_result = all.then([](auto all_futures) {
        auto [f1, f2, f3] = all_futures.get(); // destructure tuple
        return f1.get() + f2.get() + f3.get();
    });

    std::cout << "Sum of all results: " << final_result.get() << "\n";
}
```

**✅ Output:**
```plaintext
Sum of all results: 21
```

---

# 🔥 Diagram of `when_all()`

```plaintext
Task1  ----+
Task2  ----+---> when_all() --> then() --> final processing
Task3  ----+
```

All 3 finish ➔ `then()` continues.

---

# ✨ C++23 `when_any` example:

```cpp
auto f1 = std::async(std::launch::async, compute1);
auto f2 = std::async(std::launch::async, compute2);

auto any = std::when_any(std::move(f1), std::move(f2));

auto result = any.then([](auto any_future) {
    auto winner = any_future.get();
    return winner.get(); // result of the first finished task
});
```

✅ **whichever task finishes first**, you use its result!

---

# ✨ `then()` explained clean:

After `future` becomes ready, `.then()` **attaches a continuation function**.

```cpp
auto f = std::async(std::launch::async, []() { return 5; });

auto f2 = f.then([](std::future<int> fut) {
    return fut.get() * 2;
});

std::cout << f2.get(); // prints 10
```

---

# ⚙️ Important:
- `then()` returns a **new future** for the result of your continuation.
- `when_all` returns a future with a **tuple of all futures** inside.
- All C++23 future compositions are **non-blocking** until you call `.get()`.

---

# 🏆 Quick Summary table:

| Concept | Purpose | Style |
|:---|:---|:---|
| `std::future` | Wait for 1 async result | Simple, basic |
| `std::promise` | Manually set a future result | Fine-grained control |
| `std::packaged_task` | Wrap a function for future use | Useful for queues |
| `when_all()` | Wait for **all** tasks | Parallel join |
| `when_any()` | Wait for **any** task | Fastest winner |
| `.then()` | Attach follow-up logic | Cleaner chaining |

---

# 🎯 Mini project idea?

Would you like me to show you:
- **Build a simple task graph** using `then()`
- or a **multi-future combiner** example (parallel downloads)?

🚀 Let’s go bigger if you want — it’s really fun with C++23!  
Which one you want?
*/