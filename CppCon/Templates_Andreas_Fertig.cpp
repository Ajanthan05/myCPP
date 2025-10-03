#include <iostream>
#include <memory>
#include <array>
#include <vector>
#include <cstddef>
#include <string>
#include <list>
#include <map>
#include <unordered_set>

#include "Vector.h"
#include <utility>  // std::move
#include <type_traits>
// #include <QString>

using namespace std;

template<typename T, size_t SIZE>
// In C++20 We can use require
requires(not std::is_pointer_v<T>)
struct Array {
    // Added a check that T is not a pointer
    // static_assert(not std::is_pointer<T>::value);

    // Constructor that value-initializes mData
    constexpr Array() : mData{} {}

    Array(std::initializer_list<T> init) {
        if (init.size() > SIZE) 
            throw std::out_of_range("List size is grater than size\n");
        int i = 0;
        for(const auto& val : init) {
            mData[i++] = val; 
        }

        for(; i < SIZE; ++i)
            mData[i] = T{};  
    }

    T* data();
    const T* data() const {
        return std::addressof(mData[0]);
    }

    constexpr size_t size() const { return SIZE; }
    T* begin() { return data(); }
    T* end() { return data() + size(); }

    // Indexed access (operator[]).
    T& operator[](size_t idx) { return mData[idx]; }
    const T& operator[](size_t idx) const { return mData[idx]; }

private:
    T mData[SIZE];
};

template<typename T, size_t SIZE> requires(not std::is_pointer_v<T>)
T* Array<T, SIZE>::data() {
    return std::addressof(mData[0]);
}

/*  const-correctness?
Const version:
const T* data() const;
Returns a read-only pointer (const T*)
Used when the Array object is const*/
void T() {
    Array<int, 4> arr;
    arr.data()[0] = 42;  // OK: non-const

    const Array<int, 4> c_arr;
    // c_arr.data()[0] = 42;  // ❌ Error: returns const int*
    /*  Without both versions:
You couldn't write to the data in non-const objects.
Or, you couldn't call data() at all on const objects.*/
}

/*  Problem: Overloaded operator&       std::addressof vs operator&
In C++, you normally get the address of a variable using the address-of operator: &x.
But this isn't always safe, especially when a class overloads operator&().  */
struct Weird {
    int value;
    Weird* operator&() {
        std::cout << "Custom operator& called!\n";
        return this;  // Totally custom behavior
    }
};
void T2() {
    Weird w;
    Weird* ptr = &w;  // Calls custom operator&()!
    // Here, &w doesn’t do the usual "give me the memory address" — it calls the overloaded operator&.
}
/*  std::addressof(x) bypasses the overloaded operator& and gives you the true memory address of x.
It works even if operator& is customized in weird or dangerous ways.
This trick forces the compiler to ignore operator& by:

Casting the object to a built-in type (char&)

Taking the real address

Reinterpreting it back to T*
*/
template<typename T>
T* Addressof(T& arg) noexcept {
    return reinterpret_cast<T*>(
        &const_cast<char&>(
            reinterpret_cast<const volatile char&>(arg)
        )
    );
}

/*  Universal reference + std::forward (if needed)
Accepts everything (lvalue, rvalue, const, non-const).
Doesn't copy.
You can specialize or overload further if needed.*/
template<typename T>
void Print(T&& arr) {
    for (const auto& val : arr) {
        std::cout << val << " ";
    }
    std::cout << "T&&\n";
}

/*  const T& (most common)
If your function just reads from the container:
This is often better:
It's clearer: "I just want to read"
Slightly more optimized by the compiler
Prevents accidental moves or modifications*/
template<typename T>
void Print(const T& arr) {
    for (const auto& val : arr) {
        std::cout << val << " ";
    }
    std::cout << "Const\n";
}

void T3() {
    Array<int, 4> arr = {1, 2};
    Print(arr);
/*
| Signature             | Accepts           | Purpose                                 |
| --------------------- | ----------------- | --------------------------------------- |
| `const T&`            | Lvalues & Rvalues | ✅ Best for read-only usage (like Print) |
| `T&&` (universal ref) | Everything        | ✅ Flexible, use if forwarding/moving    |
| `const T&&`           | Rvalue const only | ❌ Not useful for printing               |
*/
}

//////////////////////////  Class templates: Inheritance //////////////////////////
template<typename T>
class Foo {
public:
    void Func() { cout << "Main\n"; }
};

template<typename T>
class Bar : public Foo<T> {
public:
    void BarFunc() {
        // Func(); there are no arguments to ‘Func’ that depend on a template parameter
        this->Func();
        Foo<T>::Func();
    }
};

void Inheritance() {
    Bar<int> b{};
    b.BarFunc();
}

/*  alias template is like a macro, but for types.
It creates a shortcut or nickname for a templated type*/
template <size_t N>
using CharArray = std::array<char, N>;

template<size_t N>
void fill_zeros(CharArray<N>& arr) {
    arr.fill(0);
}
// Makes Code More Generic-Friendly than
template<size_t N>
void fill_zeros_Not_Friendly(std::array<char, N>& arr){};

template<size_t N>
using CharArray =
#ifdef PRODUCT_A
    Array<char, N>;
#else
    std::array<char, N>;
#endif
/*  In Code (for testing or debugging)
#define PRODUCT_A

g++ -DPRODUCT_A main.cpp -o main
🔹 Makefile:
CXXFLAGS += -DPRODUCT_A
🔹 CMake:
add_compile_definitions(PRODUCT_A)*/


// Where You Can Use T (&arr)[N]
template<std::size_t N>
void print_array(int (&arr)[N]) {
    for (std::size_t i = 0; i < N; ++i)
        std::cout << arr[i] << " ";
}







// Base class
template <typename T>
class Base {
public:
    void show() const {
        std::cout << "Base::show()" << std::endl;
    }
};

// Template derived class
template <typename T>
class Derived : public Base<T> {
public:
    void showDerived(const T& value) {
        // Call base function
        this->show();
        Base<T>::show();
        std::cout << "Derived::showDerived() with value = " << value << std::endl;
    }
};

void T4() {
    Derived<int> d1;
    d1.showDerived(42);

    Derived<std::string> d2;
    d2.showDerived("Hello");
}

// class Array {


// private:
//     T mData[];
// }

/*  ALIAS template
typedef cannot take template parameters


SFINAE / Traits Simplification
#include <type_traits>

template <typename T>
using RemoveConstRef = typename std::remove_const<typename std::remove_reference<T>::type>::type;

// With alias template (shorter!)
template <typename T>
using RemoveConstRef2 = std::remove_const_t<std::remove_reference_t<T>>;

*/
template <size_t N>
using CharArray = 
#ifdef PRODUCT_A
    Array<char, N>
#else 
    std::array<char, N>;
#endif

void T5() {
    CharArray<5> ar;
}

/*  SPAN
Safety Features
    No bounds checking by default (use with care).
    std::span is safer than raw pointers since it carries size.
    Only works with contiguous data.

Doesn't perform bounds checks (unless you write your own).

Since it’s non-owning, be careful about dangling references (don't use span after the underlying data is destroyed).

std::span<int, 5>	Fixed-size span (extent known at compile time)
std::span<int>	Dynamic-size span (size set at runtime)
*/

/*  template<class T, std::size_t Extent = std::dynamic_extent>
class span;

T: The element type — must be known at compile time.
Extent: The number of elements:
If Extent == std::dynamic_extent (i.e., -1), then size is stored at runtime.
If Extent is any other value, then the span's size is fixed at compile time.

constexpr std::size_t dynamic_extent = -1;
This special value tells the std::span to treat the size as not known at compile time, so it will store the size internally.

*/


/*  tag dispatch with non-type template parameters:

When dimensions are known at compile time, the compiler can optimize (loop unrolling, 
stack allocation, smaller object size, etc.).

When dimensions are dynamic, we fall back to storing sizes at runtime (heap allocation via std::vector).

So the user interface is unified (Matrix<T, Rows, Cols>), but the implementation is chosen at compile-time depending on the template arguments.

template with a default non-type parameter (like std::size_t Extent = std::dynamic_extent) — is a powerful idiom in modern C++. 
It's often used when a class can behave differently at compile-time vs. run-time, depending on the availability of information.
*/
#include <cassert>

constexpr std::size_t dynamic_extent = static_cast<std::size_t>(-1);

template <typename T, std::size_t Extent = dynamic_extent>
class span;

// -----------------------------
// Fixed extent version
// -----------------------------
template <typename T, std::size_t Extent>
class span {
    T* ptr_;

public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = std::size_t;
    static constexpr size_type extent = Extent;

    constexpr span(T* ptr) : ptr_(ptr) { }

    constexpr T* data() const noexcept { return ptr_; }
    constexpr size_type size() const noexcept { return Extent; }
    constexpr bool empty() const noexcept { return Extent == 0; }

    constexpr T& operator[](size_type idx) const {
        assert(idx < Extent);
        return ptr_[idx];
    }

    constexpr span<T, Extent> subspan(size_type offset) const {
        assert(offset <= Extent);
        return span<T, Extent - offset>(ptr_ + offset);
    }
};

// -----------------------------
// Dynamic extent version
// -----------------------------
template <typename T>
class span<T, dynamic_extent> {
    T* ptr_;
    std::size_t size_;

public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = std::size_t;
    static constexpr size_type extent = dynamic_extent;

    constexpr span() : ptr_(nullptr), size_(0) {}
    constexpr span(T* ptr, std::size_t size) : ptr_(ptr), size_(size) {}

    template <std::size_t N>
    constexpr span(T (&arr)[N]) : ptr_(arr), size_(N) {}

    constexpr T* data() const noexcept { return ptr_; }
    constexpr size_type size() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size_ == 0; }

    constexpr T& operator[](size_type idx) const {
        assert(idx < size_);
        return ptr_[idx];
    }

    constexpr span<T> subspan(size_type offset, size_type count = dynamic_extent) const {
        assert(offset <= size_);
        size_type remaining = size_ - offset;
        return span<T>(ptr_ + offset, count == dynamic_extent ? remaining : count);
    }
};

void mySpan() {
    int arr[] = {1, 2, 3, 4, 5};

    // Fixed extent
    span<int, 5> s_fixed(arr);
    std::cout << "Fixed: ";
    for (size_t i = 0; i < s_fixed.size(); ++i)
        std::cout << s_fixed[i] << " ";
    std::cout << "\n";

    // Dynamic extent
    span<int> s_dyn(arr, 5);
    std::cout << "Dynamic: ";
    for (size_t i = 0; i < s_dyn.size(); ++i)
        std::cout << s_dyn[i] << " ";
    std::cout << "\n";

    // Subspan
    auto s_sub = s_dyn.subspan(2, 2);
    std::cout << "Subspan: ";
    for (size_t i = 0; i < s_sub.size(); ++i)
        std::cout << s_sub[i] << " ";
    std::cout << "\n";
}
/*  extent Purpose: Allows you (or meta-programming code) to check at compile time:

if constexpr (span<int, 5>::extent != std::dynamic_extent) {
    // known size at compile time
}*/
template <typename Span>
void print_info(const Span& s) {
    using T = typename Span::value_type;
    std::cout << "Span of type: " << typeid(T).name() << "\n";
    std::cout << "Size: " << s.size() << "\n";
    
    if constexpr (Span::extent != dynamic_extent)
        std::cout << "Extent known at compile time\n";
    else
        std::cout << "Extent is dynamic\n";
}



/*
struct Vector3 {
    float x=0.0f, y=0.0f, z=0.0f;

    Vector3() {}
    Vector3(float s) : x(s), y(s), z(s) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {
        cout << "Copy\n";
    }

    Vector3(Vector3&& other) : x(other.x), y(other.y), z(other.z) {
        cout << "Move\n";
    }

    ~Vector3() {
        cout << "Destroy\n";
    }

    Vector3& operator=(const Vector3& other) {
        cout << "Copy\n";
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3& operator=(Vector3&& other) {
        cout << "Move\n";
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};
*/
struct Vector3 {
    float x=0.0f, y=0.0f, z=0.0f;
    int* ptr;

    Vector3() {
        ptr = new int[5];
        cout << "Default Constructor\n";
    }

    Vector3(float s) : x(s), y(s), z(s) {
        ptr = new int[5];
        cout << "Scalar Constructor\n";
    }

    Vector3(float x, float y, float z) : x(x), y(y), z(z) {
        ptr = new int[5];
        cout << "XYZ Constructor\n";
    }

    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {
        cout << "Copy Constructor\n";
    }

    Vector3(Vector3&& other) noexcept : x(other.x), y(other.y), z(other.z) {
        ptr = other.ptr;
        other.ptr = nullptr;
        cout << "Move Constructor\n";
    }

    ~Vector3() {
        cout << "Destructor\n";
        delete[] ptr;
    }

    // Vector3& operator=(const Vector3& other) {
    //     cout << "Copy Assignment\n";

    //     ptr = other.ptr;
    //     other.ptr = nullptr;
    //     if (this != &other) { // self-assignment check
    //         x = other.x;
    //         y = other.y;
    //         z = other.z;
    //     }
    //     return *this;
    // }

    // Vector3& operator=(Vector3&& other) noexcept {
    //     cout << "Move Assignment\n";

    //     ptr = other.ptr;
    //     other.ptr = nullptr;
    //     if (this != &other) {
    //         x = other.x;
    //         y = other.y;
    //         z = other.z;
    //     }
    //     return *this;
    // }

    Vector3& operator=(const Vector3& other) {
        cout << "Copy Assignment\n";
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
            // Deep copy if ptr is owning memory
            if (other.ptr) {
                ptr = new int(*other.ptr); 
            } else {
                ptr = nullptr;
            }
        }
        return *this;
    }

    Vector3& operator=(Vector3&& other) noexcept {
        cout << "Move Assignment\n";
        if (this != &other) {
            delete ptr; // free old resource
            x = other.x;
            y = other.y;
            z = other.z;
            ptr = other.ptr;
            other.ptr = nullptr; // ✅ only legal here
        }
        return *this;
    }
};


template <typename T>
void Print( Vector<T> &v) {
    for (size_t i=0; i<v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << "\n";
}

template <>
void Print( Vector<Vector3> &v) {
    for (size_t i=0; i<v.size(); ++i) {
        cout << v[i].x << " " << v[i].y << " " << v[i].z << "\n";
    }
    cout << "\n";
}



void T_Vector() {
    Vector<std::string> v;
    v.push_back("1");
    v.push_back("2");
    Print(v);

    // Vector<Vector3> v3;
    // v3.push_back(Vector3(2.9f));
    // v3.push_back(Vector3(1,2,3));
    // cout << "Default\n";
    // v3.push_back(Vector3());

    Vector<Vector3> v3;
    v3.emplace_back(Vector3(2.9f));
    v3.emplace_back(Vector3(1,2,3));
    cout << "Default\n";
    v3.emplace_back(Vector3());
    Print(v3);

    std::vector<Vector3> v4;
    v4.emplace_back(Vector3(2.9f));
    v4.emplace_back(Vector3(1,2,3));
    cout << "Default\n";
    v4.emplace_back(Vector3());

    cout << "Range base for loop\n";
    for (std::string value : v) {
        cout << value << " ";
    }

    cout << "\nIterator\n";
    for(Vector<std::string>::iterator it = v.begin();
        it != v.end(); it++) {
            cout << *it << " ";
        }
        cout << "\n";
}

template <typename T>
auto length(T const& val) {
    if constexpr (is_integral<T>::value) {
        return val;
    }
    else {
        return val.length();
    }
}
void IfConstexper() {
    int n{10};
    string s;
    cout << "n = " << n << " and length = " << length(n) << '\n';
    cout << "s = " << n << " and length = " << length(n) << '\n';
}


// Partial Specialization   You specialize the template for a subset of its parameters.
// Primary template
template<typename T1, typename T2>
struct Pair { };
// Partial specialization when both types are the same
template<typename T>
struct Pair<T, T> {
    void info() { std::cout << "Same types\n"; }
};

// Value Specialization (Non-type template parameters)
// You can specialize based on constant values (like int, bool, etc.).
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};
template<>
struct Factorial<0> {
    static constexpr int value = 1;
};
static_assert(Factorial<5>::value == 120);


/* Function Templates ≠ Specializable
Function templates cannot be partially specialized. You use overloading or 
std::enable_if/concepts instead.    */
template<typename T>
void print(T t) {
    std::cout << "Generic\n";
}
void print(int x) { // Overload instead of specialization
    std::cout << "int\n";
}


template<typename T, typename... Ts>
constexpr auto min(const T& a, const T& b, const Ts&... ts) {
    const auto m = a<b ? a : b;
    if constexpr(sizeof...(ts) > 0) {
        return min(m, ts...);
    }
    return m;
}
/*  Why if constexpr?

if constexpr(sizeof...(ts) > 0) ensures the recursion stops safely.

Without if constexpr, the compiler would try to compile min(m, ts...) even when ts... is empty → error.

if constexpr is a C++17 feature: only the branch that’s valid is compiled.*/

auto Normalize(const std::string& t) { return t; }
// auto Normalize(const QString& t) { return t.toStdString(); }
auto Normalize(const char* t) { return std::string{t}; }

template<class T> 
auto Normalize(const T& t) { return std::to_string(t); }
/*  This is the generic fallback.
    If it’s not one of the special cases above (not std::string, 
QString, const char*), then assume it’s some numeric type (int, float, 
double, etc.) and convert with std::to_string.
    Now we can normalize 42 → "42", 3.14 → "3.140000", etc.*/

template <typename T, typename... Ts>
auto _StrCat(std::string& ret, const T& targ, const Ts&... args) {
    ret += Normalize(targ);
    if constexpr(sizeof...(args) > 0) {
        _StrCat(ret, args...);
    }
}
// With C++17, if constexpr allows stopping recursion at compile-time → no need for special base case.

template <typename T, typename... Ts>
auto StrCat(const T& targ, const Ts&... args) {
    std::string ret{Normalize(targ)}; // Start with firse args
    _StrCat(ret, args...);
    return ret;
}

// Modern C++20/23 alternative With fold expressions, we can avoid recursion:
template<typename... Ts>
std::string StrCat_20(const Ts&... args) {
    std::string ret;
    (ret += ... += Normalize(args));  // fold expression
    return ret;
}
// ret += Normalize(arg1) += Normalize(arg2) += ... += Normalize(argN);

template <typename T, typename... Ts>
auto BuildCSVLine(const T& targ, const Ts&... args) {
    auto ret{Normalize(targ)};
    auto addColonAndNormalize = [&](const auto & arg) {
        ret += ',';
        ret += Normalize(arg);
    };
    (addColonAndNormalize(args),  ...);

    return ret;
}
void STR() {
    // QString qs(L"Ajanthan@");
    auto s = BuildCSVLine("Hello", std::string{"C++"}, 20);
    cout << s << "\n";

    auto s2 = StrCat_20("Ajanthan", std::string{"C++"}, 20);
    cout << s2 << "\n";
}

/*
namespace details::array_single_compare {
    template<typename T, size_t N, typename U, size_t... I>
    constexpr bool Compare(const T (&a)[N], const U &b, std::index_sequence<I...>) {
        return ((a[I] == b) && ...);
    }
}

template<typename T, size_t N, typename U>
constexpr bool Compare(const T (&a)[N], const U &b) {
    return details::array_single_compare::Compare(a, b, std::make_index_sequence<N>{});
}
namespace datails::array_compare {
    template<typename T, size_t N, size_t... I>
    constexpr bool Compare(const T (&a)[N], const T (&b)[N], std::index_sequence<I...>) {
        return ((a[I] == b[I]) && ...);
    }
}

// N is common compare only 2 equal size array
template<typename T, size_t N>
constexpr bool Compare(const T (&a)[N], const T (&b)[N]) {
    return details::array_compare::Compare(a, b, std::make_index_sequence<N>{});
}
*/

template<typename T, size_t N>
constexpr bool Compare(const T (&a)[N], const T (&b)[N]) {
    return std::ranges::equal(a, b);
}
void T_range() {
    int arr1[] = {1,2,3,4,5};
    int arr2[] = {1,2,5,4,5};
    int arr3[] = {1,2,3,4,5};

    cout << "Compare20: " << Compare(arr1, arr2) << "\n";
}

// ------------------------------  Template That Takes Another Template    ----------------------------------------
template < 
    template<class, class> class Container, 
    class T, 
    class Allocator = std::allocator<T>
>
void Fun(const Container<T, Allocator> &c) {
    for (const auto& e : c) {
        cout << e << " ";
    }
    cout << "\n";
}
void Test_TemplateTemplate() {
    vector<int> v{2, 3, 4};
    Fun(v);

    std::list<char> l{'a', 'B', 'c'};
    Fun(l);
}

// Method 1
template <typename A, typename B>
std::ostream& operator<<(std::ostream& os, const std::pair<A,B>& p) {
    return os << "(" << p.first << ", " << p.second << ")";
}

// Method 2
struct Printer {
    template <typename T>
    static void print(const T& x) { std::cout << x; }

    template <typename A, typename B>
    static void print(const std::pair<A,B>& p) {
        std::cout << "(" << p.first << "," << p.second << ")";
    }
};
template <
    template<class...> class Container2,  // accepts any number of type template params
    class... Args
>
void Fun2(const Container2<Args...>& c) {
    for (const auto& e : c) {
        cout << e << " ";
// ethod 2
// This way you don’t globally overload operator<< for pairs.
        // if constexpr (requires { e.first; e.second; }) {
        //     cout << "(" << e.first << "," << e.second << ") ";
        // } else {
        //     cout << e << " ";
        // }

// Method 3
        // Printer::print(e);
        // std::cout << " ";
    }
    cout << "\n";
}

void Test_TemplateTemplate2() {
    vector<int> v{2, 3, 4};
    Fun2(v);

    list<char> l{'a', 'B', 'c'};
    Fun2(l);

    unordered_set<string> us{"hi", "bye", "wow"};
    Fun2(us);

    // For map, each element is a pair
    map<int, string> m{{1, "one"}, {2, "two"}};
    Fun2(m);
}


// Partial specializations cannot be done for function templates (use overload or SFINAE instead).
// Deduction type
int main() {
    // T2();
    // T3();

    // Inheritance();

    // T4();
    mySpan();
    // T_Vector();

    // T_Span();
    STR();
    T_range();

    Test_TemplateTemplate();

    Test_TemplateTemplate2();
}

// Reference : https://chatgpt.com/c/6814ca57-2ff8-8000-ba23-a80b7efbf11d