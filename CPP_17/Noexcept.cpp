#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/* noexcept is a specification in C++ that tells the compiler (and the programmer) whether a function will not throw exceptions.

void foo() noexcept;        // This function promises not to throw exceptions.
void bar() noexcept(false); // This function may throw exceptions (default behavior).
void baz() noexcept(std::is_nothrow_constructible<T>::value); 
This is useful for templates where you want to inherit the noexcept property from a type.

🔸 Why Use noexcept?
Optimization
If the compiler knows a function won't throw, it can generate more efficient code (e.g., skipping stack unwinding logic).

Stronger Exception Guarantees
Helps document and enforce the intent of functions not throwing exceptions.

Move Semantics
STL containers prefer move constructors/assignments marked as noexcept because it guarantees that moving won't throw.
Without it, containers fall back to copying, which can be slower.

Cleaner Code
Makes it easier to reason about exception safety and failure behavior.
*/

/*  noexcept is a C++ keyword that tells the compiler whether a function throws exceptions.

There are two main usages:

1. noexcept specifier (declares behavior)
void foo() noexcept;         // Guaranteed not to throw
void bar();                  // Might throw

2. noexcept(expr) operator (evaluates whether an expression can throw)
noexcept(foo())              // true if foo() is declared noexcept */

/*  ✅ Benefits of noexcept
    1. Enables optimizations (e.g., skipping stack unwinding)
    2. Affects move operations (e.g., std::vector)
    3. Helps with static analysis and SFINAE
    4. Required for exception-safe generic code
*/

void may_throw();
void guaranteed() noexcept;

static_assert(!noexcept(may_throw()));      // ✅ false
static_assert(noexcept(guaranteed()));      // ✅ true

template<typename T>
void wrapper(T t) noexcept(noexcept(t.process())) {
    t.process();  // noexcept only if t.process() is noexcept
}

/*  🔹 noexcept in Lambdas
Lambdas aren’t noexcept by default, even if they can't throw.*/
auto lam = [](int x) { return x + 1; };          // not noexcept
auto safe_lam = [](int x) noexcept { return x + 1; };  // now noexcept

static_assert(!noexcept(lam(5)));
static_assert(noexcept(safe_lam(5)));


/*  🔹 noexcept and SFINAE (Substitution Failure Is Not An Error)
Used with enable_if, decltype, etc */
template <typename T>
auto do_stuff(T t) -> std::enable_if_t<noexcept(t()), void> {
    t();  // Only enabled if t() is noexcept
}
template <typename T>
void run(T t) {
    if constexpr (noexcept(t())) {
        std::cout << "Noexcept path\n";
    } else {
        std::cout << "Might throw\n";
    }
}
/*  🔹 noexcept and Move Semantics
Containers like std::vector prefer move if it’s noexcept, else fall back to copy
If move constructor is not noexcept, moving during resize may throw — leading to fallback or reallocation problems.*/
struct X {
    X(X&&) noexcept;     // ✅ vector prefers this
    X(const X&);         // fallback if move isn’t noexcept
};


int main() {


    return 0;
}