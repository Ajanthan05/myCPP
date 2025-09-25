#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include <typeinfo> // for typeid
#include <utility> // std::swap
#include <set>
#include <stdexcept>
#include <cstring> // strlen
#include <cstdint> // uint32_t
#include <memory>
#include <list>
#include <numeric> // iota
#include <limits>

using namespace std;

/*  this  pointer is used to hold the address of current object (using which we
called perticular member function)
2. this pointer is a constant pointer
3. this pointer is passed as a hidden argument to non-static member functions.
4. Compiler automatically changes all data member access with this pointer.
5. It is not passed in static member functions.
*/

class Base {
    int x;
public:
    Base() {}
    Base(int a) : x{a} {}

    void set(int a) { this->x=a; }
    int get() { return this->x; }

    // void set(Base * const this, int a) { this->x=a; }
    // int get(Base * const this) { return this->x; }
};
void This() {
    Base b1;
    Base b2(5);

    cout << b2.get() << "\n";
    // cout << Base::get(&b2) << "\n";
}
// How to check 2 object belong to same class in c++
class B {};
void TypeId() {
    Base b1;
    B b2;
    if (typeid(b1) == typeid(b2)) cout << "Equal\n";
}

/*  explicit constructor:- it avoid implicit call to the constructor

*/

class Simple {
private:
    int *data;
public:
    Simple(int value) : data(new int(value)) {
        cout << "Constructure Called. Data: " << *data << "\n";
    }
    
    Simple(const Simple& other) {
        data = new int(*other.data);
        cout << "Copy Constructure Called. Data: " << *data << "\n";
    }
    
    Simple(Simple&& other) {
        data = other.data;
        other.data = nullptr;
        cout << "Move Constructure Called. Data: " << *data << "\n";
    }

    ~Simple() {
        if (data) delete data;
        cout << "Destroyer Called\n";
    }
};
void move_Constructor() {
    Simple p(5);
    Simple q = p;

    vector<Simple> v;
    v.reserve(2);

    v.push_back(Simple(1));
    v.push_back(Simple(2));

    cout << "Resizing\n";

    v.push_back(Simple(3));

    cout << "\nEnd of Resizing\n";

    /*  
Resizing
Constructure Called. Data: 3
Move Constructure Called. Data: 3
Copy Constructure Called. Data: 1
Copy Constructure Called. Data: 2
Destroyer Called
Destroyer Called
Destroyer Called

End of Resizing

After noexcept : We are telling no way it throw an exception then its ok for 
CONTAINERS to use move constructor instead of copy constructor 


    */
}

class String {
    char *data;
public:
    // Default constructor
    String() : data(new char[1]) {
        data[0] = '\0';
    }

    // Constructor from C-string
    String(const char *str){
        data = new char[strlen(str) + 1];
        std::strcpy(data, str);
    }

    // Copy constructor (deep copy)
    String(const String& other) {
        data = new char[strlen(other.data) + 1];
        std::strcpy(data, other.data);
    }

    // Move constructor
    String(String&& other) noexcept : data(other.data) {
        other.data = nullptr;
    }

    // Copy assignment using copy and swap idiom
    String& operator=(String other) {
        std::swap(data, other.data);
        return *this;
    }

    // Move assignment
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }

    ~String() {
        delete[] data;
    }

    // String Concatenation
    String operator+(const String& other) const {
        size_t len = strlen(data) + strlen(other.data);
        char* new_data = new char[len + 1];
        strcpy(new_data, data);
        strcat(new_data, other.data);
        String result(new_data);
        delete[] new_data;
        return result;  // end of this scope result will deleted => So have to return by value
    }

    // Indexing operator (const and non-const)
    char& operator[](size_t index) {
        if (index >= strlen(data)) throw std::out_of_range("Index out of range");
        return data[index];
    }
    const char& operator[](size_t index) const {
        if (index >= strlen(data)) throw std::out_of_range("Index out of range");
        return data[index];
    }

    // Comparison operators
    bool operator==(const String& other) const {
        return strcmp(data, other.data) == 0;
    }
    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    size_t length() const {
        return strlen(data);
    }

    // Raw Pointerr access
    const char* c_str() const {
        return data;
    }

    String substr(size_t pos, size_t len) const {
        size_t strLen = strlen(data);
        if (strLen < pos) throw std::runtime_error("Position out of range");

        size_t max_len = (pos + len > strLen) ? strLen - pos : len;

        char *sub_data = new char[max_len + 1];
        strncpy(sub_data, data + pos, max_len);
        sub_data[max_len] = '\0';

        String result(sub_data);
        delete[] sub_data;
        return result;
    }

    // Output stream
    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        os << (str.data ? str.data : "");
        return os;
    }
};
void T_string() {
    String s;
    cout << s.length() << "\n";

    String a("Hello");
    String b(" Ajanthan");
    String c = a + b;

    cout << "Concatenated: " << c << "\n";

    cout << "Length: " << c.length() << "\n";
    cout << "C_String: " << c.c_str() << "\n";
    cout << "Substring (6,5): " << c.substr(6,5) << "\n";

    cout << "Char at index 0: " << c[0] << "\n";

    String d("Hellow Ajanthan");
    cout << "c == d: " << (c==d ? "True" : "False") << "\n";

    String e = std::move(c);
    cout << "Moved string: " << e << "\n";
    cout << "Original String: " << c << "\n";
}

char* my_strcat(char* dest, const char* src) {
    char* ptr = dest;

    // Move ptr to the end of dest string
    while (*ptr != '\0') {
        ptr++;
    }

    // Copy src to the end of dest
    while (*src != '\0') {
        *ptr = *src;
        ptr++;
        src++;
    }

    // Add null terminator at the end
    *ptr = '\0';

    return dest;
}


/* CONST data member of a class
They are constant co cannot be changed after initialized
1. Inatialize in class
2. Initializer list is used to initialize from outside
*/

template <typename T>
class Vector {
private:
    T *data;
    size_t _size;
    size_t _capacity;

    void resize(size_t capacity) {
        T* new_data = new T[capacity];
        for (size_t i=0; i<_size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        _capacity = capacity;
    }

public:
    Vector() : data(nullptr), _size(0), _capacity(0) {}

    ~Vector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (_size == _capacity) {
            size_t new_capacity = (_capacity == 0) ? 1 : _capacity*2;
            resize(new_capacity);
        }
        data[_size++] = value;
    }

    void pop_back() {
        if (_size > 0) --_size;
    }

    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }

    T& operator[] (size_t index) {
        if (index >= _size) throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[] (size_t index) const {
        if (index >= _size) throw std::out_of_range("Index out of range");
        return data[index];
    }
};

void Const() {
    char ch = 'A';
    char ch2 = 'B';

    const char *ptr1 = &ch; // ptr1 is pointer to a chractor constant 
    char const *ptr2 = &ch; // ptr2 is pointer to a constant chractor => both are same
    // *ptr2 = 'B'; // We cannot dereference the pointer and change 'ch'
    ptr2 = &ch2; // But we can change where it point
    // Char is constant not the pointer


    // Here pointer is constant
    char * const ptr3 = &ch;
    *ptr3 = 'N';

    const char * const ptr4 = &ch2;
}

// How delete[] how much to delete
/*  1. Over Allocation: Overallocating the original array with some extra memory and
put the size there.
    2. Associative Array: is about maintaning a separate array with pointer and value pair
*/
/* Own unique ptr quece*/

/*  CONSTRUCTOR DELEGATION
We can avoid code bloating  an repatative code
*/
class BAse {
    int _x, _y;
public:
    BAse() : BAse(0, 0) {
        // .
        // .
        // .
        // _x = some1;
        // _y = Some2;
    }
    BAse(int a) : BAse(a, 0) {
        // .
        // .
        // .
        // _x = some1;
        // _y = Some2;
    }
    BAse(int x, int y) : _x{x}, _y{y} {
        // SO we can do the hevy lifting hear only
        // .
        // .
        // .
        // _x = some1;
        // _y = Some2;
    }
};

const int n = 10;
class BASE {
public:
    int b_var;
};
void Delete() {
    BASE *bp = new BASE[n];
    /*
    char* tmp = (char*) operator new[] (WORDSIZE + n * sizeof(BASE));
    BASE* p = (BASE*) (tmp + WORDSIZE);
    *(size_t*)tnp = n;
    for (int i=0; i<n; ++i) 
        new(p + i) BASE();

    tmp:- 0 1 2 3 4 5 - 43;
    p  :- 4 - 43;
    */
   delete[] bp;
   /*
   size_t n = * (size_t*) ((char*)p - WORDSIZE);
   while(n-- != 0)
      (p + n)->~BASE();
   operator delete[] ((char*)p - WORDSIZE);
   */

//    2. ASSOCIATIVE ARRAY
    BASE *bp1 = new BASE[n];
    /*
    BASE* bp1 = (BASE*) operator new[] (n * sizeof(BASE));
    for (int i=0; i<n; ++i) 
        new(bp1 + i) BASE(); // placement new
    associationArray.insert(bp, n);
    */
    delete[] bp1;
    /*
    size_t n = associationArray.lookup(bp1);
    while(n-- != 0)
        (bp1 + n)->~BASE();
    operator delete[] (bp);
    */
}

/*  Const keyword on function overloading
1. C++ allows to overload member functions on the basis of const and non-const.
2. const parameters allows to overload member functions and normal function but 
it should be reference/pointer.*/
class Test {
    int x;
public:
    Test(int x=0) :x{x} {}
    // void print(int val) { cout << val << "\n"; }
    // void print(const int val) { cout << val << "\n"; }
    //  ERROR cannot be overloaded with ‘void Test::print(int)

    void print(int& val) { cout << val << "\n"; }  // OR can be pointer
    void print(const int& val) { cout << val << "\n"; }
}; 


// OVERLOAD TYPECAST
double dollarToRupeeExchangeRate() {
    return 81.5;
}
class Rupee {
    double rupee;
public:
    Rupee(double d=0) : rupee{d} {}
    void Print() { cout << rupee << "\n"; }
};
class Dollar {
    double dollar;
public:
    Dollar(double d=0) : dollar{d} {}
    void Print() { cout << dollar << "\n"; }

    operator Rupee () {
        return Rupee(dollar * dollarToRupeeExchangeRate());
    }
};
void OverloadTypeCast() {
    Dollar d(10);
    Rupee r;
    r = d;
    d.Print();
    r.Print();
}

class Overload {
public:
    // void* operator new(size_t size) {
    //     // cout
    // }

};

// Designated Initialization
void Designated_Initialization() {
    // int arr[10] = { [6]=10, [3] = 20 }; Work in c; for c++ all mmeber should be present in the order
}

// StringView
// uint32_t alloc = 0;
// void* operator new (size_t s) {
//     alloc++;
//     cout << "Allocated: " << s << " alloc: " << alloc << "\n";
//     return malloc(s);
// }
// void StringView() {
//     std::string str1{"Hello This is Ajanthan"};

//     std::string_view str2{ str1.substr(0, 5) };
//     std::string str3{str1};
// }

/*  How To Stop Someone From Copying Your Objects?
1. Keep copy constructor and assignment operator as private in your class.
2. Inherit dummy class with private copy constructor and assignment operator.
3. Delete copy constructor and assignment operator from your class*/
class StopCopy {
public:
    StopCopy() {}
private:
    StopCopy(const StopCopy& obj) {}
    StopCopy& operator = (const StopCopy &rhs) { return *this; }
}; 
class Base1 : public StopCopy {
    int _x;
public:
    Base1() : _x(0) {}
    Base1(int x) : _x(x) {}
};
void Test_StopCopy() {
    Base1 b1(10);
    // Base1 b2 = b1; error: use of deleted function ‘Base1::Base1(const Base1&)’
}

/*  How To Return 2D Array From Function In C & C++

*/
const int N = 3;
int** getArray() {
    int** arr = new int*[N];
    for(int i=0; i<N; ++i) {
        arr[i] = new int[N];
        for(int j=0; j<N; ++j) {
            arr[i][j] = i+j;
        }
    }
    return arr;
}

int ( *(StaricGetArray)() )[N] {
    static int arr[N][N] = { {0,1,2}, {3,4,5}, {6,7,8}};
    return arr;
}

typedef int (*DoublePointer)[N];

DoublePointer StaricGetArray2() {
    static int arr[N][N] = { {0,1,2}, {3,4,5}, {6,7,8}};
    return arr;
}

struct ArrStruct
{
    int arr[N][N];
};
ArrStruct getArray3() {
    ArrStruct var;
    return var;  // But this will return a copy. Not prefered
}

void Array() {
    int (*arr)[N];
    arr = StaricGetArray();
}

/*  VIRTUAL FUNCTION: help to achieve dynamic polymorphism. which is the ability to
call detived class function using base class pointer or reference.

1. Virftual fun cannot be static and also cannot be a friend function of another class
2. A class can have virtual destructor but can't have virtual constructor

Abstract class:- mean it have at least onek pure virtual function (fun doesnt have body in Base)
u can't do Base b1; // Object of abstract class type base is not allowed

Even derived class (who inherite Abstract Base class) doesn't have definition of pure 
virtual funstin in base are considered as abstract class

*/
class Animal {
public:
    virtual void move() = 0;
};
// Pure virtual function can have body
void Animal::move() {
    cout << "Animal Class\n";
}
class Cow: public Animal {
public:
    void move() {
        Animal::move();
        cout << "Cow walking\n";
    }
};
class SmallCow : public Cow {
private:
    void move() {
        cout << "Small cow\n";
    }
};
void Virtual() {
    Animal *cow = new Cow;
    cow->move();

    Animal *sCow = new SmallCow;
    sCow->move();
}


void IOTA() {
    std::list<int> lst(10);
    std::iota(lst.begin(), lst.end(), -5);
    for (auto elm : lst)cout << elm << " ";
}
struct C {
    int a;
  
  	// Defining ++ for struct C
    C& operator++() {
        // ++a;
        a = a+2;
        return *this;
    }
};

void IOTA2() {
    vector<C> v(5);

    // Fill the vector starting from Counter{1}
    iota(v.begin(), v.end(), C{2});

    for (auto i: v) {
        cout << i.a << " ";
    }
    cout << "\n";
}

/*  Friendship is not inherited 
Static function can access only static data member
Non static member function can access both
*/

/*  Week_pointer: are able to create a non-owning reference to the object managed by shared_ptr.
circular link:- occurs when two or more objects reference each other using a shared_ptr
It allows you to create a non-own reference to an object managed by shared_ptr without affecting the reference count or preventing the object from being deleted.

Cache Systems: weak_ptr is commonly used in cache implementations. Caches often need to temporarily store references to objects without preventing the deletion of those objects when they are no longer in use. weak_ptr provides an elegant solution for this use case.

reset()
swap()
expired()     	Check if the resource weak_ptr pointing to exists or not.
lock()	If the resource pointed by weak_ptr exists, this function returns a shared_ptr with ownership of that resource. If the resource does not exist, it returns default constructed shared_ptr.
use_count()	Tells about how many shared_ptr owns the resource.

A std::weak_ptr is non-owning, meaning:
    It doesn’t keep the object alive.
    It can’t be dereferenced directly (because the object might already be destroyed).
*/
struct BB; // forward declaration

struct AA {
    shared_ptr<BB> b_ptr;  // strong ownership
    ~AA() { cout << "A destroyed\n"; }
};

struct BB {
    weak_ptr<AA> a_ptr;    // weak ownership — breaks the cycle
    ~BB() { cout << "B destroyed\n"; }
};

void WeakPtr() {
    auto a = make_shared<AA>();
    auto b = make_shared<BB>();

    a->b_ptr = b;
    b->a_ptr = a;  // weak_ptr doesn't increase ref count

    // End of main: both destructors called, no leak

    weak_ptr<AA> wptr;

    if (auto sptr = wptr.lock()) {
        // ✅ Object is still alive, safe to use
        // sptr->doSomething();
    } else {
        // ❌ Object already destroyed
        cout << "Object expired\n";
    }
}

struct Data {
    string value;
    Data(string v) : value(v) { cout << "Data created: " << v << "\n"; }
    ~Data() { cout << "Data destroyed: " << value << "\n"; }
};

class Cache {
    unordered_map<string, weak_ptr<Data>> cache;
public:
    void store(const string& key, shared_ptr<Data> data) {
        cache[key] = data;  // store weak_ptr
    }

    shared_ptr<Data> get(const string& key) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            shared_ptr<Data> sp = it->second.lock();
            if (sp) {
                cout << "Cache hit: " << key << "\n";
                return sp;
            } else {
                cout << "Cache expired: " << key << "\n";
            }
        }
        return nullptr;
    }
};

void TCash() {
    Cache c;

    {
        auto d = make_shared<Data>("object1");
        c.store("obj1", d);

        // Access while alive
        auto p = c.get("obj1");
    } // `d` goes out of scope here — object destroyed

    // Try to access after destruction
    auto p = c.get("obj1");
}

/*  void is used to denote nothing
void* is universal pointer
we can convert any data type pointer to void* (except function pointer, const or volatile)
void * canot be dereferenced
*/

class S {
public:
    int x;
    S(int  x=0) : x{x} { cout << "Construct\n"; }
    S(const S& rhs) { x = rhs.x; cout << "Copy\n"; }
};
bool operator< (const S& lhs, const S& rhs) { return lhs.x < rhs.x; }
void SET() {
    set<S> s;
    s.insert(S(10));
}

/*  STATIC IN TEMPLATE */
template <typename T, int SIZE>
class Container {
public:
    Container() : m_data(new T[SIZE]) {}
    ~Container() {
        delete[] m_data;
    }

    static T m_variable;
private:
    T* m_data;
};

// Tell compiler to generate thid variable for each instantination of my class
template<typename T, int SIZE> T Container<T, SIZE>::m_variable;

void T_Static() {
    Container<int, 5> c1;
    Container<int, 5>::m_variable = 7; // undefined reference to `Container<int, 5>::m_variable'\

}



struct Foo {
    int x;
    Foo(int v) : x(v) {}
};
void VecPointer() {
    std::vector<std::unique_ptr<Foo>> v;

    v.push_back(std::make_unique<Foo>(1));
    v.push_back(std::make_unique<Foo>(2));

    for (auto& p : v)
        std::cout << p->x << "\n";
    // cleanup automatic!

    // std::vector<std::unique_ptr<Foo>> copy = v;
}

/* Why lamda function
where
*/

struct MyHash {
    int x;
    int y;
};

// namespace std {
//     template<>
//     struct haxh<MyHash> {
//         std::size_t operator()(const MyHash& obj)  const {
//             return std::hash<int>()(obj.x) ^ std::hash<int>() (obj.y);
//         }
//     };
// }


// int ReverseNum(int i) {
//     int ans = 0;
//     int sign = 1;
//     if (i<0) {
//         sign = -1;
//         i *= (-1);
//     }

//     while(i) {
//         int rem = i%10;
//         ans = ans*10 + rem;
//         i /= 10;
//     }

//     return sign*ans;
// }

double ReverseNum(double num) {
    // Handle sign
    int sign = (num < 0) ? -1 : 1;
    num = std::abs(num);

    // Remove trailing zeros and possible trailing dot
    std::string s = std::to_string(num);
    s.erase(s.find_last_not_of('0') + 1, std::string::npos);
    if(s.back() == '.') s.pop_back();
/*  s.find_last_not_of('0')

Finds the last character in s that is NOT '0'.

In "123.45000", the last non-zero character is '5' at index 5.

So this returns 5.

+ 1

Moves one position to the right (to index 6).

That’s the position of the first '0' after the last non-zero digit.

s.erase(6, std::string::npos)

Erases everything from index 6 until the end of the string.

std::string::npos just means “go until the end”.

So it erases "000", leaving "123.45".*/

    // Reverse digits ignoring the decimal point
    std::string rev;
    for(char c : s) {
        if (c != '.')
            rev.push_back(c);
    }
    std::reverse(rev.begin(), rev.end());

    // Put decimal point back at the right place
    size_t dotPos = s.find('.');
    if (dotPos != std::string::npos) {
        size_t digits_after_dot = s.size() - dotPos - 1;
        rev.insert(rev.begin() + digits_after_dot, '.');
    }
    return sign * std::stod(rev);


    /*  std::string::npos?
It is a constant defined as the maximum possible value of size_t:
static const size_t npos = -1;
So for a 64-bit system, it’s 18446744073709551615.

size_t dot_pos = s.find('.');
If '.' exists in the string, dot_pos will be the index (position) of the first '.'.
Example: "123.456" → dot_pos = 3.
If '.' does not exist, then find() returns std::string::npos.*/
}
void T_ReverseNum() {
    std::cout << ReverseNum(123.456) << "\n";   // 654.321
    std::cout << ReverseNum(-10.25)   << "\n";  // -52.01
}

/*  The function std::numeric_limits<T>::digits does not accept any parameter. 
Return Value: The function std::numeric_limits<T>::digits returns the number of 
radix digits that the type can represent without loss of precision. Below is the 
program to demonstrate std::numeric_limits<T>::digits in C++: Program*/
void NumericLimits() {
    cout << "For int: "
         << numeric_limits<int>::digits
         << endl;

    cout << "For float: "
         << numeric_limits<float>::digits
         << endl;

    cout << "For double: "
         << numeric_limits<double>::digits
         << endl;

    cout << "For long double: "
         << numeric_limits<long double>::digits
         << endl;
}


struct Outer {
    int id;

    struct Inner {
        int x;
        int y;
    };
};
void T_inner() {
    Outer::Inner p;   // standalone object of type Outer::Inner
    p.x = 10;
    p.y = 20;
    // This is not tied to any specific Outer instance.
    // It’s just a regular object of type Outer::Inner.
}
/*  Summary

✅ Standalone inner object = using Outer::Inner outside Outer.
✅ Needed when:

You want reusability of inner type.

You want hierarchical naming (self-documenting).

You want to avoid name clashes.

You want to organize related concepts but still use them independently.

It’s very common in the STL (std::vector::iterator, std::map::value_type, etc.) and in real-world design (geometry, networking, configs).*/
struct Person {
    std::string name;
    int age;

    struct Address {
        std::string city;
        std::string street;
        int zipcode;
    };

    Address addr;
};

void T_inner2() {
    Person p{"Alice", 30, {"New York", "5th Avenue", 12345}};
    std::cout << p.name << " lives in " << p.addr.city << "\n";
}

void T_Unordered_set() {
    unordered_set<int> us = {1, 2, 3, 4, 5};
    
    // Using iterator in loop
    for(auto it = us.begin(); it != us.end(); it++)
        cout << *it << " ";

    us.erase(5);
    us.erase(us.begin());

/*  Operation	            Time Complexity
Insert an element	        O(1) (average)
Delete an element	        O(1) (average)
Access element by position  O(n)
Find element by value	    O(1) (average)
Traverse the set	        O(n)            

max_size()

Returns maximum number of elements that an unordered set can hold.
size()

*/
}

struct A {
    int a;
    int c;
    int e;
    int d;
    int f;
    int m;
};

struct Q {
    int b;
    int l;
    A a; // size: 12
    // A *aptr;  // pointer to A
    // std::unique_ptr<A> aptr;
};
/*
struct A {
    int a;
    int c;
};

struct Q {
    int b;
    A a;                    // embedded struct
    std::unique_ptr<A> aptr; // smart pointer
};
struct Q (24 bytes total):

Offset 0–3   : int b
Offset 4–11  : struct A (a)
   - A.a = 4–7
   - A.c = 8–11
Offset 12–15 : padding (for alignment)
Offset 16–23 : std::unique_ptr<A> aptr (8 bytes)*/

struct alignas(64) Node {
    int x;
    int y;
};
void T3() {
    Q obj;
    cout << "Size: " << sizeof(obj) << "\n";

    Node n;
    cout << "Size: " << sizeof(n) << "\n";
    // cout << "Size: " << sizeof(obj) << "\n";
}


int main() {
    // This();
    // size_t a = 15;
    // cout << "Size of: " << sizeof(a) << "\n";
    // StringView();
    // Test_StopCopy();

    // Virtual();

    // IOTA2();
    // WeakPtr();
    // SET();

    // T_Static();

    // VecPointer();

    T_ReverseNum();
    T3();
    return 0;
}