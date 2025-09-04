#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
#include <unordered_map>

#include <typeinfo> // for typeid
#include <utility> // std::swap
#include <set>
#include <stdexcept>
#include <cstring> // strlen
#include <cstdint> // uint32_t
#include <memory>
#include <list>
#include <numeric> // iota

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

namespace std {
    template<>
    struct haxh<MyHash> {
        std::size_t operator()(const MyHash& obj)  const {
            return std::hash<int>()(obj.x) ^ std::hash<int>() (obj.y);
        }
    };
}

int main() {
    // This();
    // size_t a = 15;
    // cout << "Size of: " << sizeof(a) << "\n";
    // StringView();
    // Test_StopCopy();

    // Virtual();
    IOTA2();
    WeakPtr();
    SET();

    T_Static();

    VecPointer();
    return 0;
}