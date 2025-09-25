#include <iostream>
using namespace std;

template <typename T>
void printColl(const T& coll) {
    for(const auto& e : coll) {
        cout << e << " ";
    }
    cout << "\n";
}

// Abbreviated function templates
void printColl20(const auto& coll) {
    for(const auto& e : coll) {
        cout << e << " ";
    }
    cout << "\n";
}

/*  Template require that all operations are supported
Implicit requirements for T:
    • operator < (returning bool)
    • copy/move constructor
*/
template<typename T>
T mymax(T a, T b) {
    return b < a ? a : b;
}
void Requirement() {
    // std::cout << mymax(7, 33.4); // ERROR: can't deduce T (int or double)
    std::cout << mymax<double>(7, 33.4); // OK, T is double

    std::complex<double> c1, c2;
    // std::cout << mymax(c1, c2); // ERROR: deduces T as complex<>, but no < supported

    std::atomic<int> a1{8}, a2{15};
    // std::cout << mymax(a1, a2); // ERROR: deduces T as atomic<>, but copying disabled
}

//--------------    Concepts    -------------------------
// Compiler get better error messages
template <typename T>
concept SupportsLessThan = requires (T x) { x < y; }; // Named requirement

template <typename T>
requires std::copyable<T> && SupportsLessThan<T>
T mymax20(T a, T b) {
    return b < a ? a : b;
}

/*  Let the compiler deduce the return type "auto" C++14

Class Template argument Deduction (CTAD)
Constructor Deduce template parameter since C++17*/
namespace std {
    template <typename ElemT, typename Allocator = allocator<T>>
    class vector {
    public:
        vector() noexcept(noexcept(Allocator()));
        explicit vector(const Allocator&) noexcept;
        explicit vector(size_t n, const Allocator& = Allocator());
        vector(size_t n, const ElemT& value, const Allocator& = Allocator());
        template <typename Iter>
        vector(Iter beg, Iter end, const Allocator& = Allocator());
        vector(const vector& x);
        vector(vector&&) noexcept;
        vector(const vector&, const Allocator&);
        vector(vector&&, const Allocator&);
        vector(initializer_list<ElemT>, const Allocator& = Allocator());
        // ...
    };
}