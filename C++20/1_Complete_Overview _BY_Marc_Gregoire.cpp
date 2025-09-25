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

#include <algorithm>
#include <ranges>
#include <type_traits>  // decay_t

using namespace std;

/*  New keywords:
concept
requires
constinit
consteval
co_await
co_return
co_yield
char8_t

New identifiers:
import
module*/

//////////////////  COROUTINES  //////////////////////



//----------------------- LAMBDA-------------------------------
/*  [captures] (parameters) specifiers requires-clause { body }
SPECIFIERS -> mutable, constexpr, noexcept

Generic Lambdas (since C++14)
auto f = [](auto x) { return x + 1; };
Here auto makes the parameter generic → like a template function.
Equivalent to:
struct {
    template<typename T>
    auto operator()(T x) const { return x + 1; }
} f;    */

// Explicit Template Parameters in Lambdas (new in C++20)
// This is much clearer than relying on auto everywhere.
auto f = [] <typename T> (T x) {
    return x + 1;
};
// Multiple Template Parameters
auto g = [] <typename T, typename U> (T a, U b) {
    return a + b;
};
// MON-TYPE Template Parameters
auto h = [] <typename T, int N> (T (&arr)[N]) {
    for (int i=0; i<N; ++i) {
        cout << arr[i] << " ";
    }
    cout << "\n";
};
void ExplicitLambda() {
    cout << f(5) << "\n";
    cout << f(3.14) << "\n";

    cout << g(5, 3.14) << "\n";  // T=int, U=double → result double
    
    int a[3] = {1,2,3};
    h(a); // N=3

    // Why This Is Useful
        // Local templated functions without polluting namespace.
        // Pass templated lambdas to algorithms
    std::vector<int> v{1,2,3};
    std::ranges::for_each(v, [] <typename T> (T x) {
        std::cout << x << " ";
    });
    cout << "\n";

    std::variant<int, std::string> var = "Hello";
    std::visit([] <typename T> (T&& val) {
        cout << val << "\n";
    }, var); 
}
/*  Comparison to Normal Function Templates
Normally you’d write:

template <typename T>
void foo(T x) { // ...  }

Now you can do the same inline without naming a function:

auto foo = [] <typename T> (T x) { /* ... / };

In pre-C++20, this was also captured implicitly by [=].
In C++20, you must say it explicitly if you want this.*/
struct Foo {
    int x = 10;
    void run() {
        int y = 20;
        auto f = [=]() { 
            // x captured via `this`, y by copy (C++17)
            // But in C++20: `this` NOT captured unless written
            // std::cout << x << " " << y << "\n"; 
    // In C++20, the above code won’t compile unless you add this to the capture list.
        };
        f();
    }
};

/*  this] — Capture the this pointer
Gives the lambda access to all member variables of the current object.

***=> But notice: this captures the pointer (this), not the object.
If the object dies, the lambda still holds a dangling pointer → dangerous 
in async/threading.*/

/*  [*this] — Capture the object by value (new in C++20)
This is a deep copy of the whole object into the lambda closure.
Much safer in multithreaded/async scenarios.

[this] would make the lambda reference the original Foo, which might be 
destroyed while the thread runs → UB.
[*this] copies Foo into the lambda, so it’s safe.
*/
struct Foo2 {
    int x = 10;
    void run_async() {
        auto f = [*this]() {
            std::cout << x << "\n";  // works on copied Foo
        };
        std::jthread t(f); // safe, object copied into lambda
    }
};
/*
[=] → copy locals (not this in C++20).

[&] → ref locals (not this in C++20).

[this] → capture the pointer, can dangle.

[*this] → capture object by value (copy). Safe for async.

[=, this] / [&, this] → mix of old behavior + explicitness.

[=, this]   : If you want the old behavior of pre-C++20 [=] 
(all locals by copy, this included), you must now write

[&, this]   :  all locals by reference plus this    */

/*  Thread inside struct
struct/class that starts a thread as part of its behavior. 
This is common for objects that run background tasks.*/
struct Worker {
    int value = 42;
    std::thread t;

    Worker() {
        // Start a thread in the constructor
        t = std::thread([this] {
            std::cout << "Worker running with value = " << value << "\n";
        });
    }

    ~Worker() {
        if (t.joinable()) t.join();
    }
};

void Thread_inside_struct() {
    Worker w;
}

void RANGES_20() {
    std::vector<int> v{1,2,3};

    std::ranges::for_each(v, [](int x){
        std::cout << x << " ";
    });

    cout << "\n";
// Can take a range directly (v) instead of needing begin/end.
// Still works with iterators if you want:

    std::ranges::for_each(v.begin(), v.end(), [](int x){ std::cout << x << " "; });

//--------------------------------------
// res.in → the end iterator 
// res.fun → the function object
    auto res = std::ranges::for_each(v, [](int& x){ x *= 2; });

    std::cout << "Stopped at? " << (res.in == v.end() ? "end" : "not end") << "\n";
    res.fun; // the lambda is stored here

    /*  Why is this useful?

✅ You get the end iterator back (res.in).

This matters if the algorithm can stop early (e.g. projections, sentinels, 
partial ranges).

In normal std::for_each, you can’t tell if it stopped early.

✅ You still get the function object (res.fun).

Useful if your lambda has state (e.g. counting something inside a mutable lambda).*/
    int sum = 0;
    auto res2 = std::ranges::for_each(v, [&sum](int x){ sum += x; });
    std::cout << "sum = " << sum << "\n";

    struct Counter {
        int count = 0;
        void operator()(int) { ++count; }
    };

    auto res3 = std::ranges::for_each(v, Counter{});
    std::cout << "processed " << res3.fun.count << " elements\n";

    //-------------------------------------------
    std::vector<int> v2{1,2,3,4,5};
    std::vector<int> evens;
    std::copy_if(v2.begin(), v2.end(), std::back_inserter(evens),
                [](int x){ return x % 2 == 0; });
    cout << "Even: ";
    std::for_each(evens.begin(), evens.end(),
                [](int x){ std::cout << x << " "; });
    cout << "\n";

    namespace views = std::views;
    cout << "Odd: ";
    std::vector<int> v3{1,2,3,4,5};
    for (int x : v3 | views::filter([](int x){ return x % 2 == 1; })) {
        std::cout << x << " ";
    }
    cout << "\n";
}


//---------------------------------------------------------------
void Type() {
    int a = 42;
    decltype(a) b = 5;   // b is int
    decltype((a)) c = a; // c is int& (because (a) is an lvalue expression)
    //  Subtlety:
    // decltype(a) → int
    // decltype((a)) → int&

/*  std::decay_t<T>
"Normalize" a type into a plain value type.
    1. Removes references (int& → int).
    2. Removes cv-qualifiers (const int → int).
    3. Turns array/function types into pointers.*/
int arr[10];

using A = std::decay_t<int&>;       // int
using B = std::decay_t<const int>;  // int
using C = std::decay_t<int[10]>;    // int* (arrays decay to pointer)
using D = std::decay_t<decltype(arr)>; // int*

using T = std::decay_t<decltype(arr)>::value_type; // int

// decltype(expr) → gives the exact type of an expression.
// std::decay_t<T> → cleans type to its “plain” form (remove refs, 
// cv, arrays → pointers)
// value_type → tells you what element type a container/iterator holds.
}
// Generic lambda accepting a vector<T>; you want to know T (pre C++20)
auto f2 = [](const auto& vec) {
    using V = std::decay_t<decltype(vec)>;   // deduce vector<T>
    using T = typename V::value_type;        // extract T

    T x{};
    // T::static_function();
};
// auto makes the lambda a generic lambda (template under the hood).
// But you can't directly name the template parameter type — you only see the whole vec.
// So you must use decltype(vec) → remove references/const → dig into value_type.
// This is verbose and not obvious to read.

// C++20 (explicit template parameter list in lambda)
auto f3 = [] <typename T> (const std::vector<T>& vec) {
    T x{};
    // T::static_function();
};
// Now the lambda itself has an explicit template parameter T.
// The compiler deduces T directly from the argument type vector<T>.
// Much clearer and more concise.

/*  Virtual function can marked as constexper

*/
int main() {
    Thread_inside_struct();
    RANGES_20();
}