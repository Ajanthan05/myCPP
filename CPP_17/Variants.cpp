#include <iostream>
#include <variant>
#include <string>
#include <string_view>
#include <fmt/format.h>

/*  std::variant is a type-safe union that can hold exactly one value out of several possible types. Unlike raw unions, it:
    1. Tracks which type is currently active.
    2. Prevents unsafe access.
    3. Offers built-in tools to safely get and operate on the contained value.*/

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

std::variant<int, float, std::string_view, double> get_variant() {
    return 4.3;
}

// template<typename... Callable>
// struct visitor : Callable...  {
//     using Callable::operator()...;
// };

template<class... Ts>
struct Overloaded : Ts... {  // This is multiple inheritance.
    using Ts::operator()...;
};
//  CTAD: Class Template Argument Deduction
template<class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;
/*  The Overloaded struct allows you to combine multiple lambdas or 
functors into a single callable object with overloaded operator()s. 
This is very useful when you want to pass multiple lambdas to std::visit.

Ts... is a parameter pack, representing a list of types.

*/


void T_visit() {
    const auto val = get_variant();
    std::visit([](const auto& v) {
        // fmt::print("Val: {}\n", v);
        std::cout << "Val: " << v << "\n";
    }, val);

    struct Visitor {
        void operator()(int i) const {
            fmt::print(" : {}\n", i);
        }
        void operator()(float f) const {
            fmt::print("float : {}\n", f);
        }
        void operator()(std::string_view sv) const {
            fmt::print("SV : {}\n", sv);
        }
        void operator()(double d) const {
            fmt::print("Double : {}\n", d);
        }
    };

    std::visit(Visitor{}, val);

    // std::visit(
    //     visitor{[](int i){fmt::print(" : {}\n", i);},
    //             [](int f){fmt::print("float : {}\n", f);},
    //             [](std::string_view sv){fmt::print("SV : {}\n", sv);},
    //             [](double d){fmt::print("Double : {}\n", d);}
    //            }, val);

    std::variant<int, std::string> v = "hello";

std::visit(Overloaded{
    [](int x) { std::cout << "int: " << x << "\n"; },
    [](const std::string& s) { std::cout << "string: " << s << "\n"; }
}, v);

}

// AST (Abstract Syntax Tree) nodes or expression evaluation, using modern C++17/20 features
// Forward declaration
struct Expr;

// Expression nodes
struct Number {
    int value;
};

struct Add {
    std::unique_ptr<Expr> lhs, rhs;
};

struct Multiply {
    std::unique_ptr<Expr> lhs, rhs;
};

// Expr is a variant of all node types
struct Expr : std::variant<Number, Add, Multiply> {
    using variant::variant;  // inherit constructors
};
/*  Expr is a std::variant that can hold any node (Number, Add, Multiply)
Nodes use std::unique_ptr<Expr> to support recursive nesting */
// template<class... Ts>
// struct Overloaded : Ts... {
//     using Ts::operator()...;
// };
// template<class... Ts>
// Overloaded(Ts...) -> Overloaded<Ts...>;

int evaluate(const Expr& expr) {
    return std::visit(Overloaded{
        [](const Number& n)     { return n.value; },
        [](const Add& a)        { return evaluate(*a.lhs) + evaluate(*a.rhs); },
        [](const Multiply& m)   { return evaluate(*m.lhs) * evaluate(*m.rhs); }
    }, expr);
}

void Build_and_Evaluate_an_Expression() {
    // Construct: (3 + 4) * 2
    Expr expr = Multiply{
        std::make_unique<Expr>(Add{
            std::make_unique<Expr>(Number{3}),
            std::make_unique<Expr>(Number{4})
        }),
        std::make_unique<Expr>(Number{2})
    };

    std::cout << "Result: " << evaluate(expr) << "\n";  // Result: 14
}
/*  Why use std::unique_ptr<Expr>?
Recursive Tree Structures:
Each Add, Multiply, etc., contains nested Expr.
Since C++ needs to know the size of members at compile time, you can’t do this directly:

struct Add {
    Expr lhs, rhs;  // ❌ Error: incomplete type
}
std::unique_ptr<Expr> defers full type resolution, enabling recursion.

Automatic Memory Management:
    No need to manually delete anything.
    The AST will automatically clean up when it goes out of scope.
    No leaks!

Enforces Ownership:
    unique_ptr expresses that only one node owns each subtree.
    Prevents accidental copying or sharing of nodes.*/



int main() {
//     VariantType v = 42;
//     print_variant(v); // prints 42

//     v = 3.14f;
//     print_variant(v); // prints 3.14
                 
//     v = "Hello, Variant!";
//     print_variant(v); // prints Hello, Variant!

//     test();

// // 🏷️ Indexing with .index()    Returns the 0-based index of the currently held type:
//     std::variant<int, float, std::string> v = "hello";
//     std::cout << v.index(); // 2 (std::string is third type)

    T_visit();
    Build_and_Evaluate_an_Expression();
}
