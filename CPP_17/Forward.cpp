#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/*
std::forward<T>(arg) preserves the value category (lvalue or rvalue) of a function argument during template forwarding.

Think of it as the smarter cousin of std::move, but used only in templates.

*/

void inner(int&& x) {
    cout << "RValue: " << x << "\n";
}

void inner(int& x) {
    cout << "LValue: " << x << "\n";
}
template<typename T>
void Wrapper(T&& arg) {
    inner(std::forward<T>(arg));
}

//////////////////////////
// Even if you call wrapper(std::move(x)), arg is a local lvalue inside the function — the move is lost.
template<typename T>
void wrapper(T arg) {
    do_something(arg); // always an lvalue inside
}

// ✅ Solution: Use std::forward
template<typename T>
void wrapper2(T&& arg) {
    do_something(std::forward<T>(arg));
}
// T&& is a universal reference (also called forwarding reference).
// std::forward<T>(arg) preserves the original lvalue/rvalue-ness.


int main() {

    int x = 5;
    Wrapper(x); // lvalue
    Wrapper(50); // rvalue
    return 0;
}