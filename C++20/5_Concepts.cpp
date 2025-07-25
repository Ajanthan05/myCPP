#include <concepts>
#include <ranges>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

#include <type_traits>
#include <ranges>
/*  Concepts are named, compile-time predicates that express constraints on template parameters.

They help the compiler (and the developer) check
    Before C++20, templates would produce cryptic compiler errors when the wrong type was passed

| Concept                         | Description                           |
| ------------------------------- | ------------------------------------- |
| `std::integral<T>`              | `T` is an integer type                |
| `std::floating_point<T>`        | `T` is a floating-point type          |
| `std::same_as<T, U>`            | `T` and `U` are exactly the same type |
| `std::convertible_to<T, U>`     | T can be converted to U               |
| `std::derived_from<T, U>`       | T derives from U                      |
| `std::invocable<F, Args...>`    | F can be called with Args...          |
| `std::default_initializable<T>` | Can be `T{}` initialized              |
*/

// Concept  Internal
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};
/*  requires clause checks that a + b is valid
And that the result is of type T */


// Ways to Use Concepts

// 1. Trailing requires clause
template<typename T>
requires std::integral<T>
void func(T value);

// 2. Inline requires expression
template<typename T>
void func(T value) requires std::integral<T>;

// 3. Constrained template parameters
template<std::integral T>
void func(T value); // clean and expressive

// 4. Concept in a requires block
template<typename T>
concept Printable = requires(T a) {
    std::cout << a;
};

/*  requires Expression Syntax
cpp
Copy
Edit
requires(Type) {
    { expression } -> return constraint;
};
*/
template<typename T>
concept Incrementable = requires(T x) {
    { ++x } -> std::same_as<T&>;
    { x++ } -> std::same_as<T>;
};

/*  
| Feature                 | Description                                 |
| ----------------------- | ------------------------------------------- |
| ✅ Cleaner Syntax        | Replaces verbose SFINAE or `enable_if`      |
| ✅ Better Error Messages | Compiler tells *why* something doesn't work |
| ✅ Safer Generic Code    | Templates won’t compile with invalid types  |
| ✅ More Readable         | Self-documenting constraints                |
*/


template<std::ranges::range R>
void print_all(const R& range) {
    for (auto&& val : range)
        std::cout << val << " ";
}
/*Only types satisfying ranges::range will compile
You get cleaner diagnostics if someone passes an invalid type */

// Combine Concepts
template<typename T>
concept Num = std::integral<T> || std::floating_point<T>;

template<Num T>
T square(T x) {
    return x * x;
}


// 1
template <typename T, typename R>
requires (sizeof(T) == sizeof(R))
R someFun1(T data) {};

// 2
template <typename A, typename B>
concept SameSize = (sizeof(A) == sizeof(B));

template <typename T, typename R>
requires SameSize<T, R>
R someFun2(T data) {};


// T uses a concept
// template <typename T>
// requires std::is_floating_point<T> //is incorrect because std::is_floating_point<T> is not a concept — it's a type trait that yields a bool.
// void soneThing12(T data) {}

template <typename T>
requires std::is_floating_point_v<T>
void soneThing1_(T data) {}

template <std::floating_point T>
void soneThing1(T data) {}


// T uses a type trait as a constraint
template <typename T>
requires (std::is_pointer_v<T>)
void soneThing2(T data) {}

// ✅ With concept:
// #include <concepts>
template<std::floating_point T>
void doSomething(T x) { }

// ❌ With type trait:
// #include <type_traits>
template<typename T>
requires std::is_floating_point_v<T>
void doSomething(T x) { }


template<typename T>
concept Sortable = requires(T a) {
    { std::begin(a) } -> std::input_iterator;
    { std::end(a) } -> std::sentinel_for<decltype(std::begin(a))>;
    { std::sort(std::begin(a), std::end(a)) };
};
template<typename T>
concept Sortable_Update = requires(T a) {
    typename T::iterator;
    requires std::random_access_iterator<typename T::iterator>;
    { std::sort(a.begin(), a.end()) };
};
template<Sortable_Update T>
void sort_and_print(T& container) {
    std::sort(container.begin(), container.end());
    for (auto x : container)
        std::cout << x << " ";
    std::cout << "\n";
}
void T() {
    std::vector<int> v = {4, 2, 5, 1};
    sort_and_print(v); // ✅ works

    std::list<int> l = {4, 2, 5, 1};
    // sort_and_print(l); // ❌ compile-time error (list not random access)
}
/*` { std::begin(a) } -> std::input_iterator;
Checks that calling std::begin(a) is valid.
Ensures the result is an input iterator (e.g., you can read elements by iterating).
This ensures the container supports iteration.
ℹ️ std::input_iterator is a concept defined in <iterator>.


{ std::end(a) } -> std::sentinel_for<decltype(std::begin(a))>;
Checks that std::end(a) is valid.
std::sentinel_for ensures the result of end(a) can be used to compare
 against the iterator returned by begin(a) (i.e., can terminate iteration).
🧠 Sentinel = something that "marks the end" of a range.


{ std::sort(std::begin(a), std::end(a)) };
Checks that std::sort(begin(a), end(a)) is a valid expression for this container.
Ensures the elements are sortable using std::sort, which requires:
Random access iterators
Elements being comparable via <
*/
int main() {
    T();
}