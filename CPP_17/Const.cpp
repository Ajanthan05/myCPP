#include <iostream>
#include <vector>
#include <climits>
#include <string>
using namespace std;

/*  C++insists that u must initialize every const object 

*/
void T() {
    const int MAX_AGE = 90;

    int *a = new int;

    *a = 2;
    // a = &MAX_AGE;  value of type "const int *" cannot be assigned to an entity of type "int *"
    a = (int*)&MAX_AGE; // break the promice

    cout << *a << endl;

    const int *ca = new int; // Cannot modify the context of the pointer
    // *ca = 2; Cant change the data at the memory address
    int const* sameAsAvove_ca = new int; 

    int* const ca2 = new int; // can change context of the pointer
    // Can't reassign the pointer itself
    *ca2 = 2;
    // ca2 = &MAX_AGE;

    const int* const a2 = new int;
}

class Entity {
private:
    int m_X, m_Y;
    int *ptr;
    mutable int var;
public:
    int GetX() const { // This method is not going to modify any of the actual class
        var = 5;  // we are able to modify even if we are in const method
        return m_X;
    }

    // Returning a pointer that can't be modified
    // Context of the pointer cannot be modified
    const int* const GetPtr() const {
        return ptr;
    }
};

void PrintEntity(const Entity& e) {
    // e = nullptr; // Error
    // e = Entity();  // There are no different between pointer and it content
    cout << e.GetX() << endl;
}


class Entity2 {
private:
    string name;
    mutable int m_DebugCount = 0;
public:
    const string& GetName() const {
        m_DebugCount++;
        return name;
    }
};
void T2() {
    const Entity2 e; // We won't able to do it without GetName() const
    e.GetName();

    // Ex2   mutable 
    int x = 5;
    auto f = [=]() mutable {
        x++;
        cout << x << endl;
    };

    f();
}

/*  mutable keyword in C++ allows a member of a class/struct to be modified even if the containing object is const.
    1. const member functions
    2. situations involving std::function, std::thread, or lambdas with [=] mutable
    

Rule	Explanation
✅ Can only be used on non-static data members	
❌ Cannot be applied to const variables (makes no sense)	
✅ Works in const member functions	
❌ Not allowed on functions or local variables	
✅ Plays well with std::atomic (for thread-safe mutation)

*/




int main() {
    Entity x;
    PrintEntity(x);

    return 0;
}