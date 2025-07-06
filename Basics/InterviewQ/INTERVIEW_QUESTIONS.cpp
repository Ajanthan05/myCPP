/*  Constructor Delegation */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <climits>
#include <list>
#include <unordered_set>
#include <set>
#include <typeinfo> // for typeid

using namespace std;

void T() {

    // Q: What is declaration in below code?

    extern int p;                                   // declaration
    extern float myMfuction(float x);               // declaration
    double myFunction2 (double, double);            // declaration
    // extern double myFunction3 (double, double) {};     // Not a declaration boc it has {}
}

// decltype In C++11
// template<typename T1, typename T2>
// T2 add(T1 a, T2 b) {
//     return a+b;
// }
/*  ANS Before
2.8
2
*/
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a+b) {
    return a+b;
}
void T_DECLTYPE() {
    cout << add(1, 1.8) << "\n";
    cout << add(1.8, 1) << "\n";
}

/* CPP ue mame mangling to achie function overloading */
extern "C"{
    // #include "cfile.h"
}

class BasE {};
void Auto() {
    auto x = 20;
    auto y = 20.5;
    auto b = new BasE();

    cout << typeid(x).name() << "\n";
    cout << typeid(y).name() << "\n";
    cout << typeid(b).name() << "\n";
}

class Mull {
    int _val;
public:
    Mull() {}
    Mull(int _val) : _val{_val} {}

    int operator () (int val) {
        return val*_val;
    }
};

void Functor() {
    Mull mull5(5);
    cout << mull5(6) << "\n";
}

// Why we should return Base reference from copy assignment operator
class Base{
    int _var;
public:
    virtual void fun() { cout << "Base fun\n"; }
    Base()  { cout << "Base Constructor\n"; }
    virtual ~Base() { cout << "Base Distructor\n"; }
    Base(int _var) : _var{_var} {} 

    Base& operator = (const Base& rhs) {
        _var = rhs._var;
        return *this;
    }
    void print() {
        cout << _var << "\n";
    }
};

class Derive : public Base{
    int _var_d;
public:
    virtual void fun() { cout << "Derive fun\n"; }
    Derive()  { cout << "Derive Constructor\n"; }
    ~Derive() { cout << "Derive Distructor\n"; }
    Derive(int _var) : _var_d{_var} {} 

    Derive& operator = (const Derive& rhs) {
        _var_d = rhs._var_d;
        return *this;
    }
    void print() {
        cout << _var_d << "\n";
    }
};
void T2() {
    cout << "\n";
    Base *B1 = new Base();
    Base *B2 = new Derive();

    B1->fun();
    B2->fun();

    delete B1;
    delete B2;
    /*  You are not copying a Derive object into a Base object.
You're just creating a Derive object and assigning its pointer to a Base*.
This is polymorphism, not slicing.

This would cause slicing:
Base B = Derive(42);  // ⚠️ Slicing occurs here
Derive has extra members (_var_d) and overrides.

When assigned to Base, only the Base part is copied.

Derive-specific parts are sliced off.*/
    cout << "\n";

    int a=10, b, c=2;
    (b=c) = a;
    cout << "B: " << b << "\n";
    cout << "C: " << c << "\n";
    // B: 10
    // C: 2

    Base b1(10);
    Base b2, b3, b4;

    b2 = b3 = b4 = b1;
}

// Return Multiple Values From Functions
std::tuple<int, char, std::string> fun(bool flg) {
    if(flg) 
        return std::make_tuple(1, 'x', "Ajanthan");
    else
        return std::make_tuple(1, 'y', ":Thar");
}
void ReturnMultiple() {
    int num; char code; string name;

    tie(num, code, name) = fun(true);

    // OR without tie
    auto v = fun(true);

    num  = std::get<0>(v);
    code = std::get<1>(v);
    name = std::get<2>(v);
}


// Stop Someone To Inherit From Your Class
class FinalClass final {};  // If some class tiy to derive // error cannot derive from final base

// Object Slicing 


// Why Size Of Empty class OR struct Is One In C++
    // compiler need atleast 1 byte to differenciate frome one to another
    // Base b1, b2;  address should not be equal =>>> &b1 != &b2

void T_Memory() {
    int *intPtr1 = (int*)malloc(5*sizeof(int));
    memset(intPtr1, 0, 5 * sizeof(int));

    int *intPtr2 = (int*)calloc(5, sizeof(int));  // zero-initializes 20 bytes

    free(intPtr1);
    free(intPtr2);
}

class Int {
    int elem;
public:
    Int() {}
    Int(int val) : elem{val} {}
    void printVal() { cout << elem << "\n"; }

    Int& operator++() {                 // pre Increment    ++i
        elem++;
        return *this;
    } 

    Int operator++(int dummy) {         // Post Increment   i++
        Int tmp = *this;
        ++(*this);
        return tmp;
    }

    /*  Why not return Int&?
    tmp is a local object.
Returning a reference to it is undefined behavior (lifetime ends after function returns).
Even if you returned *this, that would represent the modified object — which violates the expected behavior of post-increment.*/
};

void T_Int() {
    Int i{10};
    (i++).printVal();
    (++i).printVal();
    i.printVal();
}


// How To Print Bingo N Time Without loop & recursion In C++?
class Print {
public:
    Print() {  cout << "Ajanthan\n"; }
};
void PrintN() {
    int N = 10;
    Print p[N];
}

// Changing n'th bit to bitVal
void Bit() {
    int number = 3;
    int n = 2;
    int bitVal = 1;

    number = number & ~(1 << n) | (bitVal << n);
    cout << number << "\n";
}


// class is user define datatype and Object is an ainstance of class
// Premitive data type this is Integer

/* DATA ENCAPULATION
It is an object oriented programming concept that binds data with function*/

/*  Shallow Copy And Deep Copy In C++   */

/*  Segmentation fault occurs because of memory access violations
stack overflow
write violations
read violations
etc but all are related ti memory error*/

// How delete know how meny objects to delete?
// 1. Over Allocation,      2. Associative array

// MEMORY ADDRESS
void mem() {
    int i = 10;
    int &r = i; // REFERENCE
    // i and r are refering same mem address r is alias  of i
/*  r is not a separate variable with its own memory—it is a reference to i.

That means:
r does not occupy new storage for the value.
r is just another name or alias for i.*/

// 2.   REASSIGNMENT IS NOT POSSIBLE WITH REFERENCE
    int var = 90;
    r = var;
    cout << "i is changed: " << i << "\n";
    // Reference is type bound u cannot brack it

    // 3. NULL VALUE
    int *p = nullptr; // U can nale null pointer and assign it later
    // int &ref; // cannot make null reference ro empty reference

    // 4. ARITHMETIC OPERATIONS are not possible for reference
    // p++ pointer we can increment next memory location
    // r++ not possible
    // (&r)++ we can increment the address of reference

    // 4. N om of INDIRECTION IS NOT GIVEN IN POINTERS
    int **ptr = &p;
    int ***ptr1 = &ptr;

    int &r1 = r; // 1 level of inderection
}

int AToI(char *str) {
    int sign = 1;
    int ans = 0;
    int i = 0;

    if(str[i] == '-') {
        sign = -1;
        i++;
    }

    for(; str[i]!='\0'; ++i) {
        ans = ans*10 + str[i] - '0';
    }

    return ans*sign;
}
void T_AToI() {
    char str[] = "-123";
    cout << "AToI: " << AToI(str) << "\n";
}

// To Stop Someone From Taking Addess Of Your Object
// 1. Overloade & operator ans keep it private
// 2. delete & operator from the class
class BaseAddr {
public:
    BaseAddr() {}
    BaseAddr(int x) : x{x} {}
    // or
    // BaseAddr* operator & () = delete;
private:
    BaseAddr* operator & () {
        cout << "Addr Operator\n";
        return this;
    }

private:
    int x;
};
void T_BaseAddr() {
    BaseAddr b;
    // BaseAddr *bp = &b;
    // cout << bp << "\n";
    // cout << &b << "\n";
}

/*  How To Return An Array From Function
Dont return pointers of local or reference of local
*/
int *funcHeap() {
    // int* arr = new int[3];
    // arr[0] = 1;
    // arr[1] = 2;
    // arr[2] = 3;

    int* arr = new int[3]{1, 2, 3};  // cleaner initialization
    return arr;
}
int* funcStatic() {
    static int arr[3] = {1,2,3};
    // unsafe if f() is called recursively
    return arr;
}

// Prefer std::array or std::vector in modern C++
std::vector<int> funcVec() {
    return {1, 2, 3};  // Return-by-value with move semantics
}

void Return_Array_From_Function() {
    int* arr = funcHeap();
    cout << arr[2] << "\n";
    delete[] arr;  // Must manually delete heap alocated memory

    int *arrS = funcStatic();
    cout << arrS[0] << "\n";
    cout << arrS[1] << "\n";
    cout << arrS[2] << "\n";

    // 1. Shared State / Not Thread-Safe
// Problem: All callers share the same array instance.
    int* arr2 = funcStatic();
    arr[0] = 42;  // This also changes arr2[0]

// 2. Not Reentrant
// Problem: Function can't safely be called recursively or from multiple places at once.

// 3. Unexpected Lifetime  Problem: The array lives forever (until program ends), which can waste memory.
}


/*  How To Return 2D Array From Function */
int ** get2DArray(int N, int M) {
    int** arr = new int*[N];     // Allocate array of pointers
    for(int i=0; i<N; ++i) {
        arr[i] = new int[M];    // Allocate array of ints per row
        for(int j=0; j<M; ++j) {
            arr[i][j] = i+j;
        }
    }
    return arr;
}
// Using Pointer of arrays
const int L = 3;
int (*(getArray)())[L] {
    static int arr[L][L] = {{1,2,3},{4,5,6},{7,8,9}};
    return arr;
}
// OR we can define the type Double Pointer of array
// typedef (*DoublePointer)[L]
// DoublePointer getArrayStatic() {
//     static int arr[L][L] = {{1,2,3},{4,5,6},{7,8,9}};
//     return arr;
// }

void Return_2D_Array_From_Function() {
    int **arr = get2DArray(2,3);
    int N = 2; // row
    for (int i = 0; i < N; ++i) {
        delete[] arr[i];  // delete each row
    }
    delete[] arr;  // delete the array of row pointers


}


/*  Q Passing Two Dimensional Array In Function In C++
1. keep row and column size as global
2. Pass column size in array*/
const int c = 2;
void funArr(int arr[][c], int r) {
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) {
            cout << arr[i][j] << " ";
        }
        cout << "\n";
    }
}
// Thpe cast to Integer pointer
void funArr2(int *arr, int r, int col) {
    for(int i=0; i<r; i++) {
        for(int j=0; j<col; j++) {
            cout << *(arr + i*col + j) << " ";
        }
        cout << "\n";
    }
}
void TwoD_Arr() {
    int arr[2][2] = { {1,2},
                      {3,4}};
    funArr(arr, 2);

    // Thpe cast to Integer pointer
    funArr2((int*)arr, 2, 2);
}

/*  std::array<T, N>
✅ Fixed Size — Stack-Allocated
A wrapper around raw array.
Memory is part of the object itself — no dynamic allocation.

Memory Layout:
template<typename T, size_t N>
struct array {
    T elems[N]; // in-place storage
};

| Feature          | Details                    |
| ---------------- | -------------------------- |
| Memory location  | Stack or inline (in-place) |
| Resizable        | ❌ No                       |
| Manual deletion  | ❌ No (RAII)                |
| Very lightweight | ✅ Yes                      |


| Container            | Memory Location | Resizable | Allocator-Based | Fast Access | Notes                              |
| -------------------- | --------------- | --------- | --------------- | ----------- | ---------------------------------- |
| `std::vector`        | Heap            | ✅         | ✅               | ✅ random    | Best general-purpose dynamic array |
| `std::array`         | Stack/in-place  | ❌         | ❌               | ✅ random    | No overhead, fixed-size            |
| `std::list`          | Heap (nodes)    | ✅         | ✅               | ❌           | Slow random access, stable refs    |
| `std::deque`         | Heap (blocks)   | ✅         | ✅               | ✅           | Fast push/pop at both ends         |
| `std::map`           | Heap (nodes)    | ✅         | ✅               | ❌           | Sorted keys                        |
| `std::unordered_map` | Heap            | ✅         | ✅               | ✅ average   | Faster lookup, no order            |

std::list, std::deque, std::map, std::unordered_map
✅ Internally Use Heap
- std::list<T>:
Doubly-linked list.

Each node (T + 2 pointers) is dynamically allocated.

- std::deque<T>:
Array of blocks.

Fast push/pop at both ends.

More complex than vector, but still uses heap for blocks.

- std::map / std::set:
Self-balancing binary trees (e.g., Red-Black Tree).

Each node is heap-allocated.

- std::unordered_map / unordered_set:
Hash table + linked list or open addressing.

Buckets and entries are heap-allocated.

comparison in terms of performance
*/

/*  Neither std::array nor C-style arrays allocate memory on the heap unless explicitly wrapped in new.
🔥 Now, Heap Allocation — Only Happens If You Explicitly Use new
🔹 C-style array on heap:
int* arr = new int[3];  // Heap allocation

🔹 std::array on heap:
auto* arr = new std::array<int, 3>;  // Heap allocation of std::array
These must be manually delete[]d or deleted.

Why Important?
Stack = fast, automatic, limited size (~1 MB typically)
Heap = slower, manual, huge size, you control lifetime

If you don’t use new, you're working on the stack or as globals.

By default, both std::array and C-style arrays:
    1. Do not use new or malloc.
    2. Their memory is stored inline in:
        the stack (for local variables),
        or the data segment (for global/static variables).


*/


// Difference Between An Array And Address of the Array
void Q1() {
    int arr[5];

    cout << "arr  :" << arr  << "arr+1  :" << arr + 1  << "\n"; 
    cout << "&arr :" << &arr << "arr+1  :" << &arr + 1 << "\n"; 
    // &arr is integer pointer pointing to 5 array size
}

// Function Hiding
class BaseFH {
public:
    int fun(int i) {
        cout << "BaseFH\n";
        return 0;  // ✅ Always return if return type is non-void
    }
};
class DerivedFH : public BaseFH {
public:
    using BaseFH::fun; // Bring all BaseFH::fun into scope
    int fun(char c) {
        cout << "DerivedFH\n";
        return 0;  // ✅ Always return if return type is non-void
    }
};
void T_FH() {
    DerivedFH d;
//     d.fun(1);
//     d.fun('a');
// DerivedFH
// DerivedFH

//     d.BaseFH::fun(1);
//     d.fun('a');
// Now ans
// BaseFH
// DerivedFH

    // How to Fix the Function Hiding? Use a using declaration in the derived class
    d.fun(1);
    d.fun('a');
}


int FindStr(string s, string x) {
    int n = s.length();
    int l = x.length();
    int count = 0;
    int xl = 0;
    for(int i=0; i<n; ++i) {
        if(s[i] == x[xl++]) {
            count++;
            if(count==l) return i - l + 1;
        }
        else {
            count=0;
            xl=0;
        }
    }
    return -1;
}
void T_FindStr() {
    string s = "Hi this is";
    string x = "th";
    cout << "FindStr: " << FindStr(s, x) << "\n";
}


/*  Function Overloading
u cannot overload with return type

RULES FUNCTION OVERLOAD WILL NOT WORK
1. Fun declarations that deffer inly in the return thoe
2. In class if we have similar functions names and parameter but one is static function aanother one is non static
3. When we receive array as pointer or array both are similar
4. Const and volatile doesn't make any difference.
5. 

int add(int ()) {  // Take function
    cout << "First\n";
}
int add(int (*)()) {    // Take pointer of function
    cout << "First\n";
}
    Both are similar



int add(int p ) {  
    cout << "First\n";
}
int add(int p = 10) {    // with default
    cout << "First\n";
}
    Both are similar
*/

void Vec() {
    std::vector<int> vec(10);  // 10 elements

    std::cout << "sizeof(vec): " << sizeof(vec) << " bytes\n";
    std::cout << "size(): " << vec.size() << "\n";
    std::cout << "capacity(): " << vec.capacity() << "\n";
    std::cout << "Pointer: " << static_cast<const void*>(vec.data()) << "\n";
    std::cout << "Pointer: " << (vec.data()) << "\n";

    std::cout << "Heap memory used: " << vec.capacity() * sizeof(int) << " bytes\n";

    /*  Pointer to internal array

    int* ptr = vec.data(); // or &vec[0] if not empty

    This gives you the pointer to the underlying buffer (on heap).
    nullptr if vec is empty.*/
}
class Tamil {
// private:
public:
    int val;
};

class A : virtual public Tamil {
public:
    void printMessage() {
        cout << "Class A\n";
    }
    A() {
        val=1;
    }
    void getValue() {
        cout << "In A value is: " << val << "\n";
    }
};
class B: virtual public Tamil {
public:
    void printMessage() {
        cout << "Class B\n";
    }
    B() {
        val=2;
    }
    void getValue() {
        cout << "In B value is: " << val << "\n";
    }
};
class AB : public A, private B {
public:
    // Overiding
    // void printMessage() {
    //     cout << "Class AB\n";
    // }
    // Output: Class AB
// After coment it ===>>>   request for member ‘printMessage’ is ambiguous  obj.printMessage();
    void printMessage() {
        A::printMessage();
        B::printMessage();
        cout << "Class AB\n";
    }

};
void Multiple_Inheritance_Ambiguity() {
    AB obj;
    obj.printMessage();
}

class Eng : public B, public A { // After virtual inheritance only right most class it inherited
public:
    void getValue() {
        cout << "Int value is: ";
        B::getValue();
        cout << "\n";
    }
};
void Hybrid_Inheritance_With_Dimond_Problem() {
    Eng obj;
    obj.getValue();
}

double median(vector<int>& a, vector<int>& b) {
    int n1 = a.size();
    int n2 = b.size();
    if (n1 > n2) return median(b, a);


    int n = n1 + n2;
    int left = (n + 1) >> 1;
    int low = 0, high = n1;

    while(low <= high) {
        int mid1 = (low + high) >> 1;
        int mid2 = left - mid1;

        int l1 = INT_MIN, r1 = INT_MAX;
        int l2 = INT_MIN, r2 = INT_MAX;

        if (mid1 < n1) r1 = a[mid1];
        if (mid2 < n2) r2 = b[mid2];
        if (mid1 - 1 >= 0) l1 = a[mid1-1];
        if (mid2 - 1 >= 0) l2 = b[mid2-1];

        if (l1 <= r2 && l2 <= r1) {
            if (n%2 == 1) return max(l1, l2);
            else return (double)( max(l1, l2) + min(r1, r2) ) / 2.0; 
        }
        else if (l1 > r2) high = mid1 - 1;
        else              low  = mid1 + 1;
    }
    return 0;
}

int KthElement(vector<int>& a, vector<int>& b, int k) {
    int n1 = a.size();
    int n2 = b.size();
    if (n1 > n2) return KthElement(b, a, k);


    int n = n1 + n2;
    int left = k;
    // int low = 0, high = n1;
    int low = max(0, k-n2), high = min(k, n1);

    while(low <= high) {
        int mid1 = (low + high) >> 1;
        int mid2 = left - mid1;

        int l1 = INT_MIN, r1 = INT_MAX;
        int l2 = INT_MIN, r2 = INT_MAX;

        if (mid1 < n1) r1 = a[mid1];
        if (mid2 < n2) r2 = b[mid2];
        if (mid1 - 1 >= 0) l1 = a[mid1-1];
        if (mid2 - 1 >= 0) l2 = b[mid2-1];

        if (l1 <= r2 && l2 <= r1) {
            return max(l1, l2);
        }
        else if (l1 > r2) high = mid1 - 1;
        else              low  = mid1 + 1;
    }
    return 0;
}

int lowerBound(vector<int> arr, int n, int x) {
    int low = 0, high = n-1;
    int ans = n;
    while (low < high) {
        int mid = (low + high) >> 1;
        // May be an answer
        if (arr[mid] >= x) {
            ans = mid;
            // look for more small index on left
            high = mid-1;
        }
        else {
            low = mid + 1; // look for the right
        }
    }
    return ans;
}
int rowWithMax1s(vector<vector<int>>& mat, int n, int m) {
    int cnt_max=0;
    int index = -1;
    for (int i=0; i<n; i++) {
        int cnt_onces = m - lowerBound(mat[i], m, 1);
        if (cnt_onces > cnt_max) {
            cnt_max = cnt_onces;
            index = i;
        }
    }
    return index;
}


////////////////////////    MEMORY      /////////////////////////////////////////////////
/*  🔹 1. Stack Memory
Used for local variables, function parameters, return addresses, etc.
Managed automatically (allocated/deallocated when functions are called/return).
Fast access, but limited in size.

✅ Advantages:
Very fast (LIFO – Last In First Out).

No memory leaks (automatically cleaned up).

❌ Disadvantages:
Small in size (typically a few MBs).

Lifetime tied to function call.


🔹 2. Heap Memory
📌 Characteristics:
Used for dynamic memory allocation (malloc, calloc, realloc).

You control allocation and deallocation.

Larger than stack, but slower.

✅ Advantages:
Flexible size.

Lives until explicitly freed.

❌ Disadvantages:
Must manage manually (or causes memory leaks).

Slower access than stack.

🔹 3. Data Segment (Static/Global Memory)
Split into two parts:

🔸 a. Initialized Data Segment
1. Stores global/static variables that are initialized.
2. Lifetime: entire program duration.
    int global_var = 5;  // Stored here

🔸 b. Uninitialized Data Segment (BSS)
1. Stores global/static variables that are not initialized.
2. Automatically initialized to zero by the OS.
    static int counter;  // Stored in BSS (zero-initialized)

🔹 4. Code/Text Segment
1. Stores the compiled program instructions (binary code).
2. Usually read-only, preventing accidental modification.

🔹 5. Memory-Mapped Segment
📌 Characteristics:
Used to map files or devices into memory.

Used in OS-level programming, device drivers, or shared memory (IPC).

+-------------------------+
|     Command-Line Args   |
+-------------------------+
|     Environment Vars    |
+-------------------------+
|     Stack (grows down)  |
+-------------------------+
|     Unused (Gap)        |
+-------------------------+
|     Heap (grows up)     |
+-------------------------+
|     BSS (uninit global) |
+-------------------------+
|     Data (init global)  |
+-------------------------+
|     Text/Code Segment   |
+-------------------------+

| Situation                             | Use Memory Type |
| ------------------------------------- | --------------- |
| Temporary variable in a function      | Stack           |
| Data shared across functions/modules  | Global/Data     |
| Variable-size or user-controlled data | Heap            |
| Constants or compiled code            | Text segment    |

*/

/*  🔸 malloc() – Memory Allocation
    void* malloc(size_t size);
1. Allocates size bytes of uninitialized memory.
2. Returns a void* (generic pointer), which should be cast to the 
required type (e.g., int*, char*, etc.).
3. If allocation fails, returns NULL.*/
int *arr = (int*)malloc(5 * sizeof(int)); // allocates space for 5 integers
// ⚠️ Contents are uninitialized and may contain garbage values.

/*  🔸 calloc() – Contiguous Allocation
    void* calloc(size_t num, size_t size);
1. Allocates memory for an array of num elements, each of size bytes.
2. Initializes all bytes to zero.
3. Returns void*, or NULL if it fails.
int *arr = (int*)calloc(5, sizeof(int)); // allocates and zero-initializes space for 5 integers
✅ Preferred when you want zero-initialized memory. */

/*  🔸 realloc() – Reallocation
Prototype:

c
Copy
Edit
void* realloc(void* ptr, size_t new_size);
Resizes memory block pointed to by ptr to new_size bytes.

Can increase or decrease the memory size:

If increasing: old data is preserved, new memory is uninitialized.

If decreasing: extra data may be lost.

May return a new address; always assign to a new pointer to avoid memory leaks.
int *arr = (int*)malloc(5 * sizeof(int));

arr = (int*)realloc(arr, 10 * sizeof(int)); // expands to 10 integers

⚠️ If realloc() fails, original pointer is unchanged — that's why it’s safer to use:
int *temp = realloc(arr, new_size);
if (temp != NULL) {
    arr = temp;
}

🔹 Case 1: Enough space at current location
1. If the next memory block is free, the system extends the memory in place.
2. Old data remains unchanged.
3. The extra memory is not initialized — it contains garbage values.

🔹 Case 2: Not enough space at current location
1.  Allocates a new larger block somewhere else.
2. Copies the old data to the new block.
3. Frees the old memory block.
4. Returns a pointer to the new block.

🔸 Best Practices
✅ Always check if the return value is NULL before using it.
✅ Always free() any allocated memory once done.
✅ Avoid memory leaks and dangling pointers.
✅ Prefer calloc over malloc + memset() when you need zeroed memory.*/


/*  🔹 1. memcpy() — Memory Copy
    void *memcpy(void *dest, const void *src, size_t n);
1. Copies n bytes from memory area src to dest.
2. Assumes non-overlapping memory regions.
3. Very fast, but undefined behavior if src and dest overlap.



*/

void mem_test() {
    char src[] = "Hello";
    char dest[10];
    memcpy(dest, src, 6); // Include '\0'


    int arr[10];
    // void *memset(void *ptr, int value, size_t num);
    memset(arr, -1, sizeof(arr));  // set all bytes to 0
    for (auto val : arr) {
        cout << val << " ";
    }
    cout << "\n";
    /*void *memset(void *ptr, int value, size_t num);
expects an int, but it actually sets each byte of the memory block to the lower 8 bits of that value.

This is because memory is written byte by byte, and only 1 byte (8 bits) fits in a char.
int arr[4];
memset(arr, 1, sizeof(arr));
This sets every byte to 0x01:
0x01010101 = 16843009  (not 1!)*/

    // ❌ What if we do:
    float f[4];
    memset(f, -1, sizeof(f)); // That fills every byte with 0xFF.
    // 0xFFFFFFFF = NaN (Not a Number), or undefined float

    // ❌ Example with Pointers:
    void *ptrs[4];
    memset(ptrs, 0, sizeof(ptrs));  // This is safe: null pointers
    memset(ptrs, -1, sizeof(ptrs)); // Unsafe: sets all bytes to 0xFF
    // (void*)0xFFFFFFFFFFFFFFFF is likely invalid and unsafe on most systems.
}

/*  🔹 2. memset() — Memory Set / Initialize
    void *memset(void *ptr, int value, size_t num);
1. Fills the first num bytes of memory pointed to by ptr with the byte value value.
2. Often used to initialize memory to 0, -1, or a fixed byte.

⚠️ Note:
value is converted to an unsigned char (i.e., only the lower 8 bits are used).

int arr[10];
memset(arr, 0, sizeof(arr));  // set all bytes to 0
🔎 This zeroes the array, but beware when using with non-char types — setting 
all bits to zero may not always mean logical zero (e.g., for floating-point).*/

/*  🔹 3. memmove() — Safe Memory Move
    void *memmove(void *dest, const void *src, size_t n);
1. Copies n bytes from src to dest, safely handles overlapping regions.
2. Internally detects overlap and copies accordingly (forward or backward).
3. Slightly slower than memcpy, but safer.*/


void T_memmove(){
    char str[] = "ABCDEFG";
    memmove(str + 2, str, 5);  // overlapping copy
    // Result: str becomes "ABABCDE"
}

/*  🔹 4. memcmp() — Memory Compare
    int memcmp(const void *ptr1, const void *ptr2, size_t num);
Compares num bytes of ptr1 and ptr2.

Returns:
0 if equal,
< 0 if ptr1[i] < ptr2[i],
> 0 if ptr1[i] > ptr2[i]

*/
void Memcmp() {
    char a[] = "XYZ";
    char b[] = "ACB";
    cout << "memcmp: " << memcmp(a, b, 3) << "\n";
}



// If order doesn't matter and you just want unique elements
std::vector<int> removeDuplicates(const std::vector<int>& arr) {
    std::unordered_set<int> seen;
    std::vector<int> result;

    for (int num : arr) {
        if (seen.insert(num).second) { // Only insert if not already present
            result.push_back(num);
        }
    }

    return result;
}
// ✅ If the array is already sorted and you want to remove duplicates in-place
int removeDuplicatesSorted(std::vector<int>& arr) {
    if (arr.empty()) return 0;

    int index = 1; // Index to place the next unique element

    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] != arr[i - 1]) {
            arr[index++] = arr[i];
        }
    }

    return index;
}
void unique_elements() {
    std::vector<int> arr = {4, 2, 4, 5, 2, 3, 1};
    std::vector<int> unique = removeDuplicates(arr);

    for (int num : unique)
        std::cout << num << " ";

// ✅ If the array is already sorted and you want to remove duplicates in-place
    std::vector<int> arr2 = {1, 2, 2, 3, 3, 4, 5, 5};
    int newSize = removeDuplicatesSorted(arr2);

    for (int i = 0; i < newSize; ++i)
        std::cout << arr2[i] << " ";

}

// Using std::set for automatic sorting + deduplication
std::vector<int> removeDuplicatesSortedSet(const std::vector<int>& arr) {
    std::set<int> s(arr.begin(), arr.end());
    return std::vector<int>(s.begin(), s.end());
}
// Time Complexity: O(n log n)

class BaseV { 
protected:
// public:
    virtual ~BaseV() { cout << "Base Distructor\n"; } 
};  // Not virtual!
class DerivedV : public BaseV { 
public: 
    ~DerivedV() { cout << "Derived Distructor\n"; } 
};
void Test_Virtual_Diastructor() {
    BaseV* b = new DerivedV();
    delete b;  // ❌ Only Base's destructor will run — resource leak
}

/*  abstract interface patterns where you don’t want external code to delete the object directly
This pattern is sometimes used in plugin interfaces or where Base objects must not be deleted from outside. */
class BaseV {
protected:
    virtual ~BaseV() { cout << "Base Destructor\n"; }
};

class DerivedV : public BaseV {
public:
    ~DerivedV() { cout << "Derived Destructor\n"; }

    static void destroy(BaseV* ptr) { delete ptr; }  // Can access protected dtor
};

int main() {
    BaseV* b = new DerivedV();
    DerivedV::destroy(b);  // ✅ safe destruction
}

int main() {
    
    // printf("%d\n", 5.3%5); // Mod % work only with integer
    printf("%f\n", fmod(5.3, 5));
    /*  g++ INTERVIEW_QUESTIONS.cpp -o output -lm
The -lm tells the linker to link against the math library, which contains fmod() and other math functions.*/

    // T_DECLTYPE();
    // Functor();

    // T2();

    // T_Int();

    // // PrintN();
    
    // mem();

    // T_AToI();
/*
    T_BaseAddr();
    Return_Array_From_Function();
    TwoD_Arr();
    Q1();

    T_FH();
    Find_Endian();

    T_FindStr();

    Vec();

    Multiple_Inheritance_Ambiguity();

    Hybrid_Inheritance_With_Dimond_Problem();
*/

    mem_test();
    Memcmp();

    Test_Virtual_Diastructor();
}