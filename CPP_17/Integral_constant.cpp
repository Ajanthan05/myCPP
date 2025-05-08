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

// Defines a templated struct that represents a compile-time constant value.
template<typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    /*  Defines a static constant named value, which holds the compile-time constant v.
It can be accessed like T::value.*/

    using value_type = T;   // Introduces an alias value_type for the type T.
    using type = integral_constant<T, v>;
    /*  Defines a type alias that refers to the type itself.
Useful in meta-algorithms that expect a ::type.*/

    // Implicit conversion to value_type
    constexpr operator value_type() const noexcept { return value; }
    /*  Allows an object of this type to be used where a T is expected.
Example: integral_constant<int, 5>{} + 1 is valid and evaluates to 6.*/

    // Function call operator
    constexpr value_type operator()() const noexcept { return value; }
    /* Allows the object to be called like a function: obj() returns value. */
};

void Test() {
    integral_constant<int, 42> my_const;

    // 🟢 Implicit conversion operator: used like a plain int
    int x = my_const + 1; // converts to int automatically
    std::cout << "x = " << x << '\n'; // Output: x = 43

    // 🟢 Function call operator: call like a function
    std::cout << "my_const() = " << my_const() << '\n'; // Output: my_const() = 42
/*  int x = my_const + 1;
→ my_const implicitly converts to int (via operator value_type()), so it becomes 42 + 1.

my_const()
→ Calls operator()(), which simply returns value, also 42.*/
}
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


template <auto V> 
using IntegralConstant = std::integral_constant<decltype(V), V>;

void T4() {
    constexpr auto five = IntegralConstant<5>{};

    static_assert(five == 5, "Error: value is not 5");
    std::cout << "T4 value is: " << five << "\n";
    /*  integral_constant_t<5> is shorthand for std::integral_constant<int, 5>.
five is a constexpr object holding the value 5.
It behaves like an int but is also a type with five::value, which is useful in templates.*/
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
/*std::is_integral<T> returns a type (std::true_type or std::false_type), 
which is derived from std::integral_constant<bool, true/false>.

std::enable_if<condition>::type is only defined if the condition is true. 
Otherwise, substitution fails silently (SFINAE), and that overload is discarded.*/
}


void T5() {
    std::cout << std::boolalpha;

    std::cout << "\nIs int integral? " << std::is_integral<int>::value << "\n";
    std::cout << "Is float integral? " << std::is_integral<float>::value << "\n";

    using trait = std::is_integral<int>;
    std::cout << "value_type is bool? " << std::is_same<trait::value_type, bool>::value << "\n";
/*  using type = std::integral_constant<bool, value>;
This is a type alias to the actual std::integral_constant<bool, value>.
It allows for nested use in metaprogramming contexts.

✅ Example:
typename std::is_integral<int>::type   // same as std::true_type*/
}


template<typename T>
void print_type_info_constexpr(T val) {
    if constexpr (std::is_integral<T>::value) {
        std::cout << val << " is an integral type.\n";
    } else {
        std::cout << val << " is NOT an integral type.\n";
    }
}

int main() {
    T1();
    T4();
    T2();

    T5();
}