
/*
If 1 virtual function in class VTables are created
VTables are lookup tables which have function pointers in there

it created at compile time, they cannot be static Static functions belong 
to the class itself, not to an instance. They do not have access to the 
this pointer and cannot be overridden in the derived class dynamically.

No, you do not need to declare a virtual destructor in every derived class as long as the base class has a virtual destructor.

Once the base class destructor is virtual, all derived class destructors become virtual automatically.

*/

// C++ program to demonstrate how we will calculate
// the area of shapes USING VIRTUAL FUNCTION
#include <fstream>
#include <iostream>
using namespace std;

// Declaration of Base class
class Shape {
public:
	// Usage of virtual constructor
	virtual void calculate()
	{
		cout << "Area of your Shape ";
	}
	// usage of virtual Destuctor to avoid memory leak
	virtual ~Shape()
	{
		cout << "Shape Destuctor Call\n";
	}
};

// Declaration of Derived class
class Rectangle : public Shape {
public:
	int width, height, area;

	void calculate()
	{
		cout << "Enter Width of Rectangle: ";
		cin >> width;

		cout << "Enter Height of Rectangle: ";
		cin >> height;

		area = height * width;
		cout << "Area of Rectangle: " << area << "\n";
	}

	// Virtual Destuctor for every Derived class
	virtual ~Rectangle()
	{
		cout << "Rectangle Destuctor Call\n";
	}
};

// Declaration of 2nd derived class
class Square : public Shape {
public:
	int side, area;

	void calculate()
	{
		cout << "Enter one side your of Square: ";
		cin >> side;

		area = side * side;
		cout << "Area of Square: " << area << "\n";
	}

// Virtual Destuctor for every Derived class
	virtual ~Square()
	{
		cout << "Square Destuctor Call\n";
	}
};

int main()
{
    /*
    Dynamic Allocation (new) – Heap Allocation & Polymorphism

✅ Dynamic Memory Allocation: Rectangle is created on the heap using new.
✅ Polymorphism Enabled: If Shape has a virtual destructor, calling delete myShape; ensures proper cleanup.
❗ Requires Manual delete: Forgetting delete myShape; leads to a memory leak.
✅ Allows Different Derived Types at Runtime: Can assign other derived objects dynamically.
*/
    Shape* myShape = new Rectangle();
    delete myShape;

/*
2️⃣ Stack Allocation & Pointer Assignment

✅ Stack Allocation: r is created on the stack (automatic memory management).
✅ No Manual delete Required: The destructor is called automatically when r goes out of scope.
❌ No Polymorphism if Object is Sliced: If Shape has non-virtual functions, S->someMethod(); will call Shape's version instead of Rectangle's.
❌ Cannot Reassign S to a New Derived Object: Unlike heap allocation, you cannot replace r dynamically.

/* above line mean
Shape* S;

Rectangle r;
S = &r;  // ✅ S now points to a stack-allocated Rectangle

S = new Square();  // ✅ Allowed, but now `S` points to a heap-allocated object
delete S;  // ✅ Must delete the heap-allocated object

S = &r;  // ✅ Allowed, now S points back to stack-allocated r

r = Square();  // ❌ ERROR! Cannot assign a Square object to r (fixed type)

*/
	Shape* S;  // ✅ Base class pointer (Uninitialized)

    // ✅ Stack-allocated object (No `new` used)
	Rectangle r;  // The Rectangle object r is created on the stack

	// initialization of reference variable
	S = &r;  // ✅ Assigning stack-allocated `Rectangle` to base class pointer

	// calling of Rectangle function
	S->calculate();
	Square sq;

	// initialization of reference variable
	S = &sq;

	// calling of Square function
	S->calculate();

	// return 0 to tell the program executed
	// successfully
	return 0;

/*  When to Use Which?
✅ Use new (Heap Allocation) when:

You need polymorphism and objects that can be dynamically changed at runtime.
You require an object to persist beyond function scope.
You can ensure proper memory management (delete after new).
✅ Use Stack Allocation (S = &r;) when:

You want automatic memory management (no need for delete).
You don't need to reassign the object dynamically.
Performance is critical (stack allocation is faster than heap allocation).
*/
}



    