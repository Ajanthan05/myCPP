#include <iostream>
using namespace std;

class Base {
public:
    Base() {
        cout << "Base Constructor\n";
    }
    static void show() {
        std::cout << "Base class static function" << std::endl;
    }

    virtual ~Base() {
        cout << "Base Distructor\n";
    }
};

class Derived : public Base {
public:
    Derived() {
        cout << "Derived Constructor\n";
    }
    static void show() {  // This does NOT override Base::show()
        std::cout << "Derived class static function" << std::endl;
    }

    ~Derived() {
        cout << "Derived Distructor\n";
    }
};

int main() {
    {
    Base* ptr = new Derived(); // base class pointer is used to make drived class object
    ptr->show();  // ❌ ERROR: Cannot call static function using a pointer to base
    delete ptr;
    }
    return 0;
}
