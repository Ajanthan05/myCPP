#include <concepts>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>

#include <type_traits>
#include <ranges>
#include <cassert>

/*  Concepts are named, compile-time predicates that express constraints on template 
parameters.

They help the compiler (and the developer) check
    Before C++20, templates would produce cryptic compiler errors when the wrong 
    type was passed

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

template <typename T>
requires std::integral<T> // constraint
T add(T a, T b) {
    return a + b;
}

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



// template <class T, T v>
// struct Integral_constant {
//     static constexpr T value = v;
// };
/*  This is a generic “box” that stores a constant at compile time.

Example: integral_constant<bool, true>::value == true.

The STL defines it exactly like this — many traits are built on top of it. */

// Aliases for true/false types
// using true_type  = Integral_constant<bool, true>;
// using false_type = Integral_constant<bool, false>;
/*  Now instead of writing integral_constant<bool, true> everywhere, 
you just use true_type or false_type.

Clean, readable, matches what the STL does.*/

// Primary template for is_pointer
// template <class T>
// struct in_pointer : false_type {};
// By default, assume T is not a pointer.
// Example: is_pointer<int>::value == false.

// Specialization for pointer types
// template<class T>
// struct is_pointer<T*> : true_type {};
// If the type is T* (pointer to T), then is_pointer<T*>::value == true.
// Example: is_pointer<int*>::value == true.

//  Test with static_assert
// static_assert(is_pointer<int*>::value);      // ✅ passes
// static_assert(not is_pointer<int>::value);   // ✅ passes
// Both compile-time checks succeed.
// If you swapped them, compilation would fail.

template <class T, T v>
struct integral_constant {
    static constexpr T value = v;      // the compile-time constant
    using value_type = T;              // its type
    using type = integral_constant<T, v>;  // alias for itself

    // This is an implicit conversion operator.
    constexpr operator value_type() const noexcept { return value; }

    // Function Call Operator
    constexpr value_type operator()() const noexcept { return value; }
};
/*  🔹 What It Does
Stores a constant value at compile time.
Provides it as ::value.
Can be used in places like static_assert, SFINAE, enable_if, etc.*/
void T_integral_constant() {
    using five_t = integral_constant<int, 5>;
    using true_t = integral_constant<bool, true>;

    std::cout << five_t::value << "\n";
    std::cout << true_t::value << "\n";

    static_assert(five_t::value == 5);
    static_assert(true_t::value);

    // Implicit conversion to value_type
    five_t x;
    int n = x;  // calls operator value_type(), so n == 5
    std::cout << "n: " << n << "\n"; 
    /*  It allows an integral_constant<T, v> object to be used as if it were a T.
    This works without needing f.value.*/
    static_assert(five_t{} == 5);   // works, f converts to int

    /*  2. Function Call Operator
constexpr value_type operator()() const noexcept { return value; }
    This makes the object callable like a function that returns the constant.

Together, they make std::integral_constant behave both like:
A value: five_t{} == 5
A function object: five_t{}() == 5    */
    int n2 = x(); // call operator()(), return 5
    static_assert(five_t{}() == 5);
}
/*  type traits.
std::true_type   // which is just integral_constant<bool, true>
std::false_type  // which is just integral_constant<bool, false>
std::is_pointer<int*>::value   // true
std::is_pointer<int>::value    // false*/

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// C: Primary template (for any type T)
template<class T>
struct is_pointer : false_type {};

// D: Partial specialization for T*
template<class T>
struct is_pointer<T*> : true_type {};

static_assert(is_pointer<int*>::value);   // ok
static_assert(!is_pointer<int>::value);   // ok

template <typename T>
auto getValue(T t) {
    if constexpr(is_pointer<T>::value) {
        assert(nullptr != t);
        return *t;
    } else {
        return t;
    }
} 
void T_getValue() {
    int  i  = 5;
    int* ip = &i;

    auto iv  = getValue(i);
    auto ipv = getValue(ip);
    auto itv = getValue(11);
}

/* Error
template <class From, class To>
struct to_convertable : integral_constant<
                            bool,  __is_convertible_to(From, To) // compiler buildin
                        > {};

template <class From, class To>
inline constexpr bool is_convertable_v = is_convertable<From, To>::value;

template <class From, class To>
struct is_convertible
    : std::integral_constant<
          bool,
          __is_convertible_to(From, To) // compiler builtin
      > {};


template <typename From, typename To>
struct is_convertible
#if defined(__clang__) || defined(__GNUC__)
    : std::bool_constant<__is_convertible_to(From, To)> {};
#else
    : std::is_convertible<From, To> {};
#endif

template <class From, class To>
inline constexpr bool is_convertible_v = is_convertible<From, To>::value;*/

using std::is_convertible_v; 

template <typename T>
std::string str(T val) {
    if constexpr (is_convertible_v<T, std::string>) {   // ✅ works
        return static_cast<std::string>(val);
    } else {
        return "not convertible";
    }
}

void Test_is_convertable_v() {
    auto s = str(std::string({"11"}));
    auto i = str(5);

        // implicit conversions
    std::cout << is_convertible_v<int, double> << "\n";     // true
    std::cout << is_convertible_v<double, int> << "\n";     // true
    std::cout << is_convertible_v<int*, void*> << "\n";     // true
    std::cout << is_convertible_v<void*, int*> << "\n";     // false

    // not convertible
    std::cout << is_convertible_v<std::string, int> << "\n"; // false
}

// Step 1 – Primary template
template<class T> 
struct is_reference : false_type {};
// By default, for any type T, is_reference<T> inherits from false_type.
// That means is_reference<T>::value == false.
// Step 2 – Partial specializations
template<class T> struct is_reference<T&> : true_type {};
template<class T> struct is_reference<T&&> : true_type {};
// These are partial specializations of the template.
// If T is an lvalue reference (T&), this specialization is chosen, and it inherits from true_type.
// If T is an rvalue reference (T&&), this specialization is chosen, and it also inherits from true_type.

// Step 3 – Variable template shortcut
template<class T>
inline constexpr bool is_reference_v = is_reference<T>::value;

// is_same
template <class T, class U>
struct is_same : false_type {};

template <class T>
struct is_same<T, T> : true_type {};

template <class T, class U>
inline constexpr bool is_same_v = is_same<T, U>::value;

template<class T> struct remove_reference       { using type = T; };
template<class T> struct remove_reference<T&>   { using type = T; };
template<class T> struct remove_reference<T&&>  { using type = T; };

template<class T>
using remove_reference_t = remove_reference<T>::type;

static_assert( is_same_v< remove_reference_t<int&>, int> );
static_assert( is_same_v< remove_reference_t<int>, int> );

template <class T> struct add_lvalue_reference1 { using type = T&; };
template <class T> 
using add_lvalue_reference_t1 = typename add_lvalue_reference1<T>::type;

static_assert( is_same_v< add_lvalue_reference_t1<int&&>, int& >);
/*  why does int&& (rvalue reference) turn into int& (lvalue reference) 
when wrapped in add_lvalue_reference_t?

Reference-collapsing rules

C++ has reference collapsing rules that apply when references are combined:
T& & → T&
T& && → T&
T&& & → T&
T&& && → T&&    */
static_assert( is_same_v< add_lvalue_reference_t1<int&>, int& >);
static_assert( is_same_v< add_lvalue_reference_t1<int>, int& >);

/* Here we have trouble. When we try to form the type void&, the compiler gives
us an error and dies. We must prevent the type void& from being formed.

static_assert( is_same_v< add_lvalue_reference_t< void >, void >); 

// The wrong fix
template<class T> struct add_lvalue_reference { using type = T&; };
template<> struct add_lvalue_reference<void> { using type = void; };
template<> struct add_lvalue_reference<const void> { ........... };
template<> struct add_lvalue_reference<volatile void> { ....... };
template<> struct add_lvalue_reference<const volatile void> { ... }; */


// VOID_T
// template<class...> using void_t = void;
/*  Whatever you give it (void_t<int>, void_t<int&>, void_t<T* const volatile> …) → it always becomes void.

Its magic is that if the expression inside void_t<...> is ill-formed, the compiler won’t error out — instead SFINAE kicks in and removes that specialization.

👉 This makes void_t a powerful tool for “detection” (checking whether a type or expression is valid).*/

// template<class T, class Enable>
// struct ALR_impl                  { using type = T; };

// template<class T>
// struct ALR_impl<T, void_t<T&>>  { using type = T&; };

// template<class T>
// struct add_lvalue_reference : ALR_impl<T, void> {};

/*Primary template (ALR_impl<T, Enable>): if no specialization matches, just return T.

Specialization (ALR_impl<T, void_t<T&>>): if T& is a valid type, use T&.

Wrapper: add_lvalue_reference<T> calls ALR_impl<T, void>, which either falls back or picks the specialization.

Examples:

add_lvalue_reference<int>
→ ALR_impl<int, void>
→ specialization matches because int& is valid
→ result: int&.

add_lvalue_reference<int&>
→ ALR_impl<int&, void>
→ specialization matches ((int&)& collapses to int&)
→ result: int&.

add_lvalue_reference<void>
→ ALR_impl<void, void>
→ but here void& is not a valid type
→ specialization ALR_impl<T, void_t<T&>> is discarded
→ falls back to primary: result: void.

This matches exactly the standard rule: add_lvalue_reference<void> → void.


//////////////////////////////////////////
void_t is a tool for mass production
template<class...> using void_t = void;
template<class T, class>    struct ALR_impl { using type = T; };
template<class T>           struct ALR_impl<T, void_t<T&>> { using type = T&; };
template<class T, class>    struct ARR_impl { using type = T; };
template<class T>           struct ARR_impl<T, void_t<T&&>> { using type = T&&;};

template<class T, class>    struct AP_impl { using type = T; };
template<class T>           struct AP_impl<T, void_t<T*>> { using type = T*; };
template<class T>           struct add_lvalue_reference : ALR_impl<T, void> {};
template<class T>           struct add_rvalue_reference : ARR_impl<T, void> {};
template<class T>           struct add_pointer : AP_impl<T, void> {};

But what if our maybe-ill-formed thing is a value-space expression, not a
type-expression? Like not “T&” but something like “a = b”???  */

template<class...> using void_t = void;
template<class T, class>    struct ALR_impl { using type = T; };
template<class T>           struct ALR_impl<T, void_t<T&>> { using type = T&; };
template<class T, class>    struct ARR_impl { using type = T; };
template<class T>           struct ARR_impl<T, void_t<T&&>> { using type = T&&;};

template<class T, class>    struct AP_impl { using type = T; };
template<class T>           struct AP_impl<T, void_t<T*>> { using type = T*; };
template<class T>           struct add_lvalue_reference : ALR_impl<T, void> {};
template<class T>           struct add_rvalue_reference : ARR_impl<T, void> {};
template<class T>           struct add_pointer : AP_impl<T, void> {};



// 
template <class A, class B>
auto findMin(A a, B b) -> decltype(a < b ? a : b) {
    return (a < b) ? a : b;
}
int fun1() { return 10; }
char fun2() { return 'g'; }
void T_decltype() {
    
    // This call returns 3.44 of double type
    std::cout << findMin(4, 3.44) << std::endl;

    // This call returns 3 of double type
    std::cout << findMin(5.4, 3) << std::endl;

    decltype(fun1()) x;
    decltype(fun2()) y;
    std::cout << "typeid(x): " << typeid(x).name() << "\n";
    std::cout << "typeid(y): " << typeid(y).name() << "\n";

    auto x1 = 4;
    auto y1 = 3.37;
    auto z = 3.37f;
    auto c = 'a';
    auto ptr = &x;
    
    //pointer to a pointer
    auto pptr = &ptr; 
    std::cout << typeid(x1).name() << std::endl
         << typeid(y1).name() << std::endl
         << typeid(z).name() << std::endl
         << typeid(c).name() << std::endl
         << typeid(ptr).name() << std::endl
           << typeid(pptr).name() << std::endl;

}

/*  "declval" is a function whose declaration exist but definition doesn't
Boc we dont need it

declval turns a type into a value, for the purposes of unevaluated expressions.
It’s a function with no definition, just a return type. And we use it like this*/
template<class T>
auto declval() noexcept -> add_rvalue_reference_t<T>;

template<class T, class U>
using assignment_


int main() {
    T();
    T_integral_constant();
    Test_is_convertable_v();
    T_decltype();
}