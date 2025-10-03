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
#include <optional>
#include <shared_mutex>

#include <algorithm>
#include <ranges>
#include <type_traits>  // decay_t

#include <compare>
#include <cctype>
#include <cstring>

#include <string_view>
#include "ctre.hpp"


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

void PtrDiff() {
    int arr[5] = {10, 20, 30, 40, 50};
    int *ptr1 = &arr[0];
    int *ptr2 = &arr[2];

    ptrdiff_t diff = ptr2 - ptr1; // Calculate the difference
    cout << "Difference between pointers: " << diff << "\n"; // Use %td for ptrdiff_t

}

/*  semaphore is a synchronization primitive that controls access to a shared resource by counting available permits.

If the count > 0 → threads may proceed (acquire a permit).

If the count = 0 → threads block until another thread releases a permit.

It’s like having a limited number of “keys” to enter a room.

acquire() = take a key (wait if none available).

release() = return a key (possibly waking up another thread).


1. std::counting_semaphore<N>
    A semaphore that can hold up to N permits.
    The template parameter N is the maximum count (compile-time constant).

2. std::binary_semaphore
    A semaphore that only allows counts 0 or 1.
    Equivalent to a mutex-like lock, but lighter.
    Defined as using binary_semaphore = counting_semaphore<1>;.

Mutex	Exclusive lock (1 thread at a time)
Atomic	Protect single variable with low overhead
Semaphore	Limit concurrency to N threads (not just 1)

Mutex = mutual exclusive access to a critical section. Owner-based (only owner 
should unlock). Best for protecting shared data structures.

Semaphore = counting resource controller (can be binary or counting). 
Not owner-based — any thread/process can post (release). Best for limiting concurrency
(resource pools) or signalling (producer/consumer).

WARNINKG Notes & Gotchas
    Semaphores in C++20 are always non-recursive (a thread cannot acquire more than
it releases without deadlocking).
    std::binary_semaphore is like a lightweight mutex, but without ownership 
tracking (so you must pair acquire/release correctly).
    counting_semaphore is useful for resource pools (e.g., database connections, 
thread-safe queues).
*/

// acquire(), release(), try_acquire(), try_acquire_for(), try_acquire_until()
std::counting_semaphore<10> sem(3); // allow 3 concurrent permits
// At most 3 threads will be inside the critical section at the same time.
void Semaphore(int id) {
    sem.acquire(); // take permit
    std::cout << "Thread " << id << " is working...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread " << id << " done.\n";
    sem.release(); // release permit
}
void T_Semaphore() {
    std::vector<std::thread> threads;
    for(int i=0; i<10; i++) {
        threads.emplace_back(Semaphore, i);
    }
    for (auto& t : threads) t.join();
}


struct Data {
  int anInt { 0 };
  std::string aString;
};
// Designated initialization
void DesignatedInitialization() {
    Data d { .aString = "Hello" };
}

// Before C++20
// class Point {
// public:
//   int x; int y;
// public:
//   friend bool operator==(const Point& a, const Point& b){ return a.x==b.x && a.y==b.y; }
//   friend bool operator< (const Point& a, const Point& b){ return a.x < b.x ||
//                                                           (a.x == b.x && a.y < b.y); }
//   friend bool operator!=(const Point& a, const Point& b) { return !(a==b); }
//   friend bool operator<=(const Point& a, const Point& b) { return !(b<a); }
//   friend bool operator> (const Point& a, const Point& b) { return b<a; }
//   friend bool operator>=(const Point& a, const Point& b) { return !(a<b); }
//   // ... non-comparison functions ...
// };

struct Point {
    int x, y;
    auto operator<=>(const Point&) const = default;
};

void SpaceshipOperator() {
    std::vector<Point> v = { {2,3}, {1,5}, {2,1}, {2,1} };

    std::sort(v.begin(), v.end()); // uses <, which compiler generates

    for (auto& p : v)
        std::cout << "(" << p.x << "," << p.y << ") ";

    cout << "\n";
}

/*  std::strong_ordering
(like integers, compares with total ordering, == implies substitutable).

std::weak_ordering
(values can be equal without being substitutable, e.g. case-insensitive strings).

std::partial_ordering
(some values might be unordered, e.g. NaN in floating-point comparisons).

1) std::strong_ordering (total, strict ordering — like integers)
All values are comparable, and if a == b, they are fully interchangeable 
(“substitutable”).
    Strong ordering works because integers have total order — every pair of ints 
compares <, ==, or >.

2) std::weak_ordering (equivalent but not substitutable)
Use this when values can compare “equal” but aren’t identical.
Example: case-insensitive string comparison — "hello" and "HELLO" are “equivalent”, 
but not the same actual string.
    "Hello" and "HELLO" compare equivalent, but they’re not literally equal strings. */

/* NOTE : If you pass a plain char that happens to have a negative value 
(for example, non-ASCII characters like é when char is signed), std::tolower has 
undefined behavior.*/
// struct CiString {
//     std::string s;
//     static std::string tolower_copy(const std::string& str) {
//         std::string r;
//         for(char c : str) r.push_back(std::tolower(static_cast<unsigned char>(c)));
//         return r;
//     }

//     std::weak_ordering operator<=>(const CiString& other) const {
//         auto lhs = tolower_copy(s);
//         auto rhs = tolower_copy(other.s);
//         if (lhs < rhs) return std::weak_ordering::less;
//         if (lhs > rhs) return std::weak_ordering::greater;
//         return std::weak_ordering::equivalent;
//     }
// };
struct CiString {
    std::string s;

    static std::string tolower_copy(const std::string& str) {
        std::string r;
        for(char c : str) r.push_back(std::tolower(static_cast<unsigned char>(c)));
        return r;
    }

    std::weak_ordering operator<=>(const CiString& other) const {
        auto lhs = tolower_copy(s);
        auto rhs = tolower_copy(other.s);
        if (lhs < rhs) return std::weak_ordering::less;
        if (lhs > rhs) return std::weak_ordering::greater;
        return std::weak_ordering::equivalent; // not strictly equal
    }

// In C++20, defining <=> does not automatically generate 
// operator== unless you also explicitly = default it.
    bool operator==(const CiString& other) const {
        return tolower_copy(s) == tolower_copy(other.s);
    }
};
void T_weakOrdering() {
    CiString a{"Hello"}, b{"HELLO"}, c{"World"}, d{"Apple"};

    std::cout << std::boolalpha;
    std::cout << "(a == b) = " << (a == b) << "\n"; // true (equivalent)
    std::cout << "(a == c) = " << (a == c) << "\n"; // false
    std::cout << "(a <  d) = " << (a <  d) << "\n"; // false
}

/*  3) std::partial_ordering (not all values are comparable)

Example: floating-point numbers with NaN. A NaN is unordered (neither <, >, nor ==).
With partial_ordering, NaN is unordered relative to any value (even itself).*/
#include <compare>
#include <cmath>
#include <iostream>

struct FPBox {
    double value;

    std::partial_ordering operator<=>(const FPBox& other) const {
        if (std::isnan(value) || std::isnan(other.value))
            return std::partial_ordering::unordered;

        if (value < other.value) return std::partial_ordering::less;
        if (value > other.value) return std::partial_ordering::greater;
        return std::partial_ordering::equivalent;
    }
    bool operator==(const FPBox& other) const {
        return value == other.value;
    }
};
void T_PartialOdering() {
    FPBox a{1.0}, b{2.0}, nan{std::nan("")};

    std::cout << std::boolalpha;
    std::cout << "(a < b)     = " << (a < b) << "\n";       // true
    std::cout << "(a == a)    = " << (a == a) << "\n";      // true
    std::cout << "(a < nan)   = " << (a < nan) << "\n";     // false
    std::cout << "(nan < a)   = " << (nan < a) << "\n";     // false
    std::cout << "(nan == nan)= " << (nan == nan) << "\n";  // false
}


// ------------------   Range-based for Loop Initializer --------
// Initializers for switch statenents , if statements (C++17)
struct foo { int value; int result; };

foo GetData() { return foo(); }
void Loops() {
    switch(auto data{ GetData() }; data.value) {
        case 1:
            cout << data.result << "\n";
    }

    if (auto data { GetData() }; data.result) {
        // use 'data'
    }

    // C++20 available for range based for lopso
}

// -------- NON - TYPE TEMPLATE PARAMETERS -------------
/* Limitations :- can't use classes, floating point types

Before C++20 NTTPs were limited (integrals, pointers-to-linkage-objects, 
enums, etc.). C++20 extended NTTP to allow literal class types (so-called 
structural types) as template parameters. That makes it possible to pass a 
small fixed, constexpr string object as a template argument.

*/
template <std::size_t N>
struct fixed_string {
    char data[N];
    constexpr fixed_string(const char (&s)[N]) {
        for (std::size_t i=0; i<N; ++i) data[i] = s[i];
    }
    // trivial eonstexper access
    constexpr const char* c_str() const { return data; }
};
// NTTP of class type
template <fixed_string Pattern>
struct matcher {
    static bool match(const char* s) {
        return std::strcmp(Pattern.c_str(), s) == 0;
    }
};
void NTTP() {
    // instantiate a compile-time matcher with the literal "hello"
    bool r = matcher<"hello">::match("hello");
    std::cout << std::boolalpha << r << '\n'; // prints true
}

/*
// Template that validates a string against a regex pattern at compile time
template <ctre::fixed_string Pattern>
struct RegexValidator {
    static bool validate(std::string_view input) {
        return ctre::match<Pattern>(input);
    }
};
void CTRE() {
    std::string s1 = "abc123";
    std::string s2 = "xyz";

    // Use different regexes as template parameters
    bool r1 = RegexValidator<"[a-z]+[0-9]+">::validate(s1);
    bool r2 = RegexValidator<"[a-z]+[0-9]+">::validate(s2);

    std::cout << std::boolalpha;
    std::cout << "s1 valid? " << r1 << "\n"; // true
    std::cout << "s2 valid? " << r2 << "\n"; // false
}
*/

/*  They don’t change program behavior (except performance) and the CPU’s 
dynamic branch predictor and Profile-Guided Optimization (PGO) can override them.

*/
int main() {
    // PtrDiff();
    // Thread_inside_struct();
    // RANGES_20();

    SpaceshipOperator();
    T_weakOrdering();
    NTTP();
    // CTRE();
}

/*  <;numbers>  
e, log2e, log10e
pi, inv_pi, inv_sqrtpi
ln2, ln10
sqrt2, sqrt3, inv_sqrt3
egamma
phi     */
// git clone https://github.com/hanickadot/compile-time-regular-expressions.git
// g++ -std=c++20 -I./compile-time-regular-expressions/single-header main.cpp -o main

/*
https://www.youtube.com/watch?v=2EWejmkKlxs
https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#Instrumentation-Options
https://www.youtube.com/watch?v=7xObVomisE4
*/