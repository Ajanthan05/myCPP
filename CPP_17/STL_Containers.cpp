// https://chatgpt.com/c/68060935-175c-8000-a424-9ae5bec2d8e4

#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>
#include <list>
#include <forward_list>
#include <map>
#include <set>

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


/*  Hash:- unordered map and unordered set use hash 
Hashing Collision Resolution Techniques
1. Chaining (Open Hashing)
2. Open Addressing
    a. Linear Probing (Next available) :- When u delete u need to put #
    b. Quadratic Probing [h + i^2 % N] some time u may struct in a loop
    c. Double Hashing
        1) h1(k) = k%N
        2) h2(k) = 8 - (k%8)

Hashing (Prestoring and Fetching)
arr[10^6] Inside main(int)   arr[10^7] arr[10^7]
arr[10^7] Globaly,      arr[10^7] arr[10^8]

map storing or fetching take log(N) time
unordered_map :- Best case storing or fetching  take O(1) timeerage 
                    Worst case O(N)

Worst case happen because of internal colutions
Hashing
    1. Division Method
    2. Folding Method
    3. Mid Square Nethod

internal colutions:- When every number went to same hashing plase( or Hash index)

1
2
3 -> 13 23 43      


*/

/*  Forward_list:- single linked list
assikgn, front, empty, max_size, insert_after, emplace_after,
reverse, sort, merge, splice_after, unique, remove.
remove if, resize

unique :- only remove adjesent duplicate
remove:- will remove all occurance of given value
*/
void Forward_list() {
    std::forward_list<int> l = { 5,3,7,8,9};
    l.remove_if([] (int n) { return n > 4; });
}

/*  MULTI_MAP is an associative container that contains a sorted lost of key-value pairs
    while permitting multiple enteries with the same key.
2. count, find, contains, equal_range, lower_bound, upper_bound
3. We don't have at() and [] functions to get elements like we had in
    std::map
*/
void MULTI_MAP() {
    std::multimap<char, int, std::greater<>> m;
    m.insert(make_pair('p', 1));
    m.insert(make_pair('a', 3));
    m.insert(make_pair('a', 2));
    m.insert(pair<char, int>('d', 1));
    m.insert(make_pair('x', 1));
    m.insert({'d', 1});

    for (auto& elm : m) {
        cout << elm.first << " " << elm.second << "\n";
    }
    cout << "\n";

    // Get all the pairs od given key
    std::pair<std::multimap<char, int, std::greater<void>>::iterator, 
              std::multimap<char, int, std::greater<void>>::iterator> range = m.equal_range('a');
    for(auto it = range.first; it != range.second; ++it) {
        cout << it->first << " " << it->second << "\n";
    }

    cout << "Contains C++20: " << m.contains('a') << "\n";

    auto pair = m.find('a');
    cout << pair->first << " " << pair->second << "\n";
    
    auto ub_range = m.upper_bound('x'); // give next element
    cout << ub_range->first << " " << ub_range->second << "\n";
}


/*
            set     unordered_set   unorered_multiset
unique      T           T               F
sorted      T           F               F

Then why unordered multiset
maintain a collection of non-unique items with fast insertion and removel
It uses hashing with bucket
This allows fast access to inividual elements, boc adfter computing the hash of 
the value it refers to the exact bucket the elements is placed into
    find, count

            MULTISET        
It is an associative container that contains a sorted set of duplicate objects of Key
Usually Read Black Tree
Insertion, Removal, Search have logirathmic complexity
*/
class Person {
public:
    float age;
    string name;

    bool operator < (const Person& rhs) const { return age < rhs.age; }
    bool operator > (const Person& rhs) const { return age > rhs.age; }
};
void T_multiset() {
    std::multiset<Person, std::greater<>> mS = { {25, "Ajanthan"}, {30, "Tharma"}, {30, "Kala"}};

    for (const auto& e : mS) {
        cout << e.age << " " << e.name << "\n";
    }
}


struct Person2 {
    float age;
    string name;
};

struct PersonComparator2 {
    bool operator()(const Person2& lhs, const Person2& rhs) const {
        if (lhs.age != rhs.age)
            return lhs.age > rhs.age; // for descending age
        return lhs.name < rhs.name;   // ascending name when age is equal
    }
};

void T_multiset2() {
    multiset<Person2, PersonComparator2> mS = {
        {25, "Ajanthan"},
        {30, "Tharma"},
        {30, "Kala"}
    };

    for (auto& p : mS) {
        cout << p.age << " " << p.name << "\n";
    }
}

/*  LIST:
ASSIGN, FRONT, BACK, empty, size, max_size, clear, insert, emplace, push_back
pop_back, push_front, pop_front, reverse, merge, split, unique, remove remove_if, resize
*/
int main() {

    // test();
    // test_tie();

    // List();

    MULTI_MAP();
    T_multiset();
    T_multiset2();
    return 0;
}