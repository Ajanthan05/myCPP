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

int main() {


    return 0;
}