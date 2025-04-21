#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

/* NOTE
The default move operations are generated
    => if no copy operation or destrutor is user defined

The default copy operations are generated
    => if no move operation is user defined

=> default and = delete count as user-defined!

class X {
public:
    virtual ~X() = default;

// CORE GUIDLINE C.21: If u define or =delete any default operation, define ro =delete them all
};
*/



/* class Widget {
private:
    int i{0};
    std::string s{};
    unique_ptr<int> pi{};

public:
    // ...

    // Move sonstructor
    Widget(Widget&& w) = default;

    // Move assignment operator
    Widget& operator=(Widget&& w) = default;

};
*/

class Widget {
private:
    int i{0};
    std::string s{};
    int* pi{nullptr};

public:
    // ...

    // Move sonstructor
    // 1. Transwer the content of w into this
    // 2. Leave w in a valid but undefined state
    /*
    Widget(Widget&& w)  // w hase a name, so it's an lvalue, so it cannot or should not move, since its needed hence i do a copy
        : i (w.i)
        , s (w.s) // Copies the string, w is an lvalue
    {
    }
    */

    /*
    Widget(Widget&& w)  // but "Widget&&" the type suggests it's an rvalue reference. 
                        // it's an rvalue in the calling scope
        noexcept
        // Phase 1: Member-wise move
        : i (std::move(w.i)) // correct but no speed up
        , s (std::move(w.s))
        , pi(std::move(w.pi))
    {
        // Phase 2: Reset    
        /* if Phase 1 is only need you re fine default
        w.pi = nullptr; // this point we have 2 object pointing the same memory 
        w.i = 0; // Purely optional, not done by default!
    }
    */

// CORE GUIDLINE C.64: A move operation should mome and leave its source in a valid statec
// CORE GUIDLINE C.66: Make move operation nonexcept.

    Widget(Widget&& w)  // but "Widget&&" the type suggests it's an rvalue reference. 
                        // it's an rvalue in the calling scope
        : i (std::move(w.i)) // correct but no speed up
        , s (std::move(w.s))
        , pi(std::exchange(w.pi, nullptr)) // set w.pi to null pointer and it return old value of w.pi
    {
    }

    // Move assignment operator
    /* The Goal
    1. Clean up all visible resources
    2. Transfer the content of w into this
    3. Level w in a valid but undefined state
    */
    Widget& operator=(Widget&& w) noexcept // Goal 3
    {
        // Phase 1: Cleanup
        delete pi;   //=>1 Goal 1
        // Phase 2: Member-wise move
        i = std::move(w.i);
        s = std::move(w.s);
        pi = std::move(w.pi);  //=>2 overwritten own pointer

        // Phase 3: Reset
        w.pi = nullptr; //=>3 Goal 2

        w.i = 0; // Purely optional, not done by default!

        // OR
        // pi = std::exchange(w.pi, nullptr);  //=>insted of 2&3
        // OR insted 1,2,3
        // std::swap(pi, w.pi);  // Less deterministic Gole 1&2 satistify, but when, evantualy 


        return *this;
    }

};

class Widget_default {
private:
    int i{0};
    std::string s{};
    // int* pi{nullptr};
    unique_ptr<int> pi{};

public:

// Move constructor
Widget_default (Widget_default&& ) = default; // NOTE: also noexcept!  it will move


// Move constructor
Widget_default& operator=(Widget_default&& w) = default;

};


class Widget_another_approach {
private:
    int i{0};
    std::string s{};
    int* pi{nullptr};

public:

    // Copy+move assignment operator
    /* I pass the class by value. by doing this u can astually just swap them*/
    Widget_another_approach& operator=(Widget_another_approach w)
    {
        swap(w);
        return *this;
    }

    void swap(Widget_another_approach& w)
    {

    }
};

int main() {


    return 0;
}