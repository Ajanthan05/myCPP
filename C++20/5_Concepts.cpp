#include <concepts>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>

#include <type_traits>
#include <ranges>
#include <cassert>
#include <cstring>
#include <memory_resource>
#include <cstdint>

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

template <class T> 
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
template <class T> 
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;


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
using assignment_result_t = decltype( declval<T>() = declval<U>() );

static_assert( is_same_v< assignment_result_t<int&, double>, int& >);
// static_assert( is_same_v< assignment_result_t<int&, int*>, ill-formed >);
/*  int& = double → allowed, because you can assign a double to an int reference.
The expression type is int& (since assignment returns the left-hand side by reference).
Therefore, assignment_result_t<int&, double> = int&.

int& = double → allowed, because you can assign a double to an int reference.

The expression type is int& (since assignment returns the left-hand side by reference).

Therefore, assignment_result_t<int&, double> = int&.*/

// “Expression SFINAE”
/*  SFINAE:- If a substitution inside a template parameter list fails, it doesn’t 
trigger a hard error — it’s just ignored (Substitution Failure Is Not An Error).*/

// Primary template: assume it's false
template<class T, class U, class Enable>
struct is_assignable_impl : false_type {};
// Enable is just a placeholder for SFINAE.

// Specialization: enabled only if "declval<T>() = declval<U>()" is valid
template<class T, class U>
struct is_assignable_impl<T, U, 
    decltype(void( declval<T>() = declval<U>() ))> : true_type {};
/*  decltype(expr) normally yields the type of an expression.
Here we wrap it in void(...) so it always becomes void if it compiles.
If declval<T>() = declval<U>() is a valid expression → this specialization is chosen → derives from true_type.
If not → substitution fails → compiler falls back to the primary (false_type).*/

// Final user-facing trait
template<class T, class U>
struct is_assignable : is_assignable_impl<T, U, void> {};

// Passes void as the Enable argument, so the specialization is attempted.
// Result: true_type if T can be assigned from U, otherwise false_type.
static_assert(is_assignable<int&, int>::value, "ok");   // ✅ true
static_assert(!is_assignable<int, int>::value, "fail"); // ❌ false, can't assign to prvalue int
static_assert(is_assignable<std::string&, const char*>::value, "ok"); // ✅




template<class T, class U, class>
struct ISC_impl : false_type {};
// This is the fallback case: if substitution (SFINAE) fails, we end up here.
// By default, assume "T is NOT static_castable to U".

template<class T, class U>
struct ISC_impl<T, U, decltype(void(
    static_cast<U>(declval<T>())
))> : true_type {};
template<class T, class U>
struct is_static_castable : ISC_impl<T, U, void> {};

void T_is_static_castable() {
    std::cout << std::boolalpha;
    
    // int → double is valid
    std::cout << is_static_castable<int, double>::value << "\n";  // true
    
    // double* → int* is invalid (not allowed by static_cast)
    std::cout << is_static_castable<double*, int*>::value << "\n";  // false
    
    // void* → int* is valid with static_cast
    std::cout << is_static_castable<void*, int*>::value << "\n";  // true
}

template<class T, class> struct IP_impl : false_type {};
template<class T> struct IP_impl<T, decltype(
 dynamic_cast<void*>(declval< std::remove_cv_t<T>*>())
)> : true_type {};
template<class T>
struct is_polymorphic : IP_impl<T, void*> {};
/*  remove_cv_t<T>* removes const/volatile, then makes a pointer.

declval<...>() gives you an rvalue of that type without constructing it.

dynamic_cast<void*>(ptr) is only valid if T is polymorphic (i.e., has at least one virtual function).

If T is not polymorphic, dynamic_cast to void* is ill-formed, so substitution fails → SFINAE → specialization is discarded.

If T is polymorphic, the dynamic_cast is valid, decltype(...) works, and we pick this specialization.

So this branch makes IP_impl<T, something> inherit from true_type.*/
struct P {};                // no virtual → not polymorphic
struct Q { virtual ~Q(); }; // has virtual → polymorphic
static_assert(!is_polymorphic<P>::value);
static_assert(is_polymorphic<Q>::value);

// ----->>>>>>>> Because dynamic_cast to void* is only legal for polymorphic types.


// -----------------------------------------------------------------------------

template<bool B, class T = void>
struct enable_if {}; // empty if condition is false

template<class T>
struct enable_if<true, T> { using type = T; };

template<bool B, typename T = void>
using enable_if_t = enable_if<B, T>::type;
// If B == true → enable_if<true, T>::type is T.
// If B == false → enable_if<false, T>::type is not defined (ill-formed), so the compiler removes that overload from consideration.

template<typename T>
enable_if_t<std::is_integral_v<T>, void> foo(T) {
    std::cout << "Integral type\n";
}

template<typename T>
enable_if_t<std::is_floating_point_v<T>, void> foo(T) {
    std::cout << "Floating point type\n";
}


void T_enable_if_t() {
    foo(5);    // Integral type
    foo(3.14); // Floating point type
}


// Step 1: Convert expression to boolean trait
template <typename, typename = void>
struct has_push_back : false_type {};

template <typename T>
struct has_push_back<T, decltype(void(declval<T>().push_back(0)))> : true_type {};

// Step 2: Use boolean trait in enable_if
template <typename T>
enable_if_t<has_push_back<T>::value>
bar(T& t) {
    t.push_back(42); // only compiles if push_back exists
}



template<typename T>
void copy(T* dst, const T* src, size_t n, std::true_type /* trivially copyable */) {
    std::memcpy(dst, src, n * sizeof(T));
    std::cout << "Used memcpy\n";
}

template<typename T>
void copy(T* dst, const T* src, size_t n, std::false_type /* not trivially copyable */) {
    for (size_t i = 0; i < n; i++) dst[i] = src[i];
    std::cout << "Used element copy\n";
}

template<typename T>
void copy(T* dst, const T* src, size_t n) {
    copy(dst, src, n, std::is_trivially_copyable<T>{});
}

struct Complex {
    Complex() {}
    Complex(const Complex&) { std::cout << "copy ctor\n"; }
};

void Optimizing_code_paths() {
    int a[3] = {1,2,3}, b[3];
    copy(b, a, 3); // ✅ memcpy

    Complex c[3], d[3];
    copy(d, c, 3); // ✅ element copy
}



// ----------------------REAL WORL   HAS_SIZE-----------------------------
template<typename T, typename = void>
struct has_size : false_type {};

template<typename T>
struct has_size<T, void_t<decltype(declval<T>().size())>> : true_type {};

// using creates a type alias, but has_size<T>::value is a bool, not a type.
// That’s why the compiler complained about “expected a constant of type bool”.
template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;


template<typename T>
enable_if_t<has_size_v<T>, int> 
print_size(const T& t) {
    std::cout << "Size: " << t.size() << "\n";
    return static_cast<int>(t.size());
}
template<typename T>
enable_if_t<!has_size_v<T>, int> 
print_size(const T& ) {
    std::cout << "No Size available\n";
    return -1;  // return sentinel value
}
// Detecting member functions (duck typing in C++)
void Duck_typing() {
    std::vector<int> v{1,2,3};
     int s1 = print_size(v); // ✅ calls version with .size()

    int x = 42;
    int s2 = print_size(x); // ✅ calls fallback
    
    std::cout << "Returned values: " << s1 << ", " << s2 << "\n";
}



// --------------------------------------------------------
void Memory_Resource() {
    std::byte stackBuf[2048];
    std::pmr::monotonic_buffer_resource rsrc(stackBuf, sizeof stackBuf);
    std::pmr::vector<int> v{{1,2,3,4,5,6}, &rsrc};
}

// template<class VoidPtr>
// class fancy_memory_resource;

// template<class T, class VoidPtr>
// class fancy_poly_allocator;

// using memory_resource = fancy_memory_resource<void*>;
// template<class T>
// using polymorphic_allocator = fancy_poly_allocator<T, void*>;


// template<class T>
// using shmem_ptr = boost::interprocess::offset_ptr<T>;

// using shmem_resource = fancy_memory_resource<shmem_ptr<void>>;
// template<class T>
// using shmem_allocator = fancy_poly_allocator<T, shmem_ptr<void>>;



// boost::interprocess::managed_shared_memory shm(
//     boost::interprocess::open_or_create, "MySegment", 65536);

// using Alloc = shmem_allocator<int>;
// auto* vec = shm.construct<std::vector<int>>("SharedVector")(Alloc{shm.get_segment_manager()});

// vec->push_back(42); // stored in shared memory



// template<class T, class VoidPtr>
// class fancy_poly_allocator {
//     fancy_memory_resource<VoidPtr> *mr_;
// public:
//     // Only participates when VoidPtr == void*
//     template<class U = VoidPtr,
//              class = enable_if_t<is_same_v<U, void*>>>
//     fancy_poly_allocator() : mr_(get_default_resource()) {}
// };


// template<class U, bool_if_t<is_convertible_v<U*, T*>> = true>
// offset_ptr(const offset_ptr<U>& rhs) : offset_ptr(rhs.ptr()) {}

// template<class U, bool_if_t<is_static_castable_v<U*, T*> && !is_convertible_v<U*, T*>> = true>
// explicit offset_ptr(const offset_ptr<U>& rhs) : offset_ptr(static_cast<T *>(rhs.ptr())) {}

// The subtlety here is that the type-expression bool_if_t<expr> can't be evaluated until
// we know the value of expr, which in this case depends on the p.o.u. because it depends
// on U. At the p.o.u. the compiler will compute the overload set, which means evaluating
// bool_if_t<expr>; and if expr is false then SFINAE will kick in and that template will
// never be included in the overload set.



// small helper alias used for SFINAE
template<bool B>
using bool_if_t = std::enable_if_t<B, bool>;

// is_static_castable trait (expression-SFINAE)
// template<class From, class To, class = void>
// struct is_static_castable : std::false_type {};

// template<class From, class To>
// struct is_static_castable<From, To,
//     std::void_t<decltype(static_cast<To>(std::declval<From>()))>> : std::true_type {};

template<class From, class To>
inline constexpr bool is_static_castable_v = is_static_castable<From, To>::value;


// Simple offset_ptr (toy demonstration)
template<class T>
class offset_ptr {
    std::uintptr_t m_ptr;
public:
    // pointer-from-address ctor
    explicit offset_ptr(T* p) {
        m_ptr = reinterpret_cast<std::uintptr_t>(p) - reinterpret_cast<std::uintptr_t>(this);
    }

    // retrieve raw pointer
    T* ptr() const noexcept {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + m_ptr);
    }

    // 1) implicit converting ctor: allowed when U* -> T* is implicitly convertible
    template<class U, class = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    offset_ptr(const offset_ptr<U>& rhs) : offset_ptr(rhs.ptr()) {}

    // 2) explicit converting ctor: allowed when U* is static_castable to T* but NOT implicitly convertible
    template<class U,
             class = std::enable_if_t<!std::is_convertible_v<U*, T*> &&
                                      is_static_castable_v<U*, T*>>>
    explicit offset_ptr(const offset_ptr<U>& rhs) : offset_ptr(static_cast<T*>(rhs.ptr())) {}
};


// Test types
struct Base { virtual ~Base() = default; };
struct Derived : Base { };

void T2() {
    Derived d;
    offset_ptr<Derived> opd(&d);

    // implicit convert Derived* -> Base*
    offset_ptr<Base> opb = opd; // uses implicit constructor
    std::cout << "implicit ok\n";

    // reverse conversion requires explicit
    // offset_ptr<Derived> opd2 = opb; // <-- would be ill-formed (explicit ctor)
    offset_ptr<Derived> opd2(static_cast<offset_ptr<Derived>>(opb)); // explicit conversion
    std::cout << "explicit ok\n";
}


int main() {
    // T();
    // T_integral_constant();
    // Test_is_convertable_v();
    // T_decltype();

    T_is_static_castable();
    T_enable_if_t();

    Optimizing_code_paths();
    Duck_typing();

    T2();
}