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
void T() {
    {
    Base* ptr = new Derived(); // base class pointer is used to make drived class object
    ptr->show();  // ❌ ERROR: Cannot call static function using a pointer to base
    delete ptr;
    }
}

// In header
class BaseD {
public:
    int x;
    static int y;
/*  Normal  datamembers are attribute of object
    static  datamembers are attribute of Class*/

    void printX() { cout << x << "\n"; }
    static void printY() { cout << y << "\n"; } // Static fun can only access static data member
};

// In CPP :- .h file will included here and there. So multiple definition error
int BaseD::y; // Define

void T2() {
    BaseD b1;
    b1.x = 10;

    BaseD::y = 5;
    // or
    b1.y = 5;

    b1.printY();
    // OR
    BaseD::printY();
}


/*  FUNCTION POINTER    */
int add(int a, int b) {
    return a + b;
}
int Calculate(int (*func)(int, int), int x, int y) {
    return func(x, y);
}
void FUNCTION_POINTER() {
    // Pattern: return_type (*pointer_name)(param1_type, param2_type);
    int (*op)(int, int) = add;
    std::cout << op(3, 4); // 7

    // Passing Function Pointers to Functions
    cout << Calculate(add, 3, 6) <<"\n";
}

// Returning Function Pointers from Functions
int subtract(int a, int b) { return a - b; }

int (*choose(char op))(int , int) {
    return (op == '+') ? add : subtract;
}
void RetFunPtr() {
    auto f = choose('+');
    std::cout << f(4, 2); // 6
}

void Array_of_Function_Pointers() {
    int (*ops[])(int, int) = {add, subtract};
    std::cout << ops[2](3, 4); // 12
}


// Pointer to Member Function
struct MyClass {
    void show() {
        std::cout << "Showing\n";
    }

    static void hello() {
        std::cout << "Static Hello\n";
    }
};
// Pattern: return_type (ClassName::*ptr)(params)
void (MyClass::*fp)() = &MyClass::show;

//  Pointer to Static Member Function
void (*fp2)() = &MyClass::hello;

void Pointer_to_Member_Function() {
    MyClass obj;
    (obj.*fp)();

    fp2();
}

// Function Pointer as Template Parameter
template<int (*F)(int, int)>
int call(int a, int b) {
    return F(a, b);
}

/*  Lambda Convertible to Function Pointer (captureless)
 ❗ Lambdas with captures can't be converted to function pointers. */

/*  Using typedef / using for Clean Syntax
using func_ptr = int(*)(int, int);
func_ptr p = add;
std::cout << p(2, 3); // 5

*/

// Function Pointer in Struct/Class

int main() {
    Pointer_to_Member_Function();
    return 0;
}
