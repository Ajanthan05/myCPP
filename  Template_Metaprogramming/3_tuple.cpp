#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <map>
using namespace std;

/*  Tuple: cna hold elements of different type (hetrogeneous container)
std::tuple<int, bool, float> t;
std::get< double > (t) = 1.5f;
    error: static assertion failed: T must occur exactly once in the tuple

contains_type< float, decltype(t)>::value == true

Now Implementing
std::vector<std::string> t{"int", "bool", "float"};
contains("double", t) == false
*/

bool contains(const std::string& search, std::vector<std::string> v, size_t start_from = 0) {
    if (search == v[start_from]) return true;
    else {
        if (start_from == v.size()-1) { // We are at last element
            return false;
        }
        else {
            return contains(search, v, start_from);
        }
    }
}
void T_contains() {
    
    std::vector<std::string> t{"int", "bool", "float"};
    std::cout << std::boolalpha << contains("bool", t) << "\n";

    std::vector<std::string> vec{"int", "bool", "float"};
    std::cout << std::boolalpha << contains("bool", vec) << '\n';
    std::cout << std::boolalpha << contains("double", vec) << '\n';

}


template<bool condition, typename THEN, typename ELSE>
struct if_;

template<typename THEN, typename ELSE>
struct if_<true, THEN, ELSE>{
    using type = THEN;
};

template<typename THEN, typename ELSE>
struct if_<false, THEN, ELSE>{
    using type = ELSE;
};

void verifyIF() {
    std::cout << std::boolalpha << std::is_same< int, if_<(10>6),int,float >::type >::value << "\n";
}

template<typename SEARCH, typename TUPLE, size_t start_from=0>
struct contains_type :
    if_< //if
        std::is_same<std::tuple_element_t<start_from, TUPLE>, SEARCH>::value,
        // THEN
        std::true_type,
        // ELSE
        typename if_< // If
            (start_from == std::tuple_size<TUPLE>::value -1),
            // THEN
            std::false_type,
            // ELSE
            contains_type<SEARCH, TUPLE, start_from +1>
        >::type
    >::type
{};

template<typename SEARCH>
struct contains_type<SEARCH, std::tuple<>, 0> : std::false_type{};

void test() {
    std::tuple<int, bool, float> tuple;
    std::cout << std::boolalpha << contains_type<bool, decltype(tuple)>::value << '\n';
    std::cout << std::boolalpha << contains_type<double, decltype(tuple)>::value << '\n';

        // Edge case for empty case
    std::tuple<> tuple2;
    std::cout << std::boolalpha << contains_type<double, decltype(tuple2)>::value << '\n';

}

/*  dependentname : compiler has to know it's dealiln with type or name
using a = int;  // a is alias for int
a * b;


template<typename T>
void foo() {
    T::x * b;
}

What is T::x?
struct A{
    using x = int;
}

struct B{
    int x = 10;
}
     = int

T::x is adependent name


template<typename T>
struct A{
    // T::type b; compiler assumed to be a value
    typename T::type b; // Declared to be a type
}

What is T::function<int()>f ?
template<typename T>
void foo() {
    T::function<int()> f;
}

 T is A here, in that case std::function alias for std function
struct A{
    using std::function;
}

struct b {
    int function = 10;
}

template<>
void foo<A>() {
    std::function<int()> f;
}

template<>
void foo<B>() {
    10 < int() > f;
}

here we can use template key word to tell T is template
template<typename T>
void foo() {
    T::template function<int()> f;
    T{}.template function<int()> f;
    (T*){}->template function<int()> f; 
}\

When ur not allowed to use keywords?
1)  When specifying the base case of a class inheritance:
template<typename T>
struct Derived : typename T::type{}; 

2) When the template-id is the one being referred to in a derived
class's using-dirctive:

struct Base {
    template<typename T>
    struct type{};
};
struct Derived : Base {
    using Base::tmplate type;
};
*/


void TupleIntro() {
    std::tuple<int, std::string, double> t1(42, "Hello", 3.14);
    auto t2 = std::make_tuple(1, 'A', 3.5f, "Ajan");
    std::cout << std::get<0>(t1);       // 42
    std::cout << std::get<1>(t1);       // Hello
    std::cout << std::get<2>(t1);       // 3.14
    // 💡 Note: std::get<i>(tuple) requires a compile-time constant index.
    /*  Because std::tuple is a heterogeneous container — different elements can have different types.
So the compiler needs to know at compile time which element you want, in order to know the type you're asking for.

int i = get_from_user();
std::get<i>(t); // ❌ ILLEGAL — index not known at compile time
It will fail to compile.*/


    // 🔁 Structured Bindings (C++17)
    auto [x, y, z] = t1;
    // x = 42, y = "Hello", z = 3.14

    cout << "\ntuple_size: " << std::tuple_size<decltype(t2)>::value << "\n";
    using Type = std::tuple_element<3, decltype(t2)>::type;  // String
    // cout << "tuple_element: " << decltype(Type) << "\n"; 

    std::tuple<int, int> a(1, 2), b(1, 3);
    bool result = a < b;  // true

    // Tuple with References
    int x1 = 5;
    auto t = std::tie(x1);  // t holds reference to x
    std::get<0>(t) = 10;   // x becomes 10
}

// ✅ Workaround for runtime indexing
template <typename Tuple>
void print_runtime_tuple(const Tuple& tup, size_t index) {
    if (index == 0)
        std::cout << std::get<0>(tup) << "\n";
    else if (index == 1)
        std::cout << std::get<1>(tup) << "\n";
    // etc.
}


/*  When to Use
Returning multiple values from a function.

Grouping heterogeneous values.

Lightweight alternative to creating structs or classes.

⚠️ Drawbacks
Access by index, not by name (less readable).

Compile-time index required.

Not ideal for long or complex data structures — use struct if semantics matter.*/

template <typename T, std::size_t Index>
void print_type(const T& tup) {
    using ElemType = typename std::tuple_element<Index, T>::type;
    std::cout << "Element type at index " << Index << " is: "
              << typeid(ElemType).name() << "\n";
}
void T() {
    std::tuple<int, std::string, float> tup;// = {5, "Ajanthan", 9.5};
    print_type<decltype(tup), 1>(tup);  // Will show: std::string
}
/*  
| Expression                     | Meaning                                            |
| ------------------------------ | -------------------------------------------------- |
| `typeid(ElemType)`             | Get runtime type info for the type `ElemType`      |
| `.name()`                      | Get mangled name of the type                       |
| `NSt7__cxx1112basic_string...` | Mangled name of `std::string` on GCC               |
| Demangling tool                | `c++filt`, or `abi::__cxa_demangle` (programmatic) |
*/

void Tie() {
    std::tuple<int, double, std::string> tup = {42, 3.14, "C++"};

    int i;
    double d;
    std::string s;

    std::tie(i, d, s) = tup;

    std::cout << i << " " << d << " " << s << "\n";
    // Output: 42 3.14 C++

    int x;
    std::string str;
    // Ignore Some Returned Values (std::ignore)
    std::tie(x, std::ignore, str) = tup;
    // Only x and str are extracted

    // Tuple of References – Changes Reflect on Original Variables
    int a = 10, b = 20;
    auto ref_tuple = std::tie(a, b);

    std::get<0>(ref_tuple) = 100;  // modifies a
    std::get<1>(ref_tuple) = 200;  // modifies b

    std::cout << a << ", " << b << "\n";  // Output: 100, 200

    // Assign Multiple Values Simultaneously
    int x1, y1;
    std::tie(x1, y1) = std::make_tuple(5, 10);

    // Use in std::map Iteration (with structured bindings)
std::map<int, std::string> m = {{1, "one"}, {2, "two"}};

for (auto& [key, value] : m) {
    std::cout << key << ": " << value << "\n";
}
// If you're on C++11/14:
for (auto& p : m) {
    int key;
    std::string val;
    std::tie(key, val) = p;
}
}
/*  Absolutely! std::tie is a powerful utility for creating a tuple of references. This allows you to:

Unpack a tuple into variables
Assign values to multiple variables from a tuple
Avoid creating temporary copies

Use in Structured Assignment (if not C++17)*/


/*  std::apply: Invoke a function with a tuple of arguments
Introduced in C++17, std::apply is used to call a function 
with a tuple of arguments by expanding the tuple.*/

/*  Variadic Templates + Tuples (Advanced)
You can unpack or manipulate tuples using variadic templates and recursion, 
especially with std::apply or std::index_sequence.*/

void print(int x, double y, std::string z) {
    std::cout << x << ", " << y << ", " << z << "\n";
}
void T_Apply() {
    auto t = std::make_tuple(1, 3.14, "C++");
    std::apply(print, t);  // C++17
}


/*  std::index_sequence: Generate compile-time indices
    Introduced in C++14, std::index_sequence is a compile-time sequence of 
integers: 0, 1, 2, ... N-1.
    It's usually used with tuple unpacking, especially in generic programming 
where you want to expand a tuple manually.*/



/*      ✅ std::index_sequence: Generate compile-time indices
    Introduced in C++14, std::index_sequence is a compile-time sequence of integers: 
0, 1, 2, ... N-1.
    It's usually used with tuple unpacking, especially in generic programming where 
you want to expand a tuple manually.*/
template<std::size_t... Indices>
void print_indices(std::index_sequence<Indices...>) {
    ((std::cout << Indices << " "), ...);
}
void Index_Sequence() {
    print_indices(std::make_index_sequence<5>{});
    // Output: 0 1 2 3 4
}

// ==============>>>>>>>>>>>        compile-time tuple expansion
/*  🔁 Use Case: Unpacking Tuples with index_sequence
This is how std::apply is implemented behind the scenes!*/
template<typename Func, typename Tuple, std::size_t... Is>
auto tuple_apply_impl(Func&& f, Tuple&& t, std::index_sequence<Is...>) {
    return f(std::get<Is>(std::forward<Tuple>(t))...);
}
/*  Is... is a parameter pack like 0, 1, 2, ...
std::get<Is>(t)... expands to std::get<0>(t), std::get<1>(t), ...
The result is that each element of the tuple is passed as a separate argument to the function.

✅ Example Expansion:
If t = std::make_tuple(42, 3.14f) and f = print_Tuple:

std::get<0>(t) → 42
std::get<1>(t) → 3.14f

So it expands to:
return f(42, 3.14f);  // same as print_Tuple(42, 3.14f)*/
template<typename Func, typename Tuple>
auto my_apply(Func&& f, Tuple&& t) {
    /*  
    Extract each element from the tuple.
    Expand it into the argument list.
    Pass it to the function.
    */
    constexpr std::size_t N = std::tuple_size<std::decay_t<Tuple>>::value;
    return tuple_apply_impl(std::forward<Func>(f),
                            std::forward<Tuple>(t),
                            std::make_index_sequence<N>{});
}
void print_Tuple(int x, float y) {
    std::cout << x << " " << y << "\n";
}
void Index_Sequence_In_Template() {
    auto tup = std::make_tuple(42, 3.14f);
    my_apply(print_Tuple, tup);  // expands to print(42, 3.14f)
}


int main() {
    TupleIntro();
    test();
    T_Apply();
    T();
    Index_Sequence();
    cout << "\n";
    Index_Sequence_In_Template();
}