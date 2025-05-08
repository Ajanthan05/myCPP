#include <iostream>
#include <vector>
#include <string>
#include <tuple>

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


int main() {
    test();
}