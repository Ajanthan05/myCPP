#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>
#include <memory>
// #include <print>
using namespace std;

/*  CRTP every derive type has different base type

What is CRTP: A class inherits from a template class that takes the derived class itself as a template parameter.

🔹 Why do this?

Static polymorphism (compile-time polymorphism).

Like virtual functions, but without vtables or runtime overhead.

Compiler inlines everything → zero-cost abstraction.

Code reuse:
Base implements common logic, derived implements specific behavior.

Mixins:
You can add functionality (like logging, counting, cloning) via CRTP base classes.

| Feature       | Virtual Functions (OOP)   | CRTP (Compile-time)        |
| ------------- | ------------------------- | -------------------------- |
| Dispatch type | Runtime (via vtable)      | Compile-time (static cast) |
| Overhead      | Small (indirection)       | None (inlined)             |
| Extensibility | More flexible at runtime  | Fixed at compile-time      |
| Use case      | Plugins, polymorphic APIs | Performance-critical code  |

🔹 Key Benefits

Performance: avoids runtime overhead.

Flexibility: can build powerful frameworks (Eigen, ranges, Boost).

Type safety: compiler checks everything at compile-time.

🔹 Limitations

Compile-time only (no runtime polymorphism).

Code bloat (templates generate code for each instantiation).

More complex syntax → can confuse beginners.

Can’t decide behavior at runtime (unlike virtual functions).


*/
// Static Polymorphism
template <typename Derived>
class Shape {
public:
    void draw() {
        static_cast<Derived*>(this)->drawImpl();
    }

    Shape() = default;
    friend Derived;
};

class Circle : public Shape<Circle> {
public:
    void drawImpl() { std::cout << "Drawing Circle\n"; }
};

class Square : public Shape<Circle> {
public:
    void drawImpl() { std::cout << "Drawing Square\n"; }
};

/*  This function must be a function template to take
any kind of animal. Thus CRTP can act like a virus:
Everything touching CRTP is or becomes a template
(including higher compile times).*/
template<typename Derived>
void print(Shape<Derived>& shape) {    // void print(Shape<Derived> const& shape) {  "I can’t call a non-const method on a const object."
    cout << "Inside Print Function: ";
    shape.draw();
    cout << "\n\n";
}

void Static_Polymorphism() {
    Circle c;
    Square s;
    c.draw();
    s.draw();
    print(c);
}

// Mixins (extra behavior)
template <typename Derived>
class Counter {
    inline static int count = 0;
public:
    Counter() { ++count; }
    ~Counter() { --count; }

    static int getCount() { return count; }
};

class MyClass : public Counter<MyClass> {};
class AnotherClass : public Counter<AnotherClass> {};
// Each derived class has its own independent counter because CRTP makes a new instantiation.
void Test() {
    MyClass a, b;
    AnotherClass c;

    std::cout << MyClass::getCount() << "\n";     // 2
    std::cout << AnotherClass::getCount() << "\n"; // 1
}


// Hybrid CRTP + Virtual
// Runtime interface (abstract base class)
class IShape {
public:
    virtual void draw() = 0;
    virtual ~IShape() = default;
};

// CRTP Adapter
template <typename Derived>
class Shape2 : public IShape {
public:
    void draw() override {
        // Static dispatch to derived implementation
        static_cast<Derived*>(this)->drawImpl();
    }
};

// Concrete classes
class Circle2 : public Shape2<Circle2> {
public:
    void drawImpl() { std::cout << "Drawing Circle\n"; }
};

class Square2 : public Shape2<Square2> {
public:
    void drawImpl() { std::cout << "Drawing Square\n"; }
};

void Hybrid_CRTP_Virtual() {
    std::unique_ptr<IShape> shape;

    int choice;
    std::cout << "Enter 1 for Circle, 2 for Square: ";
    std::cin >> choice;

    if (choice == 1) shape = std::make_unique<Circle2>();
    else shape = std::make_unique<Square2>();

    shape->draw();  // ✅ Runtime decision, CRTP dispatch inside
}


// Adding Functionality with CRTP
template< typename Derived >
struct NumericalFunctions
{
 void scale( double multiplicator )
 {
 Derived& underlying = static_cast<Derived&>(*this);
 underlying.setValue( underlying.getValue() * multiplicator );
 }
};
struct Sensitivity : public NumericalFunctions<Sensitivity>
{
 double getValue() const { return value; }
 void setValue( double v ) { value = v; }
 double value;
};
void T23() {
 Sensitivity s{ 1.2 };
 s.scale( 2.0 );
//  std::println( std::cout, "s.getValue() = {}", s.getValue() );
}
int main() {
    Static_Polymorphism();
    T23();
    return 0;
}