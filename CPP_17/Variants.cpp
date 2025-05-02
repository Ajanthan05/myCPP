#include <iostream>
#include <variant>
#include <string>


/*  std::variant is a type-safe union that can hold exactly one value out of several possible types. Unlike raw unions, it:

Tracks which type is currently active.

Prevents unsafe access.

Offers built-in tools to safely get and operate on the contained value.*/

// ✅ Accessing the Value
// 1. std::get<T>(variant)     Returns the stored value if it matches type T, else throws std::bad_variant_access.
void test() {
    std::variant<int, float, std::string> v;

    v = 42;          // holds an int
    v = 3.14f;       // now holds a float
    v = "hello";    // now holds a std::string
    std::cout << std::get<std::string>(v) << "\n"; // OK
    // std::get<float>(v);         // ❌ throws if wrong type
}

/*  🧠 Why Use std::variant Over Union?

Feature	                    union	        std::variant
Type-safe access	        ❌ No	        ✅ Yes
Type tracking	            ❌ Manual	    ✅ Automatic
Multiple non-trivial types	❌ Hard	        ✅ Easy
Throws on invalid access	❌ Undefined	    ✅ std::bad_variant_access
Integrates with std::visit	❌ No	        ✅ Yes (see below)
*/

// ✅ 1. std::get_if<T>(&variant) Example 
// Returns a pointer to the stored value if it matches type T, else returns nullptr.
void get_If() {
    std::variant<int, float, std::string> v = 42;

    if (auto p = std::get_if<int>(&v)) {
        std::cout << "int: " << *p << "\n";
    } else if (auto p = std::get_if<float>(&v)) {
        std::cout << "float: " << *p << "\n";
    } else if (auto p = std::get_if<std::string>(&v)) {
        std::cout << "string: " << *p << "\n";
    }
}

// 3. std::holds_alternative<T>(variant)
// Returns true if the variant currently holds type T.
// Can't Accesses the stored value
void holds_Alternative() {
    std::variant<int, float, std::string> v = std::string("hello");

    if (std::holds_alternative<int>(v)) {
        std::cout << "variant holds int\n";
    } else if (std::holds_alternative<float>(v)) {
        std::cout << "variant holds float\n";
    } else if (std::holds_alternative<std::string>(v)) {
        std::cout << "variant holds string\n";
    }
}

// 🧰 Using std::visit – the Visitor Pattern
void visitT() {
    std::variant<int, float, std::string> v = 10;

    std::visit([](auto&& val) {
        std::cout << "Value is: " << val << '\n';
    }, v);
}


using VariantType = std::variant<int, float, std::string>;

void print_variant(const VariantType& v) {
    std::visit([](auto&& arg) { std::cout << arg << std::endl; }, v);
}

int main() {
    VariantType v = 42;
    print_variant(v); // prints 42

    v = 3.14f;
    print_variant(v); // prints 3.14
                 
    v = "Hello, Variant!";
    print_variant(v); // prints Hello, Variant!

    test();

// 🏷️ Indexing with .index()    Returns the 0-based index of the currently held type:
    std::variant<int, float, std::string> v = "hello";
    std::cout << v.index(); // 2 (std::string is third type)

    
}
