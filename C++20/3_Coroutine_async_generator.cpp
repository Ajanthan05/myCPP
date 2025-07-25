#include <iostream>
#include <coroutine>
#include <optional>

using namespace std;

/*      std::optional?
std::optional<T> is a wrapper that may contain a value of type T, or may be empty (no value).
It's a safer alternative to returning raw pointers, nullptr, or magic values (like -1) to indicate "nothing" or failure.*/


std::optional<int> find_even(int x) {
    if (x % 2 == 0)
        return x;
    return std::nullopt;  // No value
}

std::optional<std::string> Find_Name(bool t) {
    if (t) return "Ajanthan";
    else return std::nullopt;
}

void T_OPTIONAL() {
    auto result = find_even(4);
    if (result)
        std::cout << "Found: " << result.value() << '\n'; // OR *result
    else
        std::cout << "Not found\n";

    /*  
has_value() or operator bool()	Checks if value exists
value() or *opt	                Access the value (undefined behavior if no value!)
value_or(default)	            Safe access with fallback
Comparison operators	        You can compare optionals
Monadic-like operations	        You can use it with transform, and_then, etc. in C++23  ???

Why use it?
    1. Avoid returning raw pointers or sentinel values.
    2. Makes your API's intent clear — “might return nothing”.
    3. Cleaner, safer error handling in function return values.*/

    auto name = Find_Name(false).value_or("Unknown");
    cout << "name: " << name << "\n";
}
/*  Don't do this (unless you're sure)
auto x = opt.value(); // throws std::bad_optional_access if no value
Instead:
if (opt) {
    auto x = *opt; // safe
}

✅ When to Use std::optional
Return value may be missing, but not an error (e.g., search functions).

Instead of output parameters (use return value).

Caching/memoization (store computed values).

Lazy initialization.

🧪 C++23 Additions
transform() – apply function if value exists

and_then() – chaining calls

or_else() – provide alternative
opt.transform([](int x){ return x + 1; });
*/
void RESET() {
    std::optional<int> opt = 42;

    opt = std::nullopt;  // Clears the optional (destructs the int inside)

    opt.emplace(100);    // Put new value in
    opt.reset();         // Also clears (destructs the int again)
/*  Set optional to empty	    opt = std::nullopt;
    Clear optional	            opt.reset();

Both are correct — it's mostly a matter of style and readability.*/
}

struct MoveOnly {
    std::unique_ptr<int> ptr;

    MoveOnly(int val) : ptr(std::make_unique<int>(val)) {}
    MoveOnly(MoveOnly&&) = default;       // ✅ move ctor
    MoveOnly& operator=(MoveOnly&&) = default; // ✅ move assign
    MoveOnly(const MoveOnly&) = delete;   // ❌ no copy
    MoveOnly& operator=(const MoveOnly&) = delete;

    void print() const {
        std::cout << "Value = " << *ptr << "\n";
    }
};

void T_MoveOnly() {
    std::optional<MoveOnly> o1{MoveOnly{42}}; // ok
    std::optional<MoveOnly> o2 = std::move(o1); // move ok

    if (o2) o2->print();

    // std::optional<MoveOnly> o3 = o2; // ❌ error: copy ctor deleted

    std::optional<std::string> a{"hello"};
    std::optional<std::string> b = a; // ✅ copied
    std::optional<std::string> c = std::move(a); // ✅ moved

    std::cout << "b: " << *b << "\n";
    std::cout << "c: " << *c << "\n";
}
// 🧠 std::optional<MoveOnly> is move-only because MoveOnly is move-only.













// A simple generator that yields integers
template<typename T>
struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        std::optional<T> current_value;

        auto get_return_object() {
            return Generator{ handle_type::from_promise(*this) };
        }

        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }

        void return_void() {}

        void unhandled_exception() {
            std::exit(1);
        }
    };

    handle_type coro;

    Generator(handle_type h) : coro(h) {}
    ~Generator() { coro.destroy(); }

    T next() {
        coro.resume();
        return *coro.promise().current_value;
    }

    bool done() {
        return coro.done();
    }
};

// Coroutine function
Generator<int> counter(int max) {
    for (int i = 0; i < max; ++i) {
        co_yield i;  // suspend here and return i
    }
}

int main() {
    T_OPTIONAL();

    auto gen = counter(5);

    while (!gen.done()) {
        std::cout << "Value: " << gen.next() << "\n";
    }

    return 0;
}
