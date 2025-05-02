#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/*  override is a C++11 keyword that tells the compiler:

"This function is meant to override a virtual function from a base class."

If it doesn't actually override a virtual function, the compiler gives you an error.*/

// Without override, you can make subtle mistakes
class Base {
public:
    virtual void print(int x) const {}
};

class Derived : public Base {
public:
    void print(int x) {  // ❌ Forgot `const` → doesn't override!
    // void print(int x) const override {
        std::cout << "Wrong print\n";
    }
};
// This compiles fine — but you get no polymorphism, and no error!
// void print(int x) override;  // ❌ COMPILER ERROR: doesn't match base method


/*  virtual void speak() const override final;
override: Must override base class virtual function
final: Cannot be overridden again in any subclass*/

// Example: Polymorphism with override
/*
void makeAnimalSpeak(const Animal& a) {
    a.speak();  // Will call derived version if virtual + override
}

Dog d;
makeAnimalSpeak(d);  // Outputs: Woof!
*/

// "This thing ends here — no more overriding or subclassing."

/*  final on a Class (Prevent Inheritance)*/
class Animal {
public:
    virtual void speak() const {
        std::cout << "Animal speaking\n";
    }
};

// Dog is final → no one can inherit from Dog
class Dog final : public Animal {
public:
    void speak() const override {
        std::cout << "Woof!\n";
    }
};

// ❌ Error: cannot inherit from final class
// class Bulldog : public Dog {
// };

void Final_class() {
    Dog d;
    d.speak();  // Output: Woof!
}

// final on a Method (Prevent Further Override)
class Base2 {
public:
    virtual void display() const {
        std::cout << "Base2 display\n";
    }

    virtual void important() const final {  // no more overriding
        std::cout << "Base2 important\n";
    }
};

class Derived2 : public Base2 {
public:
    void display() const override {  // OK to override
        std::cout << "Derived display\n";
    }

    // ❌ Error: cannot override final method
    // void important() const override {  
    //     std::cout << "Derived important\n";
    // }
};

void MethodFinal() {
    Derived2 d;
    d.display();    // Output: Derived display
    d.important();  // Output: Base2 important
}

    
int main() {


    return 0;
}