#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/*  constexper tells the compiler “This value/function/object can and should be evaluated at compile-time, if possible.” 

✅ C++17 – constexpr if, lambdas, etc.

✅ C++20 – even more powerful: dynamic memory in constexpr, virtuals, try-catch, etc.


✅ Performance — computation is done at compile-time, not at runtime.

✅ Safety — static_assert helps catch logic errors early. ???

✅ Usability — works with templates, constexpr arrays, etc. ???

*/

// 🔸 C++11 Can only contain one return statement and no branches or loops.
constexpr int square(int x) { return x * x; }

//  C++14 and Later     Allows more complex logic like loops, conditionals, local variables.
// constexpr functions can use loops — but for them to be evaluated at compile time, all inputs must be constant expressions.
constexpr int factorial(int n) {
    int res = 1;
    for (int i = 2; i <= n; ++i)
        res *= i;
    return res;
}


// 🔹 constexpr Objects / Structs
struct Point {
    int x, y;
    constexpr Point(int x, int y) : x(x), y(y) {}
    constexpr int sum() const { return x + y; }
};


/*  🔹 constexpr and consteval (C++20)
🔸 constexpr    Means “can be run at compile time or runtime”.
🔸 consteval    Means “must be run at compile time”.*/
    consteval int always_compile_time(int x) {
        return x * 2;
    }

// 🔹 constexpr if (C++17)  Used in templates for compile-time branching:
template <typename T>
void print_type() {
    if constexpr (std::is_integral_v<T>)
        std::cout << "Integral\n";
    else
        std::cout << "Not integral\n";
}

// 🔸 Compile-Time Lookup Table
constexpr int table[5] = { 1, 2, 4, 8, 16 };

constexpr int get_value(int index) {
    // return table[index];  // 🚨 Not valid constexpr usage
    return (index >= 0 && index < 5) ? table[index] : -1;
}

static_assert(get_value(2) == 4);


/* cpp 14
Multiple statements allowed
constexper member nolinger const 
l;ambda is not allowed

    C++17
lambda automatically constexper enable

    C++20
constexper dynamic allocation
(dynamic allocation cannot leave constexper)
constexper distructors
constexper vector
constexper string
constexper std algorithms
constexper virtual function

CTRE:- Regular  Expression are compiled at compile time 
and executed at runtime

SILLY The world's fastest Raytracer
constexper enables ARM Emulator

*/

int main() {

    constexpr int val = square(5);
    std::cout << val << '\n';

    constexpr int fact5 = factorial(5); // ✅ Evaluated at compile time

    constexpr Point p(3, 4);
    static_assert(p.sum() == 7); 
    cout << "hi: " << p.sum() << "\n";

// 🔸 Compile-Time Lookup Table
    constexpr int lookupT = get_value(2);
    cout << "lookupT: " << lookupT << "\n";
    return 0;
}