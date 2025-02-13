// const data members in a class are required to be initialized at the time of object construction because their values cannot be changed 
// after initialization. They cannot be assigned a value in the body of the constructor because assignment implies modification, which is 
// not allowed for const variables.

// Assignment occurs after the object is constructed and memory is already allocated.


// 1. For Initialization of Non-Static const Data Members
/*  Reason for initializing the const data member in the initializer list is because no memory is allocated 
separately for const data member, it is folded in the symbol table due to which we need to initialize it in the initializer list. 

Also, it is a Parameterized constructor and we don’t need to call the assignment operator which means we are avoiding one extra operation.
*/

#include<iostream>
using namespace std;

class Test {
    const int t;
public:
  //Initializer list must be used
    Test(int t):t(t) {}
    int getT() { return t; }
};

// 2. For Initialization of Reference Members
class Test2 {
    int &t;
public:
    Test2(int &t):t(t) {}  //Initializer list must be used
    int getT() { return t; }
};

/* 3. For Initialization of Member Objects that do not have a Default Constructor
In the following example, an object “a” of class “A” is a data member of class “B”, and “A” doesn’t have a default constructor. 
Initializer List must be used to initialize “a”.
*/
class A {
    int i;

public:
    A(int);
};

A::A(int arg)
{
    i = arg;
    cout << "A's Constructor called: Value of i: " << i
         << endl;
}

// Class B contains object of A
class B {
    A a;

public:
    B(int);
};

B::B(int x) : a(x)
{ // Initializer list must be used
    cout << "B's Constructor called\n";
}



// 4. For Initialization of Base Class Members
/* the parameterized constructor of the base class can only be called using the Initializer List.*/

class AA {
    int i;
public:
    AA(int );
};

AA::AA(int arg) {
    i = arg;
    cout << "AA's Constructor called: Value of i: " << i << endl;
}

// Class BB is derived from AA
class BB: AA {
public:
    BB(int );
};

BB::BB(int x):AA(x) { //Initializer list must be used
    cout << "BB's Constructor called\n";
}


// 5. When the Constructor’s Parameter Name is the Same as Data Member


// 6. For Performance Reasons
/*
It is better to initialize all class variables in the Initializer List instead of assigning values inside the body.

// Without Initializer List
class MyClass {
    Type variable;
public:
    MyClass(Type a) {  // Assume that Type is an already
                     // declared class and it has appropriate 
                     // constructors and operators
      variable = a;
    }
};
Here compiler follows following steps to create an object of type MyClass 

1. Type’s constructor is called first for “a”. 

2. Default construct “variable”

3. The assignment operator of “Type” is called inside body of MyClass() constructor to assign 

variable = a;
4. And then finally destructor of “Type” is called for “a” since it goes out of scope.


With the Initializer List, the following steps are followed by compiler: 

1. Type’s constructor is called first for “a”. 
2. Parameterized constructor of “Type” class is called to initialize: variable(a). The arguments in the initializer list are used to copy construct “variable” directly. 
3. The destructor of “Type” is called for “a” since it goes out of scope.

As we can see from this example if we use assignment inside constructor body there are three function calls: constructor + destructor + one addition assignment operator call. 
And if we use Initializer List there are only two function calls: copy constructor + destructor call.
*/



int main() {
    Test t1(10);
    cout<<t1.getT();

    // 2. For Initialization of Reference Members
    cout<< "\nFor Initialization of Reference Members\n";
    int x = 20;
    Test2 t2(x);
    cout<<t2.getT()<<endl;
    x = 30;
    cout<<t2.getT()<<endl;

    // 3 
    /* If class A had both default and parameterized constructors, 
    then Initializer List is not a must if we want to initialize “a” using the default constructor, 
    but it is must to initialize “a” using the parameterized constructor. 
    */
    B obj(10);

    // 4
    BB obj4(10);


    return 0;
}


/* Parameter vs Uniform Initialization in C++
It is better to use an initialization list with uniform initialization {} rather than parameter initialization () to avoid 
the issue of narrowing conversions and unexpected behavior. It provides stricter type-checking during initialization and prevents 
potential narrowing conversions*/