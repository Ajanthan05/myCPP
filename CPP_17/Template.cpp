#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/*  decltype is a keyword in C++ used to query the type of an expression at compile time.
decltype(expression) variableName;
This declares variableName with the same type as expression.

    When It's Useful
Type deduction in templates or auto-like scenarios.
Avoiding repeating types manually.
Working with complex expressions.
*/

/* variadic template:- allows a function or class to accept any number and any types of arguments.
Args... is a template parameter pack. (List of types)
*/

void Decltype() {
    int a = 5;
    int& b = a;

    decltype(a) x = 3;  // x is int type
    decltype(b) y = a;  // y is int& (reference)

    decltype((a)) z = a;    // z is int& — notice the extra parentheses!
    // If you wrap a variable in parentheses, it’s treated as an lvalue expression, so decltype gives you a reference type.

}

// With Classes
struct MyClass {
    int getValue() { return 42; }
};

void With_class() {
    MyClass obj;

    decltype(obj.getValue()) val;  // val is of type int
    val = obj.getValue();
}

// Decltype in Templates
template<typename T, typename U>
auto multiplay(T a, U b) -> decltype (a*b) {
    return (a,b);
}

/* What is std::declval?
std::declval<T>() is a way to pretend to have a T without constructing one. It returns an rvalue reference of type T&&, and you use it only in unevaluated contexts, like inside decltype
Because sometimes you need to deduce return types or member types in templates without creating real objects.*/

struct Person {
    std::string getName() const { return "Alice"; }
};

template <typename T>
auto getNameType() -> decltype(std::declval<T>().getName());

void Declval() {
    using NameType = decltype(getNameType<Person>());
    NameType name = "Bob";  // NameType is std::string
    std::cout << name << std::endl;
}

//////////////// Template /////////////////////
template <typename... Args>
void func(Args... args) {
    // Args... becomes <int, double, const char*, std::string>.
    // args... becomes the actual values passed.

}

void Parameter_pack() {
    func(1, 3.14, "hello", std::string("world"));
}

// In std::forward<Args>(args)...
template <typename... Args>
void call_func(Args&&... args) {
    func(std::forward<Args>(args)...);
    // Args&&... is a forwarding reference pack (Perfact for universal references)
    // std::forward<Args>(args)... expands into
    // std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), ...

}

// Logging Utility (Variadic Print) A logging function that prints any number of arguments.
void log() {
    std::cout << std::endl;
}

template <typename T, typename... Args>
void log(T&& first, Args&&... rest) {
    std::cout << std::forward<T>(first) << " ";
    log(std::forward<Args>(rest)...);
}


int log_test() {
    // Flexible: Supports any number and type of arguments.
    log("Error:", 404, "Not Found", 3.14);
}


/*  Container is itself a template (like std::vector, std::list).
    typename... allows support for variadic template containers
    
    A template template parameter allows you to pass a template class 
    (like std::vector or std::list) as an argument to another class or function template.
*/

// variadic templates (...).
template < template <typename...> class Container, typename T>
class Wrapper {
    Container<T> data;

public: 
    void add(const T& value) {
        data.push_back(value):
    }

    void print() const {
        for (const T& item : data)
            std::cout << item << " ";
        std::cout << "\n";
    }
};

void TestWrapper() {
    Wrapper<std::vector, int> v;
    v.add(10); v.add(20); v.print();  // 10 20

    Wrapper<std::list, int> l;
    l.add(1); l.add(2); l.print();    // 1 2
}

template <template<typename, typename> class Map, typename K, typename V>
class MapWrapper { };
