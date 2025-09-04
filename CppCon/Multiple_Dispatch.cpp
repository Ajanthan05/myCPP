#include <iostream>

#include <variant>
#include <fmt/format.h>
#include <string_view>

#include <any>
#include <typeindex>
#include <unordered_map>

using namespace std;

struct Circle;
struct Square;

struct Shape {
    virtual void collide(Shape* other) = 0;
    virtual void collideWith(Circle* c) = 0;
    virtual void collideWith(Square* s) = 0;
};

struct Circle : Shape {
    void collide(Shape* other) override {
        other->collideWith(this);   // second dispatch
    }
    void collideWith(Circle* c) override {
        std::cout << "Circle hits Circle\n";
    }
    void collideWith(Square* s) override {
        std::cout << "Circle hits Square\n";
    }
};

struct Square : Shape {
    void collide(Shape* other) override {
        other->collideWith(this);   // second dispatch
    }
    void collideWith(Circle* c) override {
        std::cout << "Square hits Circle\n";
    }
    void collideWith(Square* s) override {
        std::cout << "Square hits Square\n";
    }
};

// std::variant<int, float, std::string_view> get_variant() {
//     return 3.14f;
// }

// template<typename ... Callable>
// struct visitor : Callable... {
//     using Callable::operator()...;
// };
// void Pattern() {
//     const auto value = get_variant();
//     std::visit(
//         visitor{[](int i)   {fmt::print("Int: {}\n", i);},
//                 [](float f) {fmt::print("Float: {}\n", f);},
//                 [](std::string_view sv) {fmt::print("SV: {}\n", sv);}}
//         , value);
// }
 
// Returns a variant that currently holds a float
std::variant<int, float, std::string_view> get_variant() {
    return 3.14f;
}

// visitor helper (folds multiple lambdas into one)
template<typename... Callables>
struct visitor : Callables... {
    using Callables::operator()...;
};
/*  "overloaded" pattern (sometimes called lambda overloading or function object composition).

    What it does

Inherits from multiple function objects (Callables...).
Brings all of their operator() into the scope (using Callables::operator()...;).
The result: a single object with multiple operator() overloads.
So it’s basically a multi-callable wrapper.*/


// Deduction guide (C++17 requires this, C++20 deduces automatically)
template<typename... Callables> visitor(Callables...) -> visitor<Callables...>;

void Pattern() {
    const auto value = get_variant();
    std::visit(
        visitor{
            [](int i) { fmt::print("Int: {}\n", i); },
            [](float f) { fmt::print("Float: {}\n", f); },
            [](std::string_view sv) { fmt::print("SV: {}\n", sv); }
        },
        value
    );
}

/*  std::monostate instances do not store any meaningful value or data. All instances of std::monostate are considered equal.
std::monostate and variant with optional empty case

Useful if you want to handle an empty state as well */
void T() {
    std::variant<std::monostate, int, float> v = std::monostate{};

    std::visit(visitor{
        [](std::monostate){ std::cout << "Empty\n"; },
        [](int i){ std::cout << "Int " << i << "\n"; },
        [](float f){ std::cout << "Float " << f << "\n"; }
    }, v);

/*  With std::any + std::type_index dispatch
Instead of if/switch, you can overload lambdas for types*/
    std::any a = 3.14f;

    auto handler = visitor{
        [](int i){ std::cout << "int " << i << "\n"; },
        [](float f){ std::cout << "float " << f << "\n"; },
        [](auto&& other){ std::cout << "other type\n"; }
    };

    if (a.type() == typeid(int)) handler(std::any_cast<int>(a));
    else if (a.type() == typeid(float)) handler(std::any_cast<float>(a));

/*  Generic functor composition
You can combine arbitrary callables into a single object    */
auto fun = visitor{
    [](int x){ return x * 2; },
    [](std::string s){ return s + "!"; },
    [](auto v){ return v; } // fallback
};

std::cout << fun(5) << "\n";        // 10
std::cout << fun("Hello"s) << "\n"; // Hello!

/*  Event handler systems
Suppose you have an event system where events are different types   */
struct KeyEvent { int key; };
struct MouseEvent { int x, y; };

auto handler2 = visitor{
    [](KeyEvent e){ std::cout << "Key " << e.key << "\n"; },
    [](MouseEvent e){ std::cout << "Mouse " << e.x << "," << e.y << "\n"; }
};

std::variant<KeyEvent, MouseEvent> ev = KeyEvent{42};
std::visit(handler2, ev);
}


/*  Pattern matching in generic algorithms
Instead of writing multiple if constexpr or if chains, you can overload lambdas */
template<typename T>
void process(T val) {
    visitor{
        [](int i){ std::cout << "int " << i << "\n"; },
        [](double d){ std::cout << "double " << d << "\n"; },
        [](auto x){ std::cout << "other\n"; }
    }(val);
}
void T_process() {
    process(10);     // int 10
    process(3.14);   // double 3.14
    process("Hi");   // other
}


/*  constexpr destructor

Since C++20, destructors can be declared constexpr.
    This means: the destructor can be executed at compile time, if the object 
itself lives in constant-expression evaluation.
    Commonly useful for small RAII wrappers that should work in constexpr contexts.

Here, the destructor runs in constant evaluation.
If the destructor does something that’s not allowed in constant expressions (like delete), it won’t be constexpr.*/
struct X {
    int* p;
    constexpr X() : p(nullptr) {}
    constexpr ~X() { /* trivial, no dynamic delete */ }
};
constexpr void test() {
    X x;  // created + destroyed at compile-time
}



/*  [[nodiscard]]?

It’s a standard C++ attribute (since C++17) that tells the compiler:
“The return value of this function/type should not be ignored.”*/
[[nodiscard]] int compute() { return 42; }
void test2() {
    // compute(); // ⚠️ Warning: ignoring return value of 'compute'
}

// Types (classes, structs, enums)
struct [[nodiscard]] Error {
    int code;
};
Error make_error() { return {1}; }
void test() {
    // make_error(); // ⚠️ Warning: ignoring result of nodiscard type
}

/*  b) Resource management (std::unique_ptr)

std::unique_ptr is marked [[nodiscard]].
Why?
Because if you construct a unique_ptr and ignore it, the resource is destroyed immediately (likely a bug).

std::unique_ptr<int>(new int(5)); // ⚠️ Warning: discarded unique_ptr immediately deletes int

c) std::optional, std::expected (C++23)
They are [[nodiscard]] because ignoring them loses important “value-or-error” semantics.
*/

// ✅ [[nodiscard("reason")]] (C++20)
// You can give a custom diagnostic message.
[[nodiscard("Check return value to avoid resource leaks")]]
bool open_file() {};
// Much more readable for API users.
void test() {
    open_file(); // ⚠️ Warning: Check return value to avoid resource leaks
}





struct SpaceObject {
    constexpr virtual ~SpaceObject() = default;
    // [[nodiscard]] virtual constexpr auto get_name() const noexcept = 0;
    int x;
    int y;
};




int main() {
    Shape* a = new Circle();
    Shape* b = new Square();

    a->collide(b);  // "Circle hits Square"

    Pattern();
    T();
    test();
    test2();
}
