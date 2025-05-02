#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

namespace E
{
    namespace V   // V is a member of E, and is fully defined within E
    { // namespace E::V { // C++17 alternative to the lines above
        class C { void m(); }; // C is a member of V and is fully defined within V
                               // C::m is only declared
        void f(); // f is a member of V, but is only declared here
    }
 
    void V::f() // definition of V's member f outside of V
                // f's enclosing namespaces are still the global namespace, E, and E::V
    {
        extern void h(); // This declares ::E::V::h
    }
 
    void V::C::m() // definition of V::C::m outside of the namespace (and the class body)
                   // enclosing namespaces are the global namespace, E, and E::V
    {}
}


namespace Q {
    namespace V {
        class C {
        public:
            void m();  // declared
        };

        void f();  // declared
        void h();  // declared for demonstration
    }

    // Define function f() of Q::V
    void V::f() {
        std::cout << "Inside Q::V::f()\n";
        extern void h(); // Declares Q::V::h
        h();        // Calls Q::V::h
    }

    // Define C::m()
    void V::C::m() {
        std::cout << "Inside Q::V::C::m()\n";
    }

    // Define Q::V::h
    void V::h() {
        std::cout << "Inside Q::V::h()\n";
    }
}

namespace A {
    void helper() {
        std::cout << "In helper\n";
    }
    void func() {
        // extern void helper(); // Declares A::helper()
        helper();
    }
}

typedef void void_tt;
// or both are same
using void_t = void;

;template<class T, size_t>
struct DataStructure {

};

// typedef DataStructure<int, 10> DataStructureInt10;
using DataStructureInt10 = DataStructure<int, 10>;

int main() {
    Q::V::C obj;
    obj.m();     // Call member function
    Q::V::f();   // Call namespace function

    A::helper();
    return 0;
}
