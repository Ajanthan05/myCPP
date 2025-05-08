#include <iostream>
using namespace std;

/*  ✅ Types of Template Specialization*/
// Template definition
template <typename KEY, typename DATA>
struct Qmap {
    Qmap() { std::cout << "Qmap\n";}
    // Default inplementation
};

// Parallel specialization
template <typename DATA>
struct Qmap<int, DATA> {
    Qmap() { std::cout << "Qmap<int, DATA>\n";}
    // Optimization for int-base keys
};

// Full specialization
template <>
struct Qmap<int, int> {
    Qmap() { std::cout << "Qmap<int, int>\n";}
    // special super fast version forint => int map
};

int T() {
    auto default_ = Qmap<float, bool>{};

    auto Parallel_specialization = Qmap<int, bool>{};

    auto Full_specialization = Qmap<int, int>{};
}


// This is the generic version of the class template A. It can be instantiated with any types T1, T2 and any integer I.
template<class T1, class T2, int I>
class A {
public:
    static void print() { cout << "Primary Template\n"; }
};

// 🔹 #1: T2 is a pointer to T1     A<double, double*, 3> obj1;  // Matches #1
template<class T, int I>
class A<T, T*, I> {
public:
    static void print() { cout << "Partial Specialization #1\n"; }
};

// 🔹 #2: T1 is a pointer       A<float*, char, 10> obj2; 
template<class T, class T2, int I>
class A<T*, T2, I> {
public:
    static void print() { cout << "Partial Specialization #2\n"; }
};

/* 🔹 #3: T1 = int, T2 is a pointer, I = 5
Matches:
T1 = int
T2 = pointer to T
I = 5
ex: A<int, double*, 5> obj3;*/
template<class T>
class A<int, T*, 5> {
public:
    static void print() { cout << "Partial Specialization #3\n"; }
};

// 🔹 #4: T2 is a pointer to T      ex: A<char, long*, 7> obj4;
template<class X, class T, int I>
class A<X, T*, I> {
public:
    static void print() { cout << "Partial Specialization #4\n"; }
};

/*  C++ uses a mechanism called partial ordering of specializations.*/
void T2() {
    A<double, double*, 3>::print(); // #1
    A<float*, char, 10>::print();   // #2
    A<int, double*, 5>::print();    // #3
    A<char, long*, 7>::print();     // #4
    A<char, int, 2>::print();       // primary
}



///////////////     The argument list       //////////////////////
template<class T1, class T2, int I> class B {};        // Primary template
/* template<class X, class Y, int N> class B<X, Y, N> {}; // ❌ Error!
  The second line is not specializing anything — it's just a duplicate of the primary template using different parameter names.*/


// ❌ 2. Error: Not More Specialized
// template<int N, typename T1, typename... Ts> struct B;

/* This is invalid:
template<typename... Ts>
struct B<0, Ts...> {}; // ❌ Error
C++ cannot determine that B<0, Ts...> is more specialized than 
B<N, T1, Ts...>. It does not uniquely match the pattern in the primary template because it doesn't constrain all parameters clearly.*/
template<int N, typename T1, typename... Ts>
struct B3 {
    static void print() { std::cout << "General case\n"; }
};

// Full specialization for N = 0
template<typename T1, typename... Ts>
struct B3<0, T1, Ts...> {
    static void print() { std::cout << "Specialized for N = 0\n"; }
};

void T3() {
    B3<5, int, char>::print();    // General case
    B3<0, int, char>::print();    // Specialized for N = 0
}

/*  2) ❌ Default arguments cannot appear in a template argument list
template<typename T = int>
struct MyTemplate {}; // ❌ Error if used in a partial or full specialization
Default template arguments are only allowed in primary templates, not in specializations.
*/

template<typename T = int>
struct MyTemplate {}; // OK: Primary template with default argument
template<>
struct MyTemplate<double> {}; // Specialization (no default here)

/*  3) ❌ A pack expansion must be the last template parameter
Template packs (...) can only expand at the end because the compiler needs to know where to stop the expansion. Putting anything after them creates ambiguity.
🚫 Invalid:
template<typename... Args, typename T>
struct Foo {}; // ❌ Error: pack is not the last parameter*/
template<typename T, typename... Args>
struct Foo {}; // OK: pack is at the end

/*  4) ⚠ Constant arguments can use template parameters only if deducible
❌ Invalid (Non-deduced context):
template<int I, int J>
struct A {};

template<int I>
struct A<I + 5, I * 2> {}; // ❌ Error: I is not deducible
In A<I + 5, I * 2>, there’s no way to deduce I from the arguments (I + 5, I * 2) because the compiler can't reverse the math to solve for I.*/
template<int I, int J, int K>
struct B4 {};

template<int I>
struct B4<I, I * 2, 2> {};  // ✅ OK: First parameter is exactly I
/*  Since the first parameter is directly I, it's deducible.
The compiler can then plug I into the remaining two parameters.*/

/* ??? REF: Raad name space with template https://en.cppreference.com/w/cpp/language/partial_specialization
Class template argument deduction (CTAD)
https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
*/



/* B. Partial Specialization:  You specialize the template for a subset of its parameters.
??? Partial specializations cannot be done for function templates (use overload or SFINAE instead).*/

// Primary template
template<typename T1, typename T2>
struct Pair { };

// Partial specialization when both types are the same
template<typename T>
struct Pair<T, T> {
    void info() { std::cout << "Same types\n"; }
};

/*  C. Value Specialization (Non-type template parameters)
You can specialize based on constant values (like int, bool, etc.).*/
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

static_assert(Factorial<5>::value == 120);
