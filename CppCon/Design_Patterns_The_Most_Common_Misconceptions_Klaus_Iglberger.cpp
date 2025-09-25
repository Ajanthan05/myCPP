#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>
#include <memory>
#include <string>
// #include <print>
#include <string_view>
#include <fstream>

using namespace std;

/*  CRTP every derive type has different base type
What is CRTP: A class inherits from a template class that takes the derived 
class itself as a template parameter.

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

Can’t decide behavior at runtime (unlike virtual functions).    */


/*  static_cast<Derived*>(this)->drawImpl();

Casts this (a pointer to Shape<Derived>) into a pointer to the derived type.

Equivalent to:

Derived* d = static_cast<Derived*>(this);
d->drawImpl();


Calls the non-const overload of drawImpl() if available.

Requires this to be non-const.

2. static_cast<Derived const&>(*this).drawImpl();

Casts *this (a reference to Shape<Derived>) into a reference to the derived object.

Equivalent to:

Derived const& d = static_cast<Derived const&>(*this);
d.drawImpl();


Works even if the base function is marked const:

void draw() const {  // <- const member
    static_cast<Derived const&>(*this).drawImpl();
}


Calls the const overload of drawImpl() if it exists.

🔑 Key differences
Expression	Works with const base?	Calls which overload?	Syntax style
static_cast<Derived*>(this)->drawImpl();	❌ No (requires non-const this)	Non-const drawImpl()	Pointer style
static_cast<Derived const&>(*this).drawImpl();	✅ Yes	const drawImpl() if defined	Reference style */
// Static Polymorphism
template <typename Derived>
class Shape {
public:
    void draw() {
        static_cast<Derived*>(this)->drawImpl();
    }

    /*  Prevents anyone from creating or destroying an Animal<Derived> directly.
Enforces that only Derived (like Circle) can create an instance of itself.*/
private:
    Shape() = default;  // Prevent direct construction
    ~Shape() = default;
    friend Derived;     // ✅ Allow access to private constructor/destructor
};

class Circle : public Shape<Circle> {
public:
// Allowed to call Shape<Circle> — because it's a friend
    void drawImpl() { std::cout << "Drawing Circle\n"; }
};

class Square : public Shape<Square> {
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
    // Cannot compile since the "Self" type cannot be deduced to be the dynamic type
    // Shape sh;
    // sh.draw();
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
/*  🔹 Idea
    Define a runtime interface (abstract base class with virtual functions).
    Provide a CRTP adapter that implements the virtual functions by forwarding them to the derived class (via static polymorphism).

That way:
    At runtime, you can store different objects (Circle, Square, …) behind a common Shape*.
    Inside, the CRTP still avoids code duplication and allows compile-time optimization.

🔹 What’s Happening

shape is an IShape* → lets you decide at runtime which shape to use.

But Shape<Derived> uses CRTP → the virtual call forwards to Derived::drawImpl().

So you get:

Runtime flexibility (choose Circle or Square dynamically).

Compile-time performance benefits (no code duplication in each derived).

🔹 Where This is Used

Game engines: common base interface for components, but CRTP for optimization.

UI frameworks: runtime widgets, but static mixins for logging/tracing.

Scientific libraries (Eigen): runtime polymorphic API wrappers + CRTP for actual math kernels.

✅ Summary:

Pure virtual → runtime flexibility, slower.

Pure CRTP → compile-time only, very fast.

Hybrid → combines both: runtime choice + compile-time optimizations.*/
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
        static_cast<Derived &>(*this).drawImpl();
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

    // Object slicing
    // Circle2 c;
    // IShape s = c;  // ❌ slicing, object no longer a Circle2
    // s.draw();      // UB
    /*  🔹 Advantages

Runtime + Compile-time flexibility

Runtime: can pick shape type dynamically (unique_ptr<IShape>).

Compile-time: still get CRTP’s zero-cost forwarding (no virtual calls inside draw()).

No boilerplate in concrete classes

Derived only writes drawImpl().

No need to override draw() manually.

Type safety at the interface

IShape guarantees a draw() for all shapes.

No need for templates when writing print(IShape&).

Cleaner error messages

Without hybrid, every print<Circle>, print<Square>, etc. generates long template instantiations.

With hybrid, there’s just one virtual draw() in IShape.

🔹 Disadvantages

Virtual call overhead

One vtable lookup at runtime for shape->draw().

Tiny cost (1 indirect call), but not zero.

Matters in very hot loops with millions of calls.

Compile-time safety is reduced at the top level

With pure CRTP, a missing drawImpl() → compile error.

With the hybrid, missing drawImpl() may lead to a runtime error unless you guard with static_assert.

Const-correctness needs extra effort

Must duplicate draw() for const and non-const cases.

Slightly more boilerplate in the hierarchy

You introduce IShape + CRTP adapter + derived class.

Pure CRTP or pure virtual inheritance is simpler.

🔹 Where to use

✅ Good use cases

When you want runtime polymorphism at the top-level interface (e.g., a factory returning unique_ptr<IShape>).

But also want CRTP’s efficiency inside implementations.

Libraries/frameworks where external users need a clean virtual interface, but you still care about compile-time optimizations inside.

Example: GUI frameworks, plugin systems, AST node hierarchies.

❌ Avoid if:

You only need static polymorphism (compile-time dispatch only) → CRTP alone is enough.

You only need runtime polymorphism → virtuals alone are simpler.

You’re in performance-critical inner loops → virtual call overhead might dominate (stick to CRTP).*/
}




template<typename T>
struct Addable {
    T operator+(const T& other) const {
        const T& self = static_cast<const T&>(*this);
        return T(self.value + other.value);
    }
};

struct MyInt : Addable<MyInt> {
    int value;
    MyInt(int v): value(v) {}
};
/*  Addable<T> is a mixin template: it defines operator + assuming that the derived 
type T has a member value.

Inside operator+, we need access to the derived object (MyInt).

static_cast<const T&>(*this) casts the base (Addable<MyInt>) to the actual derived 
(MyInt).

This is the CRTP trick: recover the derived from the base.

Then we use the derived’s value field.

Finally, we construct a new T with the sum.


MyInt inherits from Addable<MyInt>.
So MyInt automatically gets operator+ for free.

// When you write:
MyInt a{5}, b{10};
MyInt c = a + b;   // uses Addable<MyInt>::operator+


Inside operator+:
    self is a (as MyInt&)
    other is b
    returns MyInt(self.value + other.value) → MyInt(15)*/



///////////////------------ CRTP as Mixin (adding functionality)
// base provides reusable implementation (extra functionality), derived 
// classes just inherit it.

// Adding Functionality with CRTP
template< typename Derived >
struct NumericalFunctions {
    void scale( double multiplicator ) {
        Derived& underlying = static_cast<Derived&>(*this);
        underlying.setValue( underlying.getValue() * multiplicator );
    }
};
struct Sensitivity : public NumericalFunctions<Sensitivity> {
    Sensitivity(double v) : value(v) {}   // constructor

    double getValue() const { return value; }
    void setValue( double v ) { value = v; }
    double value;
};
void T17() {
    Sensitivity s{ 1.2 };
    s.scale( 2.0 );
    std::cout << "s.getValue() = " << s.getValue() << "\n";
    //  std::println( std::cout, "s.getValue() = {}", s.getValue() );
}

// C++23 No template parameter needed
struct NumericalFunctions23 {
    void scale( this auto&& self, double multiplicator ) { // Explicit object parameter (aka "Deducing This")
         self.setValue( self.getValue() * multiplicator );
    }
};
struct Sensitivity23 : public NumericalFunctions23 {
    Sensitivity23(double v) : value(v) {}   // constructor

    double getValue() const { return value; }
    void setValue( double v ) { value = v; }
    double value;
};
void T23() {
    Sensitivity23 s{ 1.2 };
    s.scale( 2.0 );
    std::cout << "s.getValue() = " << s.getValue() << "\n";
    //  std::println( std::cout, "s.getValue() = {}", s.getValue() );
}

//-----------------20 
template <typename T>
concept HasValue = requires(T t, double v) {
    { t.getValue() } -> std::convertible_to<double>;
    { t.setValue(v) };
};
template<HasValue T>
void scale20(T& obj, double multiplicator) {
    obj.setValue(obj.getValue() * multiplicator);
}


// Reference style
/*  static_cast<Derived&>(*this).drawImpl();
Casts *this (a reference to Shape<Derived>) into a reference to Derived.
Then directly calls drawImpl() on that reference.
Derived& d = static_cast<Derived&>(*this);
d.drawImpl();
Works in both const and non-const overloads if you choose Derived const& or Derived&.
Safer style because it avoids null pointers (you can’t dereference a null reference).

static_cast<Derived*>(this)->drawImpl();
Casts this (a pointer to Shape<Derived>) into a pointer to Derived.
Then calls drawImpl() through the pointer.
Only works in the non-const version (since this is Shape<Derived>*, not const Shape<Derived>*).
If you want const support, you’d need another overload with static_cast<Derived const*>(this)->drawImpl();.
*/

template <typename Derived>
class ShapeRS {
public:
    void draw() {
        static_cast<Derived&>(*this).drawImpl();         // non-const
    }

    void draw() const {
        static_cast<Derived const&>(*this).drawImpl();   // const
    }

private:
    ShapeRS() = default;   // prevent direct construction
    ~ShapeRS() = default;
    friend Derived;      // allow Derived to construct/destruct
};
class CircleRS : public ShapeRS<CircleRS> {
public:
    void drawImpl() { std::cout << "Circle (non-const)\n"; }
    void drawImpl() const { std::cout << "Circle (const)\n"; }
};

class SquareRS : public ShapeRS<SquareRS> {
public:
    void drawImpl() { std::cout << "Square (non-const)\n"; }
    void drawImpl() const { std::cout << "Square (const)\n"; }
};
void Reference_Style() {
    CircleRS c;
    const CircleRS cc;
    SquareRS s;
    const SquareRS ss;

    c.draw();   // → Circle (non-const)
    cc.draw();  // → Circle (const)
    s.draw();   // → Square (non-const)
    ss.draw();  // → Square (const)
}


// ------------------CRTP C++20-----------
class AnimalTag {};
// Empty tag type used only for classification — 
//                          ensures only “intended” types can be Animals.

template <typename T>
concept Animal20 = requires( T animal ) { animal.make_sound(); } &&
                 std::derived_from<T, AnimalTag>;
// Type T must have a member function make_sound().
// Type T must inherit from AnimalTag.

template< Animal20 T >
void print20( T const& animal ) {
    animal.make_sound();
}
class Sheep : public AnimalTag {
public:
    void make_sound() const { std::cout << "baa\n"; }
};
void CRTP20() {
    Sheep sheep;
    print20(sheep);  // prints "baa"
}
/*  Why this is called a “Static Interface”

Like CRTP static interface (Shape<Derived>), it defines a compile-time contract:
“if you’re an Animal, you must implement make_sound() and derive from AnimalTag.”

Unlike runtime polymorphism (virtuals), this is checked at compile time.

If you forget make_sound(), you get a clear compile-time error.

    CRTP vs Concepts for Static Interfaces
| Feature               | CRTP Static Interface                  | Concept Static Interface (C++20)       |
| --------------------- | -------------------------------------- | -------------------------------------- |
| Enforces interface    | via `static_cast` calls into Derived   | via `requires` clause in concept       |
| Error messages        | often cryptic (template instantiation) | very clear (“T must have make\_sound”) |
| Base/Derived coupling | tight (inheritance required)           | loose (any type can satisfy concept)   |
| Flexibility           | fixed hierarchy                        | ad-hoc satisfaction (“duck typing”)    |

✅ When to use

Use CRTP static interfaces if you want to also provide some default implementation (like draw() calling drawImpl()).

Use concepts if you only need to enforce the contract and don’t need shared base functionality.*/



///////////////////////////--------- std::varient ----------------------------
template <typename ConcreteShape>
class DrawStrategy {
public:
    virtual ~DrawStrategy() = default;

    virtual void draw( ConcreteShape const& shape) const = 0;
};

class SHape {
public:
    virtual ~SHape() = default;

    virtual void draw() const = 0;
};
class CIrcle : public SHape {
public:
    CIrcle( double r, std::unique_ptr<DrawStrategy< CIrcle >>&& ds)
        : radius{r}, drawer{ std::move(ds) } {}

    double getRadius() const { return radius; }

    void draw() const override {
        if (drawer) drawer->draw(*this);
        else std::cout << "Circle: radius=" << radius << '\n';
    }

private:
    double radius;
    std::unique_ptr<DrawStrategy<Circle>> drawer;
};

class SQuare : public SHape {
public:
    SQuare( double s, std::unique_ptr<DrawStrategy<SQuare>>&& ds )
        : side{ s }, drawer{ std::move(ds) } {}
    
    double getSide() const { return side; }

    void draw() const override {
        if (drawer) drawer->draw(*this);
        else std::cout << "Square: side=" << side << '\n';
    }

private:
    double side;
    std::unique_ptr<DrawStrategy<SQuare>> drawer;
};

using SHAPES = std::vector< std::unique_ptr<SHape> >;

class ShapesFactory {
public:
    virtual ~ShapesFactory() = default;
    virtual SHAPES create( std::string_view filename ) const = 0;
};

void drawAllShapes( SHAPES const& shapes ) {
    for( auto const& s : shapes ) {
        s->draw();
    }
}

void createAndDrawShapes( ShapesFactory const& factory, std::string_view filename ) {
    SHAPES shapes = factory.create( filename );
    drawAllShapes( shapes );
}

class OpenGLDrawer : public DrawStrategy<CIrcle>
                   , public DrawStrategy<SQuare> {
public:
    explicit OpenGLDrawer() {}

    void draw( CIrcle const& c ) const override    {
        std::cout << "[OpenGL] Circle radius = " << c.getRadius() << '\n';
    }
    void draw( SQuare const& s ) const override {
        std::cout << "[OpenGL] Square side = " << s.getSide() << '\n';
    }
};

class YourShapesFactory : public ShapesFactory{
public:
    SHAPES create( std::string_view filename ) const override {
        SHAPES shapes{};
        std::string shape{};
        std::ifstream shape_file{ filename };
        while( shape_file >> shape ) {
            if( shape == "circle" ) {
                double radius;
                shape_file >> radius /* >> color, texture, transparency, ... */;
                shapes.emplace_back(
                std::make_unique<Circle>( radius, std::make_unique<OpenGLDrawer>(/*...*/) ) );
            }
            else if( shape == "square" ) {
                double side;
                shape_file >> side /* >> color, texture, transparency, ... */;
                shapes.emplace_back(
                std::make_unique<Square>( side, std::make_unique<OpenGLDrawer>(/*...*/) ) );
            }
            else break;
        }
        return shapes;
    }
};
void Test() {
    // YourShapesFactory factory{};
    // createAndDrawShapes( factory, “shapes.txt” );

    // create a small sample file
    const char* fname = "shapes.txt";
    {
        std::ofstream out(fname);
        out << "circle 1.5\n";
        out << "square 2.0\n";
        out << "circle 3.14\n";
    }

    YourShapesFactory factory{};
    SHAPES shapes = factory.create(fname);
    drawAllShapes(shapes);

}

int main() {
    
    // Static_Polymorphism();

    // Hybrid_CRTP_Virtual();

    T23();
    CRTP20();

    return 0;
}