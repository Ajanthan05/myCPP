#include <iostream>
#include <type_traits>

/*  It's a building block of template metaprogramming, allowing values (typically constants like integers, booleans, etc.) to be wrapped as types.
🔹 Purpose
To carry a constant value as a type.

Enables compile-time computations by making values available as types.

Commonly used in std::enable_if, type traits, SFINAE (Substitution Failure Is Not An Error), and conditional compilation.

🔹 Usage
✅ Boolean constants
using true_type = std::integral_constant<bool, true>;
using false_type = std::integral_constant<bool, false>;
These are frequently used in type traits:

std::is_integral<int>::value        // true
std::is_integral<float>::value      // false
std::is_integral<T> derives from true_type or false_type depending on T.
*/
template<typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant<T, v>;

    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};


template<typename T>
void check() {
    if constexpr (std::is_integral<T>::value) {
        std::cout << "Integral type\n";
    } else {
        std::cout << "Non-integral type\n";
    }
}

void T1() {
    check<int>();     // Integral type
    check<double>();  // Non-integral type
}

// ✅ Example: Using my_five as a Compile-Time Constant

// Define a constant type
using my_five = std::integral_constant<int, 5>;

// Function template that prints the value from an integral_constant
template<typename T>
void print_constant(T) {
    std::cout << "The constant value is: " << T::value << "\n";
}

void T3() {
    static_assert(my_five::value == 5, "Error: value is not 5");

    // Call the function with the constant type
    print_constant(my_five{}); // o/p The constant value is: 5

    /*  🧠 Explanation:
my_five is a type, not a variable.
my_five{} creates an object of type std::integral_constant<int, 5>.
T::value inside print_constant gives access to the constant at compile time.
static_assert ensures correctness at compile time — the code will not compile if the value is not 5.*/
}


 







// Function enabled only if T is an integral type
template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
print_type_info(T val) {
    std::cout << val << " is an integral type.\n";
}

// Function enabled only if T is NOT an integral type
template<typename T>
typename std::enable_if<!std::is_integral<T>::value>::type
print_type_info(T val) {
    std::cout << val << " is NOT an integral type.\n";
}

void T2() {
    print_type_info(42);       // Integral
    print_type_info(3.14);     // Not integral
}

int main() {
    T1();
}