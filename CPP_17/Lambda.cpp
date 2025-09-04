#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <vector>
#include <utility>  // for std::move

#include <numeric> // for std::accumulate
#include <initializer_list>
using namespace std;
/*
[ captures ] ( parameters ) -> return_type {
    // function body
};
*/

auto add = [](int a, int b) {
    return a + b;
};

/* Capturing this in a class  from c++14*/
class Test {
    int val = 42;
public:
    void show() {
        auto lambda = [this]() {
            std::cout << val << std::endl;
        };
        lambda();
    }
};

void T() {
    std::cout << add(2, 3) << "\n"; // Output: 5

    // If it’s complex or ambiguous, use -> type
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0;
        return a / b;
    };

    // 1. Capture by Value (=)
    int x = 10;
    auto val_capture = [=]() {
        std::cout << x << std::endl; // captures a *copy* of x
    };

    // 2. Capture by Reference (&)
    int z = 3;
    auto ref_capture = [&]() {
        std::cout << "ref_capture: " << x << std::endl;
        x += 5;
    };
    
    ref_capture();
    std::cout << x; // Output: 15

    // 3. Mixed Capture
    int a = 1, b = 2;
    auto lambda = [a, &b]() {
        // a is captured by value, b by reference
    };

/*
Default Capture Modes
[=] – capture everything by value

[&] – capture everything by reference

[=, &b] – capture everything by value, but b by reference

[&, a] – capture everything by reference, but a by value
*/

    std::vector<int> v = {1, 2, 3, 4, 5};
    std::for_each(v.begin(), v.end(), [](int x) {
        std::cout << x * 2 << " ";
    });

    /* Mutable Lambdas
    If you capture by value but want to modify the captured copy
    
    🔸 Key Points:
x is captured by value, so it's a copy inside the lambda.

mutable allows you to modify that copy.

The original x outside the lambda is unchanged.

✅ When to Use This Pattern
Use this when:

You need to capture a variable's value at the time the lambda is created.

You need to modify the copy, e.g., for temporary calculations or repeated use inside the lambda.

You do not want to modify the original variable outside the lambda.
*/
    cout << "\n\n";
    int y = 10;
    auto lambda_mutable = [y]() mutable -> double {
        y += 5; // OK because lambda is mutable
        std::cout << "In lambda_mutable: " << y << std::endl; // 15
        return y;
    }; // Output: Inside lambda: 15
    double d = lambda_mutable();
    std::cout << "lambda_mutable: " << y << " return: " << d << std::endl; // still 10




    /* Lambdas as Function Pointers */
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    int (*func_ptr)(int, int) = add;
    std::cout << "func_ptr: " << func_ptr(3, 4) << "\n"; // Output: 7

    /* Generic Lambdas (C++14+) */
    auto generic = [](auto x, auto y) {
        return x + y;
    };
    std::cout << "generic(3, 4.5): " << generic(3, 4.5) << "\n"; // Output: 7.5
}


class Plus {
    int value;

public:
    explicit Plus(int v) noexcept : value(v) {}

    int operator()(int x) const noexcept {
        return x + value;
    }
};
void apply(const Plus& p) {
    int r = p(10);  // works only if operator() is const
}
void test() {
    Plus add_five(5);
    int result = add_five(10);  // returns 15
    cout << result << "\n";
}
/*
explicit avoids accidental implicit conversions.

noexcept marks functions that won't throw — helpful for compiler optimizations.

No need for move constructor/assignment here since int is a trivial type.

This class is lightweight and already optimal for both lvalues and rvalues.



means that this member function promises not to modify the object’s state — i.e., it won’t modify any non-mutable member variables of the object.

In your case:
value is a regular int (not mutable),
and the operator does not modify value,
so marking operator() as const tells the compiler and caller:
“You can safely call this function on a const Plus object.”

*/


class PlusVector {
    std::vector<int> values;

public:
    // Default constructor
    PlusVector() = default;

    // Constructor from lvalue reference
    explicit PlusVector(const std::vector<int>& vec) : values(vec) {}

    // Constructor from rvalue reference (move constructor)
    explicit PlusVector(std::vector<int>&& vec) noexcept : values(std::move(vec)) {}

    // Copy constructor
    PlusVector(const PlusVector&) = default;

    // Move constructor
    PlusVector(PlusVector&& other) noexcept = default;

    // Copy assignment
    PlusVector& operator=(const PlusVector&) = default;

    // Move assignment
    PlusVector& operator=(PlusVector&& other) noexcept = default;

    // Functor: add element-wise to another vector
    std::vector<int> operator()(const std::vector<int>& input) const {
        std::vector<int> result;
        result.reserve(std::min(input.size(), values.size()));
        for (size_t i = 0; i < std::min(input.size(), values.size()); ++i) {
            result.push_back(input[i] + values[i]);
        }
        return result;
    }
};

void T_Vector() {
    std::vector<int> data = {1, 2, 3};
    std::vector<int> add = {10, 20, 30};

    PlusVector adder(std::move(add));  // move constructor

    std::vector<int> result = adder(data);  // calls operator()

    for (int val : result)
        std::cout << val << " ";  // Output: 11 22 33
}


// LAMBDAS REDUCE BOILERPLATE
auto plus = [value=1](int x) { return x + value; };
/*  We achive this in python heap=allocation, guarbage collection, runtime polymorphysm*/
// class Book{};
// bool contains_title(const std::vector<Book> &shelf, std::string title) {
//     auto has_title_t = [t=title](const Book& b) {
//         return b.title() == t;
//     };

//     return v.end() != std::find_if(v.begin(), v.end(), has_title_t);
// }

class Book {
    std::string m_title;
public:
    Book(std::string title) : m_title(std::move(title)) {}
    const std::string& title() const { return m_title; }
};

bool contains_title(const std::vector<Book>& shelf, const std::string& title) {
    // auto has_title = [&title](const Book& b) {
    //     return b.title() == title;
    // };

    // auto has_title = [pt=&title](const Book& b) {
    //     return b.title() == *pt;
    // };

    // auto has_title = [&t=title](const Book& b) {
    //     return b.title() == t;
    // };

    // auto has_title = [t=std::move(title)](const Book& b) {
    //     return b.title() == t;
    // };
    // // [t=std::move(title)] capturethe string by value, but prefier move constructor
    // return std::find_if(shelf.begin(), shelf.end(), has_title) != shelf.end();
}
void T_contains_title() {
    std::vector<Book> shelf = { Book("C++"), Book("Python") };

    if (contains_title(shelf, "C++"))
        std::cout << "Found\n";
    else
        std::cout << "Not found\n";
}


template<typename T, std::size_t N>
auto capture_array(T (&arr)[N]) {
    return [arr]() { return sizeof(arr); }; // ✅ `arr` is full array
}
void T_no_decay() {
    char arr[10];
    auto lambda = capture_array(arr);
    std::cout << lambda() << "\n";  // ✅ 10
}

// Global/ Statics aren't captured; neither are unevaluated operands
int g = 10;
// auto kitten = [=]() { return g+1; };
auto cat    = [g=g]() { return g+1; };

void T2() {
    g = 20;
    cout << " cat: " << cat() << "\n";
}

// Convertible to Raw function pointer (If captureless)
int (*fp)(int) = [](int x) { return x+1; };
// underlying magic
struct __lambda {
    static int __invoke(int x) { return x + 1; }
    using function_type = int(*)(int);
    operator function_type() const { return &__invoke; }
};
/*  You’ll get a compile-time error, because capturing lambdas cannot be converted to raw function pointers — they need context (closure state).

✅ Use Cases
Callbacks in C APIs (like qsort, signal handlers, etc.)
Interfacing lambdas with older code:*/
void Test() {
    int (*fp)(int) = __lambda();  // Converted to function pointer
    cout << fp(5) << "\n";
}

// template<class T> void fun(T t);
// fun(+[](int x) { return x+1; });
// Template function that accepts any callable
template <typename T>
void fun(T t) {
    std::cout << t(10) << '\n'; // call with example value
}

// Overload just for raw function pointers
void fun(int (*fp)(int)) {
    std::cout << "Function pointer version: ";
    std::cout << fp(10) << '\n';
}
// Unary + to Decay Lambda into Function Pointer
void T3() {
    // Captureless lambda
    auto lambda = [](int x) { return x + 1; };
    decltype(lambda) copy; // ✅ C++20: OK
    cout << "In C++20, a captureless lambda is default-constructible: " << copy(5) << "\n";
// auto cap = [a](int x) { return x + a; };
// decltype(cap) c;  // ❌ ERROR: no default constructor — captures are involved

    // Lambda operator() is constexpr by default (if possible)
    static_assert(lambda(42) == 43);  // ✅ OK in C++17
    // This lambda is captureless, and
    // Its body is a constexpr-compatible expression.

/*Since there's no capture and the lambda body is a pure computation, the compiler automatically marks the operator() as constexpr.

📌 In C++20, captureless lambdas with constexpr-compatible bodies are implicitly constexpr.

❌ 2. But it's not noexcept by default*/



    // 1. Pass as normal lambda
    fun(lambda);  // T = lambda type (calls first template version)

    // 2. Force decay to function pointer using unary +
    fun(+[](int x) { return x + 1; });  // T = int(*)(int), triggers function pointer overload

    // 3. Show the type directly
    int (*fp)(int) = +[](int x) { return x * 2; };
    std::cout << "Manual call: " << fp(5) << '\n';  // 10
}

/*  per-lambda mutable state
[i = 0]: This is a lambda capture with initializer (C++14+). You're capturing a copy of i, initialized to 0.

mutable: By default, lambdas' operator() is const, which means it can’t modify captured variables.
mutable allows modification of captured copies inside the lambda.

return ++i;: Increments the captured i and returns it.*/
auto counter = [i = 0]() mutable {
    return ++i;
};
void T4() {
    cout << counter() << "\n";
    cout << counter() << "\n";
    cout << counter() << "\n";
}

// Class member function template
class PluS {
    int val;
public:
    PluS(int v) : val{v} {}

    template<class T>
    T operator()(T x) const {
        return x + val;
    }
};
void T5() {
    // PluS Pfive(5);
    auto Pfive = PluS(5);
    auto x = Pfive(4.5);
    cout << x << "\n";
}
// Generic lambdas are just templates under the hood
auto  P5 = [val = 5](auto x) { return val + x; };

// Variadic lambdas
// class VariadicPluS {
//     int val;
// public:
//     VariadicPluS(int v) : val{v} {}

//     template<class... As>
//     auto operator()(As... as) const {
//         return std::sum(as..., val);
//     }
// };
// void Variadic() {
//     // PluS Pfive(5);
//     auto Pfive = VariadicPluS(5);
//     auto x = Pfive(4.5, 4);
//     cout << "Variadic: " << x << "\n";
// }


/*  1. Concise Function Objects
Lambdas eliminate the need to define separate structs or classes with operator() 
when you just need a small callable object. */
class PrintElement {
public:
    void operator()(int x) const {
        std::cout << x << " ";
    }
};
void Test_Why_lambda() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Use the functor with std::for_each
    std::for_each(v.begin(), v.end(), PrintElement());

    std::for_each(v.begin(), v.end(), [](int x) { cout << x << " ";});
}

/*  2. Local Behavior with Captures
Lambdas can capture local variables from the surrounding scope, something 
regular functions can't do.

int factor = 2;
auto multiply = [factor](int x) { return x * factor; };
std::cout << multiply(5);  // Outputs 10

3. Better for Functional-Style Code
C++ STL algorithms (like std::sort, std::find_if) often accept predicates. 
Lambdas make these easier to write and more readable.

std::sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;  // Sort descending
});

4. Cleaner Callbacks
In event-driven programming, threading, or GUI code, lambdas help you pass 
small logic blocks as callbacks without cluttering code.

std::thread([]() {
    std::cout << "Thread running\n";
}).join();

5. Capture by Move
Lambdas can move ownership (e.g., of std::unique_ptr, std::thread ,closures, etc.), 
useful in modern C++ resource management.

auto ptr = std::make_unique<int>(42);
auto f = [p = std::move(ptr)]() {
    std::cout << *p << "\n";
};
*/
void Capture_by_Move() {
    std::string name = "Ajantharajah";\
    auto greet = [n = std::move(name)]() {
        cout << "Hello " << n << "\n";
    };
    greet();

    
// Capturing a std::thread into a lambda
    std::thread t([] {
        std::cout << "Hello from thread\n";
    });

    auto lambda = [t = std::move(t)]() mutable {
        if (t.joinable()) t.join();
    };

    lambda();  // Waits for thread to finish
}

/*  Does the compiler inline lambdas during optimization?
Yes, most modern compilers (like GCC, Clang, MSVC) aggressively inline small lambda bodies if optimization flags are enabled (e.g., -O2, -O3), especially if:

The lambda is small

The call site is visible (not in a separate translation unit)

It’s not assigned to a std::function or similar

If the lambda is assigned to a variable and used across translation units, 
it can lead to ODR (One Definition Rule) violations unless properly handled.
*/

class Person {
public:
    string name;
    int age;
};

/*
// comopiler generate a closer type with inline call operator
struct __lambda_1 {
    inline bool operator() (const Person& rhs, const Person& lhs) const { 
        return rhs.name < lhs.name; 
    };

    __lambda_1() = delete;      // Not default constructible
    __lambda_1& operator=(const __lambda_1&) = delete; // not assignable

when the square brackets are empty -> hve an mplicit conversion to Raw function 
pointer.  Has a conversion function to pointer to function

    using __func_type = bool(*)(constPerson&, const Person&);
    inline operator __func_type() const noexcept {
        return &__invlke;
    }
    
private:
    staticinline bool __invake(const Person& rhs, const Person& lhs) { 
        return rhs.name < lhs.name; 
    }

};
// and compiler generate unnamed temprory instant of this closure typpe
__lambda_1(); // This instant is our labbda object => This is auto generated by compiler so no error
// Inline call operator is const by default. so u cannot modify members there
// Want to modify u have to write mutable
// U can specify return type by TRAILING RETURN TYPE syntex
// It's not noexcept by default


*/
void Sort() {
    vector<Person> v = {{"Tharmarajah", 15}, {"Rajah", 20}};
    sort(v.begin(), v.end(), [](const Person& rhs, const Person& lhs) { return rhs.name < lhs.name; });

    std::for_each(v.begin(), v.end(), [](const Person& p){ cout << p.name << ": " << p.age << "\n";});
}

void Fun_Ptr() {
    auto* fptr1 = static_cast<int(*)(int)>([](int i) { return i*i; });
    auto* fptr2 = +[](int i) { return i*i; };
    // Function ptr only work for capture less lambda
}

void with_capture() {
/*
struct __lambda_2 {
    __lambda_2(int i, int j) : __i(i), __j(j) {}

    inline bool operator()() const {
        return __i == __j;
    }
private:
    int __i;
    int __j;
};
__lambda_2(i, j);
*/

    int i=0, j=0;
    auto f = [=] {
        return i == j;
    };
}

struct x {
    void printAsync() {
        callAsync([this] { cout << i << "\n"; } );
    }
private:
    int i=42;
};
/*
struct x {
    void printAsync() {
        struct __lambda_3 {
            __lambda_3(X* _this) : __this(_this) {}

            void operator()() const {
                cout << __this->i << "\n";
            }
        
        private: 
            X* __this;
        };
        callAsync(__lambda_3(this));
    
    }
private:
    int i=42;
};
*/

void Capture2() {
    /*  We can only capture local variable

    same for static object same for global variable

    Lambda only capture only Odr use
    */
   static int i = 5;
   auto f = [] { ++i; };  // alsp [=]
   f();

/*   ODR (One Definition Rule)
The One Definition Rule is part of the C++ standard that ensures your program has exactly one definition for each variable, function, class, etc., that is used across translation units.

It exists to avoid situations where the compiler and linker have conflicting or duplicate definitions for the same entity.

Across multiple translation units
If the same entity is used in multiple .cpp files, it must have exactly one definition in the entire program, but you may have multiple declarations.

✅ Example:
// common.h
extern int global_var; // declaration

// a.cpp
#include "common.h"
int global_var = 42;   // definition

// b.cpp
#include "common.h"
// No redefinition here
// int global_var = 99; // ERROR: multiple definitions during linking


*/

   return i;  // Return 6
}

    


/*  A lambda expression is essentially an inline, unnamed function object 
that you can define at the point of use.

1. Short, Inline Functions
Without lambdas, you’d often need to:
Write a separate function
Or define a functor class (struct with operator())

2. Custom Logic in Standard Algorithms
3. Capture Local Variables

4. Used in Callbacks & Event Handling
When working with threads, async tasks, or GUI event systems, lambdas let you pass 
inline custom logic without polluting the codebase with one-off functions.

5. Cleaner Resource Management

In RAII or scope-guard patterns, lambdas are super handy.
std::unique_ptr<FILE, decltype([](FILE* f){ if(f) fclose(f); })> file(fopen("test.txt", "r"));*/

auto make_lambda() {
    std::string local = "test";
    return [varD = std::move(local)] { std::cout << varD; }; // OK, varD owns it

    // BUT if you captured by reference accidentally:
    // return [&local]() { std::cout << local; }; // ⚠️ dangling reference
}

// 4. Exceptions During Move
// If the object’s move constructor throws, the lambda construction fails.
struct X {
    X() {}
    X(X&&) { throw std::runtime_error("move failed"); }
};

// 5. Copy-only Types
// If z is copyable but not movable, std::move(z) will try to use the move constructor → compile error:

struct CopyOnly {
    CopyOnly() {}
    CopyOnly(const CopyOnly&) {}
    CopyOnly(CopyOnly&&) = delete; // no move
};

void Errors() {
    std::string z = "hello";
    auto lam = [varD = std::move(z)] { std::cout << varD; };
    std::cout << z;   // ⚠️ UB if you assume z still holds "hello"

    // 2 Lifetime Issues
    auto lam = make_lambda();
    lam(); // ✅ safe, lambda owns copy

    // 4
    X z;
    auto lam = [varD = std::move(z)] {}; // ⚠️ throws here

    // 5
    CopyOnly z;
    auto lam = [varD = std::move(z)] {}; // ❌ compile error

}

/*  1. Normal reference capture
[&y] { use(y); }
Captures the variable y by reference.
Inside the lambda, the name is still y.

2. Init-capture with reference
[&varC = y] { use(varC); }
Captures y by reference.
But gives it a new name inside the lambda: varC.

🔹 Why [&varC = y] Exists?

Rename variables in lambda scope
Sometimes you want to avoid name shadowing or clarify usage:

int y = 42;
auto f = [&val = y]() { std::cout << val; }; // inside lambda it's 'val'

2 Capture an expression, not just a bare variable
[var = expr] (or [&var = expr]) lets you initialize from any expression:

auto f = [&r = someVector[index]]() { r = 100; };
Here you’re capturing a reference to an element in a container.
You cannot write [&someVector[index]] — not allowed.
So init-capture with &r = expr is the only way.
*/

/* Evaluating happen at when lambda expression define
When we call lambda expression invoking happen*/


// ---------------------------------  C++20
/*  Inordered to support c++ concepts */

    // ?????????????? Lambdas in template context, constexpr + template parameters, default constructible lambdas
    // for STL algorithms, nested lambdas, or class-based scenarios!
int main() {
    T();
    T2();
    Test();
    T3();
    T4();
    T5();

    Sort();
    // Variadic();
}