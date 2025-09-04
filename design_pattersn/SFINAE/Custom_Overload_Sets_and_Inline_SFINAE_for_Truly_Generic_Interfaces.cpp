#include <iostream>
#include <concepts>
#include <type_traits>

// 1) Catch-all: always present, lowest priority in overload resolution
void test(...) { std::cout << "Catch-all overload called\n"; }

// 2) "Reference" overload: participates only if (c .* f)() is well-formed
template<class C, class F>
auto test(C c, F f) -> decltype((void)(c .* f)(), void()) {
    std::cout << "Reference overload called\n";
}

// 3) "Pointer" overload: participates only if (c ->* f)() is well-formed
template<class C, class F>
auto test(C c, F f) -> decltype((void)((c->*f)()), void()) {
    std::cout << "Pointer overload called\n";
}

// Structure with a function f
struct X { void f() {}};

/*  c .* f is the pointer-to-member invocation operator. It is valid only when c is an object (or reference) whose type is compatible with the member pointer f.

(c .* f)() denotes calling the member function. We do this in an unevaluated context (inside decltype), so the code is checked for well-formedness but not actually executed.

(void)(c .* f)() casts the result to void — this ensures we don’t care about the return type of the member function.

The comma operator , in decltype((void)(c .* f)(), void()) makes the whole expression have the type of its second operand — void(), whose type is void. In short: if the expression is well-formed, the trailing return type is void.

If the expression is ill-formed for the deduced C and F, then substitution fails — SFINAE applies and that template overload is removed from consideration (instead of being a hard error).

The same reasoning applies to the pointer version -> decltype((void)((c->*f)()), void()).*/
void T1() {
    X x;
    test(x, &X::f);  // selects the "reference" overload
    test(&x, &X::f); // selects the "pointer" overload
    test(42, 1337);  // both templates SFINAE away, picks catch-all
}


/*  template <class T, class = std::enable_if_t<std::is_integral_v<T>>>
void function(T x) { std::cout << "integer\n"; }

// floats (note: signature = const T&)
template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>>
void function(const T& x) { std::cout << "floating point\n"; }

You put enable_if as a default template parameter. That is a valid SFINAE technique: 
when the condition is false the specialization is removed from the overload set.

Important: the two templates must be distinguishable after substitution 
(i.e., different function parameter lists) so overload resolution can choose between 
them. In your example you used T vs const T& — that makes the function signatures 
different, which avoids a direct duplicate-signature error.*/
template <class T>
std::enable_if_t<std::is_integral_v<T>, void>
function(T x) {
    std::cout << "integer\n";
}

// floating points
template <class T>
std::enable_if_t<std::is_floating_point_v<T>, void>
function(const T& x) {
    std::cout << "floating point\n";
}

// structures (same as yours)
template <class T, class = void>
struct structure {
    void operator()() const { std::cout << "structure<T>\n"; }
};

template <class T>
struct structure<T, std::enable_if_t<std::is_integral_v<T>>> {
    void operator()() const { std::cout << "structure<integer>\n"; }
};

void T2() {
    function(5);     // integer
    function(0.5);   // floating point
    structure<double>{}(); // structure<T>
    structure<int>{}();    // structure<integer>
}

// C++20
template <std::integral T>
void function20(T) {
    std::cout << "integer\n";
}
template <std::floating_point T>
void function20(const T&) {
    std::cout << "floating point\n";
}
template <typename T>
struct structure20 {
    void operator()() const { std::cout << "structure<T>\n"; }
};
template <std::integral T>
struct structure20<T> {
    void operator()() const { std::cout << "structure<integer>\n"; }
};

void concepts1() {
    function20(5);      // integer
    function20(0.5);    // floating point
    structure20<double>{}(); // structure<T>
    structure20<int>{}();    // structure<integer>
}



/*  SFINAR is useful when we want to select a different inplementation based on a
"characteristic" or "TRAIT" of a type
    With SFINAE you can also "disable" a functions based on a trait.

*/

struct RobotA {
    static const int Bullets{};
    void shootBullets() {}
};

struct RobotB {
    static const int Missiles{};
    void shootMissiles() {}
};

template<class Robot>
void handleEnemies(Robot& r, decltype(Robot::Bullets)* = nullptr) {
    r.shootBullets()
}





int main (int argc , char* argv []) {
    T1();
    // T2();
    concepts1();

    return 0;
}

