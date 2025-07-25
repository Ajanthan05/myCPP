#include <iostream>
#include <functional>
#include <memory>
#include <tuple>

using namespace std;

/*  std::function is a powerful type-erased function wrapper introduced in C++11. 
It can store and call any callable object: function pointers, lambdas, std::bind expressions, and function objects.
std::function<ReturnType(Args...)> name;
| Callable Type                     | Example                                |
| --------------------------------- | -------------------------------------- |
| Function pointer                  | `int (*)(int, int)`                    |
| Lambda                            | `[](int a, int b) { return a + b; }`   |
| Functor (class with `operator()`) | `struct { int operator()(...) {...} }` |
| `std::bind` result                | `std::bind(&Class::method, obj, ...)`  |
*/

int add(int a, int b) { return a + b; }
void T() {
    std::function<int(int, int)> f = add;
    std::cout << f(2, 3) << "\n"; // 5

    // LAMBDA
    // ✅ Capturing lambdas can’t be converted to raw function pointers but work perfectly with std::function.
    int factor = 10;
    std::function<int(int, int)> lam = [&factor](int a, int b) { return (a+b) * factor; };
    std::cout << lam(2, 3) << "\n"; // 5
}

// Functor (Object with operator())
struct Multiply {
    int operator()(int x, int y) const { return x * y; }
};
void Functor() {
    std::function<int(int, int)> f = Multiply();
    std::cout << f(3, 4); // 12
}


// std::bind Usage
struct Calc {
    int mult(int x, int y) { return x * y; }
};

// Calc c;
// std::function<int(int)> f = std::bind(&Calc::mult, c, 5, std::placeholders::_1);
// std::cout << f(3); // 15

/* 
| Benefit                      | Explanation                                  |
| ---------------------------- | -------------------------------------------- |
| Type-erased                  | Handles any callable with matching signature |
| Flexibility                  | Swap different logic at runtime              |
| Capture support              | Stores lambdas with captured variables       |
| Better abstraction           | Ideal for callbacks, events, and interfaces  |
| Safer than function pointers | Works well with RAII and modern C++ patterns |

⚠️ Things to Know
Performance Cost
std::function is heavier than raw function pointers — small allocation may occur.

Default state is empty

cpp
Copy
Edit
std::function<void()> f;
if (!f) std::cout << "f is empty";
Supports Copy/Move
Can be passed, returned, stored in containers.*/


// 🧪 Real-World Use Cases
// 1. CALLBACK SYSTEM
void run(std::function<void()> callback) {
    std::cout << "Before\n";
    callback();
    std::cout << "After\n";
}
void T2() {
    run([] { std::cout << "Inside callback\n"; });
}
// 2. Dynamic Strategy
void T3() {
    std::function<int(int, int)> operation;

    operation = [](int a, int b) { return a + b; };
    std::cout << operation(1, 2); // 3

    operation = [](int a, int b) { return a * b; };
    std::cout << operation(1, 2); // 2
}
/*
| Use Case             | Prefer `std::function`                   |
| -------------------- | ---------------------------------------- |
| Runtime strategy     | ✅ Yes                                    |
| Lambda with capture  | ✅ Yes                                    |
| Type-erased callback | ✅ Yes                                    |
| High-performance     | ❌ Use raw pointer or templated callables |
*/

class function_base {
public:
    virtual void call() = 0;
    virtual ~function_base() = default;
};

template<typename Callable>
class function_wrapper : public function_base {
    Callable c;
public:
    function_wrapper(Callable cb) : c(std::move(cb)) {}
    void call() override { c(); }
};

class my_function {
    std::unique_ptr<function_base> fn;
public:
    template<typename Callable>
    my_function(Callable cb)
        : fn(std::make_unique<function_wrapper<Callable>>(std::move(cb))) {}

    void operator()() {
        fn->call();  // Uniform call
    }
};


/*
| Feature / Use Case                            | `std::function`                                  | Function Pointer                     | Templated Callable (e.g., `auto`, `template<typename F>`) |
| --------------------------------------------- | ------------------------------------------------ | ------------------------------------ | --------------------------------------------------------- |
| 🧠 **Stores lambdas with capture**            | ✅ Yes                                            | ❌ No                                 | ✅ Yes                                                     |
| 🧲 **Stores function pointer**                | ✅ Yes                                            | ✅ Yes                                | ✅ Yes                                                     |
| 🎭 **Stores functor/class with `operator()`** | ✅ Yes                                            | ❌ No                                 | ✅ Yes                                                     |
| 🔁 **Runtime polymorphism**                   | ✅ Type-erased                                    | ✅ Limited                            | ❌ Compile-time only                                       |
| 🏎️ **Performance (overhead)**                 | ❌ Higher (may heap-allocate)                     | ✅ Very fast                          | ✅ Fast (inlined, zero overhead)                           |
| 🧳 **Copyable/Movable**                       | ✅ Yes                                            | ✅ (pointer copy)                     | ✅ If the callable type supports it                        |
| 🔒 **Type safety**                            | ✅ Strong                                         | ✅ Weak                               | ✅ Strong                                                  |
| 📦 **Stored in container**                    | ✅ Easy (e.g., `std::vector<std::function<...>>`) | ✅ Yes (less expressive)              | ❌ Hard (template bloat, type erasure needed)              |
| 🧪 **Use in callback/event system**           | ✅ Best choice                                    | ✅ OK if no state                     | ❌ Complicated, unless using wrapper                       |
| 🧬 **Custom behavior injection**              | ✅ Ideal                                          | ✅ If fixed signature                 | ✅ Best for static dispatch                                |
| 🧼 **Supports default empty state**           | ✅ Yes (`!func`)                                  | ❌ Not applicable (UB if null called) | ❌ Needs `std::optional` or manual                         |
| 🧰 **Use in virtual interface**               | ✅ Possible in non-virtual design                 | ❌ No                                 | ❌ Not directly                                            |


| You Need...                         | Use This           |
| ----------------------------------- | ------------------ |
| Dynamic callable (runtime chosen)   | `std::function`    |
| Minimal overhead and known function | Function Pointer   |
| Max performance and inlining        | Templated Callable |
| Store a variety of callables safely | `std::function`    |
| Interface-less injection            | Templated Callable |
| Event system / signal-slot          | `std::function`    |
*/



/*  std::bind is a utility that creates a callable object by binding arguments to a 
function, optionally rearranging or partially applying them.*/

// Print All Arguments
template<typename T>
void print_one(T arg) {
    cout << arg << "\n";
}
template<typename T, typename... Rest>
void print_all(T head, Rest... tail) {
    cout << head << " ";
    print_all(tail...);
}
// Base Case
template<typename T>
void print_all(T last) {
    print_one(last); // Base case ends recursion
}

/*      ✅ Optimized Version Using Fold Expression (C++17+)
((std::cout << arg1 << " ") << arg2 << " ") << ...
It avoids recursion and generates highly efficient code.*/
template<typename... Args>
void print_all17(Args&&... args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}
// If you're limited to C++11/14, you can still avoid multiple overloads using an initializer list trick
template<typename... Args>
void print_all11(Args... args) {
    using expander = int[];
    (void)expander{0, (std::cout << args << " ", 0)...};
    std::cout << "\n";
}

// C++14
template<typename... T>
auto sum(T... t) {
    typename std::common_type<T...>::type result{};
    (void)std::initializer_list<int>( (result += t, 0)...);
    return result;
}

// C++17
template<typename... T>
auto sum17(T... t) {
    return ( t + ... );
}
// Safeguard Average with Zero-Check
template<typename... T>
auto avg(T... t) {
    static_assert(sizeof...(t) > 0, "At least one argument required");
    return (t + ...) / sizeof...(t);
}
/*  
| Expression        | Meaning                           |
| ----------------- | --------------------------------- |
| `sizeof(T)`       | Size in **bytes**                 |
| `sizeof...(T)`    | Number of types in parameter pack |
| `sizeof...(args)` | Number of passed arguments        |
*/

template<typename... T>
auto devid(T... t) {
    return ( ... / t );
}

template<typename... Args>
void print_all_O(Args&&... args) {
    (std::cout << ... << args) << "\n";  // Left fold with <<
}

// Execute Multiple Functions
template<typename... Callables>
void invoke_all(Callables&&... fns) {
    (..., fns());  // comma operator fold
}
void T_Fold() {
    cout << "devid: " << devid(100, 2, 5) << "\n";
    print_all17("Hi", " Ajanthan");

    invoke_all(
        [] { std::cout << "One\n"; },
        [] { std::cout << "Two\n"; },
        [] { std::cout << "Three\n"; }
    );
}

/*  
Left Fold:  (... op args)
(... + args) → (((arg1 + arg2) + arg3) + ...) + argN
(... / args) → (((arg1 / arg2) / arg3) / ...) / argN

Right Fold: (args op ...)
(args + ...) → arg1 + (arg2 + (... + argN))
(args / ...) → arg1 / (arg2 / (... / argN))

Left fold:
(... / args) → (((100 / 2) / 5) / 2) = 5

Right fold:
(args / ...) → 100 / (2 / (5 / 2)) = 100 / (2 / 2.5) = 100 / 0.8 = 125

*/

// obj.*calls is used to call a member function pointer on the object obj
struct MyClass {
    void hello() const { std::cout << "Hello\n"; }
    void world() const { std::cout << "World\n"; }
    void exclaim() const { std::cout << "!\n"; }
};
template<typename T, typename... Callers>
void call_chain(T& obj, Callers... calls) {
    (..., (obj.*calls)());  // Fold with comma
}
void T_objCall() {
    MyClass obj;
    call_chain(obj, &MyClass::hello, &MyClass::world, &MyClass::exclaim);
}



// Variadic Template Class Example
template<typename... Ts>
struct Tuple;

template<typename Head, typename... Tail>
struct Tuple<Head, Tail...> {
    Head head;
    Tuple<Tail...> tail;
};

template<>
struct Tuple<> {}; // Base case

//  std::forward + std::move with Variadic Templates
// template<typename... Args>
// void emplace(Args&&... args) {
//     T obj(std::forward<Args>(args)...); // Perfect forwarding
// }
// before ‘obj’85 |     T obj(std::forward<Args>(args)...); // Perfect forwarding

// DSLs and static interfaces	Compile-time reflection, static dispatch
// std::tuple, std::variant	Store multiple types

// Advanced: Indexed Expansion
// You can use std::index_sequence for advanced patterns like unpacking tuples


// examples of variadic inheritance (CRTP with multiple bases)?


template<std::size_t... Is, typename Tuple>
void print_tuple_impl(const Tuple& t, std::index_sequence<Is...>) {
    ((std::cout << std::get<Is>(t) << " "), ...);
}


// "initializer list trick" 
/*  expander{0, (do_something(arg), 0)...};:

{0, ..., 0}: the first 0 ensures the list is never empty.

(do_something(arg), 0): executes do_something(arg) and returns 0.

The comma operator ensures each call is evaluated and expands args....

The cast to (void) silences unused variable warnings.*/
template<typename... Args>
void print_all(Args... args) {
    using expander = int[];
    (void)expander{0, (std::cout << args << " ", 0)...};
    std::cout << "\n";
}
void T_Initializer_List() {
    print_all(1, "hi", 3.14);  // 1 hi 3.14
}


int main() {
    T();
    T_Fold();
    T_objCall();
}