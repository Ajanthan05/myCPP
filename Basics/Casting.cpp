/*
char → int

int → float

float → double

2. Class Constructors with One Parameter (Implicit Conversion Constructors)
If a class has a constructor that takes a single argument, C++ allows implicit conversion:

cpp
Copy
Edit
class Distance {
public:
    Distance(int meters) { / }  // Implicit constructor
};

Distance d = 100;  // No cast needed — 100 is implicitly converted to Distance
3. Assignment-Compatible Types
When assigning values between compatible types, C++ handles conversion automatically:

float f = 10;      // int → float
double d = f;      // float → double
4. Function Arguments Matching Parameter Types
If the function expects a type that your argument can be implicitly converted to, casting is not needed:


void printDouble(double x) {}

int a = 42;
printDouble(a);  // int → double, no cast required
5. Standard Arithmetic Promotions
C++ applies usual arithmetic conversions in expressions:

int a = 5;
float b = 3.2;
auto result = a + b;  // a is converted to float automatically
6. Inheritance and Polymorphism (Base Class Pointers)
Upcasting from derived to base class pointer/reference is implicit:

class Base {};
class Derived : public Base {};

Derived d;
Base* b = &d;  // Implicit upcast
⚠️ When You Do Need Explicit Type Casting
Just for contrast, here's when casting is required:

Situation	Example	Notes
Narrowing conversions	double x = 3.14; int y = (int)x;	Might lose precision
Downcasting in inheritance	Derived* d = (Derived*)b;	Requires static_cast or dynamic_cast
Converting between unrelated types	void* ptr = (void*)&x;	Unsafe, must cast manually
Disabling implicit conversion	Use explicit keyword	Prevents automatic conversion*/


#include <iostream>
#include <iomanip>

#include <cstdint>
using namespace std;

void Test_iomanip() {
    double pi = 3.141592653589;

    cout << scientific << setprecision(3) << pi << endl;
}


class Entity {
    int x;
public:
    Entity(int x) : x{x} { cout << "Entity\n"; }
};

class Game {
    int y;
public:
    Game(int y) : y{y} { cout << "Game\n"; }
};

class Player : public Entity, public Game {
public:
    Player(int x, int y) : Entity{x}, Game{y} {

    }
};

void test() {
    Player* player = new Player(3,5);
    Entity *e = player;
}



/*  const_cast<T>(v) can be used to change the const or volatile qualifiers of pointers or reference.
Where T must be a pointer, reference, or pointer-to-member type.
*/

void T_const_cast() {
    const int a1 = 10;
    const int *ptr1 = &a1;
    int *dptr1 = const_cast<int*>(ptr1);
    *dptr1 = 15;    // Invalid and Undifined Behavior
/*Might crash.
    Might silently ignore the write.
    Might corrupt memory.
    Might appear to "work" on some compilers/platforms, but still be invalid C++.*/
    cout << a1 << "\n";  // o/p: 10 boc in combile time it been replaced to=> cout << 10 << "\n"; 
    cout << *dptr1 << "\n";

    // If the object was not originally const, but you have a const pointer to it, then const_cast can be used safely:
    int a = 10;               // NOT const
    const int* p = &a;        // const pointer to it
    int* q = const_cast<int*>(p);
    *q = 15;                  // OK, because original object is not const
    cout << a << endl;        // Prints 15
    // In this case, you’re casting away const only on the pointer, not the original object.
}

/* Use const_cast
1. When the actual referred object/variable is not const 
2. When we need to call 3'rd party library where it is takintg varaiable or object as non-const but not changing that.
*/
void thardPartyLibrary(int *x) {
    int k = 10;
    cout << k + *(x) << "\n";
}
void T_thardPartyLibrary() {
    const int x = 20;
    const int *px = &x;
    // thardPartyLibrary(px);     thardPartyLibrary(px);

    thardPartyLibrary(const_cast<int*>(px));
}
/*  C++ rule for const_cast:

❗️You must not modify an object that was originally declared as const, even if you cast away const using const_cast.
Merely accessing or reading it via const_cast is not undefined behavior, as long as you don’t modify it.

t is technically undefined behavior, because the standard says:

If an object is defined as const, any attempt to access it through a non-const pointer is undefined behavior, even if it's just for reading.

(Because the standard assumes that if you cast to a non-const pointer, you might write to it — and allows compilers to optimize accordingly.)

HOWEVER:
In practice, this will work on nearly every compiler.

It's "conditionally undefined behavior" — reading a const object through a non-const pointer doesn't break things on most platforms, but is still not portable or safe by the standard.
*/

/* Dynamic cast must have polimorphic class type:- need to have virtual function
It make this class to have vTable
*/

void Test_const_cast() {
    T_const_cast();
    T_thardPartyLibrary();

    enum Level
    {
        Error = 0, Warning, Info
    };
    Level l = Error;
    cout << "Enum Size:" << sizeof(l) << "\n";
}


void Static_cast() {
    // 1. It performs implicit conversions between types.
    float f = 3.4;
    int a;

    a = f;
    a = static_cast<int>(f); // Both are same. This conversion heppen at compile time
    /*  Why to use static_cast when implicit conversion is involved?
    Boc c-style cast is hared to find in code, but u can easly search static_cast keyword
    */ 
} 

/*  2. Use static_cast when conversion between types is provided through 
conversion operator or conversion constructor.*/
class Int {
    int x;
public:
    Int(int x=0):x{x} { cout << "Conversion Operator\n"; }

    operator string () {
        cout << "Conversion Operator\n";
        return to_string(x);
    }
    
};

void T2_static_cast() {
    Int obj(5);

    string str1 = obj;
    obj = 20;

    string str2 = static_cast<string>(obj);
    obj = static_cast<Int>(30);
}

class IPAddress {
    uint32_t ip;
public:
    IPAddress(uint32_t ip): ip(ip) {}
    operator string() const {
        return std::to_string((ip>>24)&0xFF) + "." + 
               std::to_string((ip>>16)&0xFF) + "." +
               std::to_string((ip>>8)&0xFF) + "." +
               std::to_string(ip&0xFF);
    }
};
void Usecase() {
    // Logging
    IPAddress ip(0xC0A80001);
    std::cout << string(ip) << "\n";  // "192.168.0.1"
}

/*  3. static_cast is more restrictive than C_Style.
char* to int* is allowec in C-Style but not with static_cast*/
void T3() {
    char c;             // 1 byte data
    int *p = (int*)&c;  // 4 byte data
    *p = 5;             // Pass compile time fail at runtime

    // int *ip = static_cast<int*>(&c); Fail in combile time
}

// 4. static_cast avoid cast from derived to private base pointer
class BaseSC {};
class DerivedSC : private BaseSC {};
void T4() {
    DerivedSC *d1;
    BaseSC *bp1 = d1;
}

// 5. Use for all upcast, but never use for confused down cast. Boc there ro no runntime \
// check peformed of static_cast conversion.

// 6. static_cast should be prefered when converting to void* OR from void*.
void T6() {
    int i = 10;
    void *v = static_cast<void*>(&i);
    int *ip = static_cast<int*>(v);
}



/*  dynamic_cast is used at run time to find out correct down cast
Need one viartual function in base class

1. work only on polymorphic base class ( at least one virtual function in base class)
*/

/*  reinterpret_cast  
WARNING: If we use this type of cast it becomes non-portable product
    1.  It can perform dangerous conversion boc it can typecast nny pointer to
any other pointer*/

/*  2.  reinterpret_cast used when u want to work with bits*/
struct myStruct {
    int x;
    int y;
    char c;
    bool b;
};
void T5() {
    myStruct s;
    s.x = 5; s.y = 10; s.c = 'a'; s.b = true;
    int *p = reinterpret_cast<int*>(&s);
    cout << *p << endl;
    p++;
    cout << *p << endl;
    p++;
    char *c = reinterpret_cast<char*>(p);
    cout << *c << endl;
    cout << *(reinterpret_cast<bool*>(++c)) << endl;

}



int main() {
    T2_static_cast();
    T5();

    T4();
}