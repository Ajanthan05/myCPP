#include <iostream>
#include <memory>
#include <array>

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

template<typename T, size_t SIZE>
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


// Deduction type
int main() {
    T2();
    T3();

    Inheritance();
}