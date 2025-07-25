// https://chatgpt.com/c/68060935-175c-8000-a424-9ae5bec2d8e4

#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>
#include <list>

using namespace std;

/*  std::tuple is a fixed-size collection of heterogeneous values (values of different types), available 
in C++11 and later. It's similar to a lightweight struct, but more generic and flexible.
*/
void test() {
    std::tuple<int, std::string, double> person(42, "Alice", 5.5);

// Creatint Tuple
    auto t = std::make_tuple('A', 1, 3.14); // Type deducted tuple<char, int, double>

/*  ✅ Accessing Elements
Use std::get<N>(tuple) where N is a compile-time constant index (0-based)*/
    int x = std::get<1>(t);     // x = 1

// ✅ Modifying Elements
    std::get<1>(person) = "Ajan";

// ✅ Tuple Size and Type   
    // Get the number of elements
    cout << std::tuple_size<decltype(person)>::value << "\n";

    // Get the type of elements
    // cout << std::tuple_element<0, decltype(t)>::type << "\n";    // ❌ Error!
    // Use tuple_element to extract the type of the first element (int)
    std::tuple_element<0, decltype(t)>::type x1 = std::get<0>(t);

    std::cout << x1 << "\n";

    // Alternate modern way (C++14+):   You can use auto instead
    auto x3 = std::get<0>(t);  // Deduces char

    /*  But using std::tuple_element<Index, TupleType>::type is helpful 
    when you need template metaprogramming or precise type declarations at compile-time.*/

// ✅ Structured Bindings (C++17+)  Easier way to unpack tuple values:
    auto [id, name] = std::make_tuple(7, "Ajan");

// ✅ Comparing Tuples  Tuples support ==, <, >, etc. — comparison is lexicographic:
    std::tuple<int, int> a(1, 2), b(1, 3);
    cout << "Compare: " << (a < b) << "\n";

// ✅ Tuple Concatenation
    auto t1 = std::make_tuple(1, 2);
    auto t2 = std::make_tuple("A", 3.14);
    auto t3 = std::tuple_cat(t1, t2); // Result: tuple<int, int, const char*, double>

// ✅ Tying Variables (std::tie)     Useful for unpacking or assigning:
    int c;
    std::string d;
    std::tie(c, d) = std::make_tuple(10, "hi");

}

// Returning multiple values:
std::tuple<int, std::string> getData() {
    return std::make_tuple(42, "Answer");
}
void test_tie() {
// ✅ Ignoring Values
    std::string s;
    std::tie(std::ignore, s) = getData(); // Only get second value
    cout << "tie: " << s << "\n";
}


////////////    ✅ Example: Template function using std::tuple_element///////////
template <std::size_t N, typename Tuple>
void print_nth_element(const Tuple& t) {
    using ElementType = typename std::tuple_element<N, Tuple>::type;
    ElementType value = std::get<N>(t);
    std::cout << "Element " << N << ": " << value << "\n";
}

void Template_function() {
    std::tuple<int, std::string, double> t(42, "Alice", 3.14);

    print_nth_element<0>(t);  // Output: Element 0: 42
    print_nth_element<1>(t);  // Output: Element 1: Alice
    print_nth_element<2>(t);  // Output: Element 2: 3.14
}


/////////////////////////////////////////////////////////////////////////

struct MyType {
    int x;
    std::string y;
};

// (1) Tuple size
namespace std {
    template<>
    struct tuple_size<MyType> : std::integral_constant<std::size_t, 2> {};

    template<>
    struct tuple_element<0, MyType> {
        using type = int;
    };

    template<>
    struct tuple_element<1, MyType> {
        using type = std::string;
    };
}

// (2) get<I>(obj) overloads
template <std::size_t I>
decltype(auto) get(const MyType& obj);

template <>
decltype(auto) get<0>(const MyType& obj) {
    return obj.x;
}

template <>
decltype(auto) get<1>(const MyType& obj) {
    return obj.y;
}

void test2() {
    MyType obj{42, "hello"};

    auto [a, b] = obj;  // structured binding works now

    std::cout << a << ", " << b << "\n";
}




/*  td::unordered_set::insert()
When you insert into a std::unordered_set, the insert() function returns a std::pair

.first → an iterator to the inserted element or the element that already existed.
.second → a bool:
    true if the element was inserted (i.e., it did not exist before).
    false if the element already existed (i.e., duplicate).*/


void printL(list<int> &l) {
    for(auto i : l) {
        cout << i << " ";
    }
    cout << "\n";
}

void List() {
    // Statement list<int> l3(5, 9) creates a list of 5 elements, each initialized to 9.
    list<int> l(3, 5);  
    printL(l);

    l.push_front(1);
    l.push_back(9);

    auto it = l.begin();

    advance(it, 2);
    cout << *it << "\n";

    // Accessing first and last elements
    cout << l.front() << endl;
    cout << l.back() << endl;
    
    // Access third element
    cout << "Access third element: " << *next(l.begin(), 2) << "\n";

    

    // Finding 4
    auto it2 = find(l.begin(), l.end(), 4);
    
    if (it2 != l.end()) cout << *it << "\n";
    else {
        cout << "Element Not Found! Add 3: ";

        *next(l.begin(), 1) = 3;
        printL(l);
    }
}

int main() {

    test();
    test_tie();

    List();
    return 0;
}