#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <set>

#include <iterator>

#include <cstdint>
#include <tuple> // Required for structured bindings
using namespace std;

/*  LANGUAGE CHANGES
Structured bindings
Selection statements with initializerrs
Combile time conditional statements
Fold_expressions
Class template deduction
auto non-type template parameters
inline variables
constexper lambdas
Guaranteed copy elision
Nested namespace definitions
Processor predicate for header testing


    LIBRARY CHANGES 
string_view
optional
variant
any
parallel algorithm
Filesystem support
Polymorphic allocation and memory resources
ASligned new
Improve insertion and splicing for associative Containers
Math special function
Variable template for metafunctions
Boolean logic metafunction

*/
struct point_3d { double x, y, z; };

void before() {
    point_3d p = {1,2,3};
    double x = p.x;
}
/*  auto [a,b, ...] = obj
The type of obj must be Destructurable

Either all non static data members: 
    must be public
    Must be direct members of the type or members of the same public
base class of the type
    cannnot be anonymous union
*/

/*  For structured binding to work, the type of obj must be "Destructurable", which means the compiler must know how to break obj down into individual components. This is only possible if the object follows specific rules.
✅ Requirements for an object to be Destructurable:
All non-static data members must be public

Structured binding only works on public data members. If they're private or protected, the compiler can't access them for unpacking.

They must be direct members of the object, or members of the same public base class

The members a, b, etc., must be directly part of obj or inherited from a public base class of obj.

They must not be part of an anonymous union

Anonymous unions can create ambiguity about which member is active, so structured binding is disallowed in this case.
*/

struct MyStruct {
    int x;
    double y;
};


void Valid_Structured() {
    MyStruct obj{42, 3.14};
    auto [a, b] = obj; // OK: public, direct members
    
    // ❌ Error: If the members are private
}

/*  ❌ Why static members are excluded:
Static members belong to the class itself, not to any specific instance. Structured binding, however, unpacks the contents of an object instance.
*/
struct MyStruct2 {
    static int s;   // static member — belongs to the class, not to instances
    int x;
    int y;
};

int MyStruct2::s = 42;

void static_members() {
    MyStruct2 obj{1, 2};
    auto [a, b] = obj; // ✅ Only `x` and `y` are unpacked — `s` is ignored
}

// ✅ Valid with public inheritance:
struct Base {
    int x;
    double y;
};

struct Derived : public Base { };

void public_inheritance() {
    Derived obj{10, 3.14};
    auto [a, b] = obj; // ✅ OK: x and y are public and in a public base class

    // if private  ❌ Error: members are not accessible due to private inheritance
}

// ❌ Invalid with anonymous union
struct Data {
    int type;

    union {
        int intValue;
        float floatValue;
        char charValue;
    }; // anonymous union — no name

    // All fields inside the union are accessed directly
};
/*  An anonymous union is a union declared without a name inside a struct 
or class. You use it when you want multiple fields to share memory and be 
accessible directly without a union name qualifier.

⚠️ Limitations / Warnings:
Only one member is active at a time — using the wrong one causes undefined behavior.

Anonymous unions must only contain non-reference, non-static, non-virtual, trivially constructible members.

You can't use them in structured bindings (as mentioned earlier).*/
void anonymous_union() {
    // No need to access via d.unionName.intValue — you just use d.intValue.
    Data d;
    d.type = 0;        // say, 0 = int
    d.intValue = 42;   // sets the shared memory
    
    std::cout << d.intValue << std::endl;  // prints 42
}


// ✅ Example: Tag-based variant
struct Value {
    enum Type { INT, FLOAT } type;

    union {
        int i;
        float f;
    };

    void set(int val) { type = INT; i = val; }
    void set(float val) { type = FLOAT; f = val; }

    void print() {
        if (type == INT) std::cout << "int: " << i << '\n';
        else if (type == FLOAT) std::cout << "float: " << f << '\n';
    }
};

/*  ✅ Requirements Recap for Union Members:
Non-reference types (i.e., no int&, std::string&, etc.)

Non-static members

Non-virtual (so unions can't be used with polymorphism)

Trivially constructible (no user-defined constructors/destructors, like POD types)

*/

// ❌ Non-trivial constructor/destructor:
struct MyClass {
    ~MyClass() {} // non-trivial destructor
};

union MyUnion {
    int i;
    float f;
    char c;

    // No user-defined constructor/destructor allowed
    // int& ref; // ❌ invalid — reference types are not allowed

    // MyClass obj; // ❌ invalid — non-trivial type
};

/*  ✅ A trivial constructor/destructor:
Is compiler-generated (defaulted),

Does nothing special (no custom logic),

All base classes and members must also have trivial constructors/destructors.

❌ A non-trivial constructor/destructor:
Is user-defined (even if empty),
Or performs any initialization, cleanup, virtual dispatch, etc.
*/
// 🔧 Example: Trivial (The compiler treats this like a plain memory struct.)
struct Trivial {
    int x;             // trivially constructible
    // Compiler adds default constructor and destructor
};


/*  auto [a,b, ...] = obj    the type has:
    An obj.get<>method or an ADL-abl get<>overload.
    Specialations od std::tuple_size<> and std::tuple_element<>

    Destructurable types in the standard library:
        std::array
        std::tuple
        std::pair
    Uses regular auto deduction rules (auto const, auto& auto&&, etc)
*/



////////////////////////////////////////////////////////////////
/*      init-statement in selection statements
if  (T0 x = ... ; condition(x) ) {
    // x is in scope here.
} else if (T1 y = ... ; condition(x) ) {
    // x and y is in scope here.
} else  {
    // x and y is in scope here.
}
*/

// template <typename Key, typename F>
// void emplace_or_throw(std::set<Key>& m, Key&& k, F f) {
//     if (auto [it, s] = m.emplace(std::forward<Key>(k)); !s) {
//         throw;
//     else
//         f(*it);
//     }
// }

template <typename Key, typename F>
void emplace_or_throw(std::set<Key>& m, Key&& k, F f) {
    if (auto [it, s] = m.emplace(std::forward<Key>(k)); !s) {
        throw std::runtime_error("Key already exists!");
    } else {
        f(*it); // operate on the newly inserted key
    }
}

// int Templace_or_throw() {
//     std::set<std::string> names;

//     try {
//         emplace_or_throw(names, "Alice", [](const std::string& name) {
//             std::cout << "Inserted: " << name << "\n";
//         });

//         emplace_or_throw(names, "Bob", [](const std::string& name) {
//             std::cout << "Inserted: " << name << "\n";
//         });

//         // Duplicate insert — will throw
//         emplace_or_throw(names, "Alice", [](const std::string& name) {
//             std::cout << "This won't print\n";
//         });

//     } catch (const std::exception& ex) {
//         std::cerr << "Exception: " << ex.what() << '\n';
//     }
// }



//////////////////////////////////////
template <typename T0>
void print(T0&& t0) {
    cout << std::forward<T0>(t0) << "\n";
}

template <typename T0, typename... Ts>
void print(T0&& t0, Ts&&... ts) {
    print(std::forward<T0>(t0));       // Print the first argument
    print(std::forward<Ts>(ts)...);    // Recursively call with the rest
}


// In C++17
/*  if constexpr (sizeof...(ts))
sizeof...(ts) gives the number of remaining arguments.

if constexpr ensures that the recursive call to print(...) is only compiled if there are any remaining arguments.

If ts... is empty (i.e., the last argument was just printed), this block is discarded at compile time, stopping recursion.*/
template <typename T0, typename... Ts>
void print17(T0&& t0, Ts&&... ts) {
    std::cout << std::forward<T0>(t0) << "\n";

    if constexpr (sizeof...(ts)) {
        print(std::forward<Ts>(ts)...);
    }
}

/*  if constexpr in C++17?
if constexpr is a compile-time conditional that lets you write branching 
logic in templates that only compiles the selected branch — the other 
branch is discarded entirely during compilation.*/
// ✅ Example: Type-based branching
template<typename T>
void print_type_info(const T& val) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Integral: " << val << '\n';
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Floating point: " << val << '\n';
    } else {
        std::cout << "Other type\n";
    }
}

void test_Type_based_branching() {
    print_type_info(10);     // Output: Integral: 10
    print_type_info(3.14);   // Output: Floating point: 3.14
    print_type_info("abc");  // Output: Other type
}


// template <typename T, typename... Args>
// auto make_unique(Args&&... a) {
//     if constexpr (std::is_constructible_v<T, Args...>) {
//         return std::unique_ptr(new T(std::forward<Args>(a)...));
//     }
//     else
//         return std::unique_ptr(new T(std::forward<Args>(a)...));
// }

template <typename Iterator, typename Dist>
void advance(Iterator& i, Dist n) {
    using category = typename std::iterator_traits<Iterator>::iterator_category;

    if constexpr (std::is_same_v<category, std::random_access_iterator_tag>) {
        i += n;
    }
    else if constexpr (std::is_same_v<category, std::bidirectional_iterator_tag>) {
        if (n >= 0) while (n--) ++i;
        else        while (n++) --i;
    }
    else { // e.g. input_iterator_tag, forward_iterator_tag
        while (n--) ++i;
    }
}


// struct Person {
//     std::uint64_t& get_id();
//     std::string&   get_name();
//     std::uint16_t& get_age();
// private:
//     std::uint64_t& id_;
//     std::string&   name_;
//     std::uint16_t& age_;
// };

// template <std::size_t I>
// auto& get(person& p) {
//     if constexpr      (I == 0)
//         return p.get_id();
//     else if constexpr (I == 1)
//         return p.get_name();
//     else if constexpr (I == 2)
//         return p.get_age();
// }

// Define Person class
struct Person {
    Person(std::uint64_t& id, std::string& name, std::uint16_t& age)
        : id_(id), name_(name), age_(age) {}

    std::uint64_t& get_id()   { return id_; }
    std::string&   get_name() { return name_; }
    std::uint16_t& get_age()  { return age_; }

private:
    std::uint64_t& id_;
    std::string&   name_;
    std::uint16_t& age_;
};

// Custom get<I>() overload
template <std::size_t I>
auto& get(Person& p) {
    if constexpr (I == 0) return p.get_id();
    else if constexpr (I == 1) return p.get_name();
    else if constexpr (I == 2) return p.get_age();
    else static_assert(I < 3, "Index out of bounds for Person");
}

// Tuple traits to allow structured binding
namespace std {
    template <>
    struct tuple_size<Person> : std::integral_constant<std::size_t, 3> {};

    template <std::size_t I>
    struct tuple_element<I, Person> {
        using type = decltype(get<I>(std::declval<Person&>()));
    };
}

void tructured_Binding() {
    std::uint64_t id = 1001;
    std::string name = "Alice";
    std::uint16_t age = 30;

    Person p{id, name, age};

    auto& [pid, pname, page] = p;
    std::cout << pid << ", " << pname << ", " << page << "\n";
}


template <typename... Ns> 
auto Sum(Ns... ns) {
    return (ns + ... + 0);  // (ns1 + (ns2 + (ns3 + 0)))
}
/*  ns + ... + 0) is a right fold:
Equivalent to: ((ns1 + ns2) + ns3) + ... + 0
It ensures that even if the argument pack is empty, the result is 0 (important for zero arguments).
*/

/*  ✅ 1. Unary Right Fold:
Syntax: (pack op ...)
Associates from right to left
No initial value
Requires at least one element in the pack
(((arg1 + arg2) + arg3) + ...)  right-to-left*/
template <typename... Args>
auto sum1(Args... args) {
    return (args + ...);  // Right fold
}

/*  ✅ 2. Unary Left Fold:
Syntax: (... op pack)
Associates from left to right
No initial value
Requires at least one element in the pack
(... ((arg1 + arg2) + arg3)) left-to-right  */
template <typename... Args>
auto sum2(Args... args) {
    return (... + args);  // Left fold
}

/*  ✅ 3. Binary Right Fold (with initial value)
Syntax: (pack op ... op init)
Right-to-left association
Uses initial value (safe with empty packs)
(arg1 + (arg2 + (arg3 + 0)))*/
template <typename... Args>
auto sum3(Args... args) {
    return (args + ... + 0);  // Right fold with initial value
}

/*  ✅ 4. Binary Left Fold (with initial value)
Syntax: (init op ... op pack)
Left-to-right association
Uses initial value
(((0 + arg1) + arg2) + arg3)    */
template <typename... Args>
auto sum4(Args... args) {
    return (0 + ... + args);  // Left fold with initial value
}

/*  🧠 When to Use Which?
Fold Type	Use Case Example	Comment
(pack op ...)	return (args && ...);	Logical AND from right
(... op pack)	return (... + args);	Sum left to right
(pack op ... op init)	return (args + ... + 0);	Handles empty packs safely
(init op ... op pack)	return (1 * ... * args);	Multiplication with identity*/

template <typename... Bs>
bool all(Bs... bs) { return (... && bs); }   // left fold

/*  For unary folds, if the parameter pack is empty then the value of the fold is:
&& true
|| false
, void()
*/

template <typename F, typename... Args>
void for_each_arg(F f, Args&&... args) {
    (f(std::forward<Args>(args)), ...);
}

/*   What is a Unary Fold?
A unary fold has a single operator and a parameter pack on one side:

(pack op ...) – right fold

(... op pack) – left fold

If pack is non-empty, it's evaluated normally. But if pack is empty, the result depends on the operator.

🔹 Default Values for Empty Unary Folds
When the parameter pack is empty, C++ defines default identities for certain operators, to ensure the fold expression is still valid.

Here are the common ones:

Operator	Empty Fold	Result	Why this value?
&&	(... && pack)	true	Identity for &&
`		`	`(...
,	(... , pack)	void()	Comma operator returns last expr
+	(... + pack)	Compilation error (unless initial value provided)	
*	(... * pack)	Error	No defined identity in standard
& `	`	(... & pack)	Error

🔹 Why true for &&, false for ||?
Example: All of zero booleans are true?
cpp
Copy
Edit
template <typename... Bools>
bool all_true(Bools... bs) {
    return (bs && ...);  // becomes true if bs is empty
}
all_true();  // true
Because for:

cpp
Copy
Edit
(true && x) == x
So, true is the identity for &&.

Similarly:

cpp
Copy
Edit
(false || x) == x
So, false is the identity for ||.

🔹 Why void() for ,?
The comma operator returns the last expression:

cpp
Copy
Edit
(a, b, c)  // returns c
If no expressions exist, it's just void() to signify "no result".

🔹 Summary
Fold	Empty Pack Result
(bs && ...)	true
`(bs	
(bs , ...)	void()

These defaults ensure that fold expressions remain well-formed and compile-time valid, even when given no arguments.*/

//////////////  Class Template Argument Deduction (CTAD) C++17 //////////////////////////
// allows the compiler to deduce template arguments from constructor arguments without you having to explicitly write them.

template <typename T>
struct Wrapper {
    T value;
    Wrapper(T val) : value(val) {}
};

template <typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;
    Pair(T1 a, T2 b) : first(a), second(b) {}
};

// 🔹 Custom Deduction Guides
template <typename A, typename B>
Pair(A, B) -> Pair<A, B>;

void CTAD() {
    Wrapper<int> w(42); // Have to explicitly specify <int>
    // With CTAD (C++17 Onwards)
    Wrapper w17(42);  // Compiler deduces T as int

    // 🔹 Custom Deduction Guides  Sometimes deduction isn't obvious. You can help the compiler with a deduction guide
    Pair p(1, 3.14);
}

/*  🔹 When You Need a Deduction Guide
The constructor doesn’t provide enough type information.
You want to deduce differently from what the constructor types suggest.*/

// namespace std {
//     template <typename It>
//     vector(It b, It e) -> vector< typename std::iterator_traits<It>::value_type >;
// }



class MyIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = int;
    using difference_type   = std::ptrdiff_t;
    using pointer           = int*;
    using reference         = int&;

    MyIterator(int* ptr) : p(ptr) {}

    int& operator*() const { return *p; }
    MyIterator& operator++() { ++p; return *this; }
    bool operator!=(const MyIterator& other) const { return p != other.p; }

private:
    int* p;
};
struct MyContainer {
    int data[3] = {10, 20, 30};

    int* begin() { return &data[0]; }
    int* end()   { return &data[3]; }
};

// template <typename It>
void Custom_Iterator() {
    MyContainer c;

    // Using deduction guide:
    
// vector(It, It) -> vector<typename iterator_traits<It>::value_type>;

    std::vector v(c.begin(), c.end());  // Deduces std::vector<int>

    for (int i : v)
        std::cout << i << " ";
}


template <typename T>
struct name {
    constexpr name(T first_, T last_) : first(first_), last(last_) {}

    T first;
    T last;
};

// Deduction guides
name(const char*, const char*) -> name<std::string_view>;
name(const std::string&, const std::string&) -> name<std::string>;
name(std::string_view, std::string_view) -> name<std::string_view>;

void Custom_deduction() {
    name n1("aj", "Rajah");  // Deduces name<std::string_view>
    name n2(std::string("aj"), std::string("Rajah"));  // Deduces name<std::string>
    name n3(std::string_view("aj"), std::string_view("Rajah"));  // Deduces name<std::string_view>

    std::cout << "n1: " << n1.first << ", " << n1.last << '\n';
    std::cout << "n2: " << n2.first << ", " << n2.last << '\n';
    std::cout << "n3: " << n3.first << ", " << n3.last << '\n';
}

void Vector_Deduction() {
    std::vector v({0,1,2,3,4,5});

    auto it = v.begin();
    std::vector v2(it, it+4);
    /*  In C++17, the reason your code works without an explicit deduction guide for std::vector is because standard library containers like std::vector have implicit class template argument deduction (CTAD) via specializations provided in the standard itself.*/
}

// Before
template <typename T, T v>
struct constant {
    static constexpr T value = v;
};

template <auto v>
struct constant2 {
    static constexpr auto value = v;
};

void T_integral_constant() {
    using t = constant<int, 5>;
    using t = constant<char, 'a'>;
    using t = constant<bool, true>;

    // Uese regular auto deduction rules (auto const, auto&, auto&&, etc)
}


/*  Variables can now be inlinejust like functions.
They may be defined in more than one translation unit as long
as the definitions are identical
    The definition must be present in a translation unit that
access an inline variable.
    An inline variabke with external lingage (e.g. not static):
    * Must be declared inline in every translation unit.
    * Has the same address in every translation unit.
    A static constexpr member variable is implicitly inline. */


template <typename... Xs>
constexpr auto make_storage(Xs... xs) {
    auto storage = [=](auto f) { return f(xs...); };
    return storage;
}

// template <typename... Xs>
// struct tuple {
//     explicit constexpr tuple(Xs... xs) : storage{ make_storage(xs...) } {}
//     decltype(make_storage(declval<Xs>()...)) storage;
// }

// template <size_t N, typename... t>
// constexpr decltype(auto) get(tuple<T...>& t) {
//     return t.storage([] (auto&&... xs) { })
// }


#if __has_include(<string_view>)
    #include <string_view>
    #define HAVE_STRING_VIEW 1
#elif __has_include(<experimental/string_view>)
    #include <experimental/string_view>
    #define HAVE_STRING_VIEW 1
    #define HAVE_EXP_STRING_VIEW 1
#else
    #define HAVE_STRING_VIEW 0
#endif

// https://www.youtube.com/watch?v=fI2xiUqqH3Q

// for tuple
// https://www.youtube.com/watch?v=Gycxew-hztI

int main() {

    // test_Type_based_branching();
    
    // int a;
    // double b;
    // std::string c;

    // std::cin >> a >> b >> c;
    // print17(a, b, c);

    tructured_Binding();

    cout << Sum(3,4,5) << "\n";

    Custom_Iterator();

    Custom_deduction();

    Vector_Deduction();
    return 0;
}