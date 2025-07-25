#include <iostream>

template <typename Derived>
class Parent {
public:
    Parent() = default;

    void doSomething() {
        Derived& d = static_cast<Derived&>(*this);
    }
};

class Derived : public Parent<Derived> {
public:
    Derived() = default;
};

// https://chatgpt.com/c/682873c6-2d40-8000-ad85-8c13fe9833c9