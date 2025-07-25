#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <utility>
#include <algorithm>
#include <unordered_map>



#include <any>
#include <variant>
#include <optional>
#include <numeric>
#include <span>

using namespace std;

static int alloc = 0;
void* operator new(size_t s) {
    alloc++;
    cout << "Allocated: "  << alloc << " : " << s << endl;
    return malloc(s);
}

void String_view() {
    string s1{"Ajanthan is the Best"};
    string s2{s1};
    string s3 = s1;

    std::string_view sv{s1.substr(0, 4)};
    cout << sv << "\n";
}



class StringsManager {
public:
    template<typename T>
    void setSetting(const string& name, const T& value) {
        settings_[name] = value;
    }

    template<typename T>
    T getSetting(const string& name) const {
        auto it = settings_.find(name);
        if (it != settings_.end()) {
            return any_cast<T>(it->second);
        }
        return T{}; // thuis is when no mach, return default of the type
    }

private:
    unordered_map<string, any> settings_;
};

void Test_StringsManager() {
    StringsManager sm;

    sm.setSetting("TheameColor", string("Blue"));
    sm.setSetting("FrontSize", 12);
    sm.setSetting("SoundVolume  ", 0.5);

    cout << "TheamC: " << sm.getSetting<string>("TheameColor") << "\n";
}


void Clamp() {
    int x = 6;
    cout << std::clamp(x,3,5) << "\n";

    // Same as
    if (x < 3)      cout << 3 << "\n";
    else if (x > 5) cout << 5 << "\n";
    else            cout << x << "\n";
}

/////////////////////////////////// NAMESPACE   ///////////////
namespace Vehicle
{
    namespace Car
    {
        namespace Engine
        {
            class PetrolEngine {};
        } // namespace Engine
        
    } // namespace Car
    
} // namespace Vehicle

// From c++17
namespace Vehicle17::Car::Engine
{
    class PetrolEngine{};
} // namespace Vehicle17::Car::Engine


//////////////////////////  OPTIONAL    /////////////////////////
std::optional<string> getSomeString(bool a) {
    if (a) {
        return "Ajanthan";
    }
    return {};
}
void T_optional() {
    if (auto mystr = getSomeString(false)) {
        cout << *mystr << "\n";
    }
    else cout << "No Values\n";
}


///////////////////////     FIX COPPY ASSIGNMENT OPERATOR       ////////////////////////
class Buffer {
public:
    Buffer(size_t size) : size(size), data(new int[size]) {}

    ~Buffer() { delete[] data; }

    Buffer(const Buffer& other) :size(other.size), data(new int[other.size]) {
        std::copy(other.data, other.data +size, data);
    }

    // Buffer& operator(const Buffer& other) {
    //     if (this != &other) {
    //         delete[] data;      // Step1: destroy old data
    //         size = other.size;
    //         data = new int[size]; // setp3: Allocate new moeory (Could throw error)
    //         std::copy(other.data, other.data +size, data);
    //     }
    //     return *this;
    // }

    // olition copy swap idiom
    Buffer& operator=(Buffer& other) { // not reseiving as const reference we are making copy he itself
        swap(*this, other);
        return *this;
    }

    friend void swap(Buffer& first, Buffer& second) {
        using std::swap;
        swap(first.size, second.size);
        swap(first.data, second.data);
    }

private:
    size_t size;
    int* data;
};


/////////////////////// VARIENT ////////////////////////////
void Variant() {
    std::variant<double, int, std::string> v;
    // If we dident assign anything first will be default
    v = 5;
    v = 3.14;
    v = std::string("Ajanthan");

    if (auto doubleValue = std::get_if<double>(&v)) {
        cout << "double: " << *doubleValue << "\n";
    }
    else if (auto intValue  = std::get_if<int>(&v)) {
        cout << "int: " << *intValue << "\n";
    }
    else if (auto stringValue = std::get_if<std::string>(&v)) {
        cout << "String: " << *stringValue << "\n";
    }
    else cout << "NoValue\n";
}

//////////////////////////  Print varient  std::visit (callable, variant...) ///////////////////////////
void func(int i) {
    cout << "func(int): " << i << "\n";
}
void func(double i) {
    cout << "func(double): " << i << "\n";
}
void func(const string& i) {
    cout << "func(string): " << i << "\n";
}
void T_Print() {
    vector<variant<int, double, string>> vec = {1, 3.14, "Ajanthan"};

    for (auto& element :vec) {
        visit([](auto&& arg) { func(arg);}, element);
    }
}


void GCD_LCM() {
    int x = 2 * 2 * 3;
    int y = 2 * 3 * 3;
    cout << "GCD: " << std::gcd(x, y) << "LCM: " << std::lcm(x,y);
}


///////////////     PLACEMENT NEW   //////////////////////////////////
class Base {
public:
    Base()  { cout << "Constructor\n";}
    ~Base() { cout << "Distructor\n"; }
};

////////////////////    IF CONSTEXPER   /////////////////////////////
template <typename T>
auto length(T const& value) {
    if constexpr (is_integral<T>::value) {
        return value;
    }
    else {
        return value.length();
    }
}
void T_IF_CONSTEXPER() {
    int n = 10;
    string s{ "abc" };

    cout << "n = " << n << " and length = " << length(n) << "\n";
    cout << "s = " << n << " and length = " << length(s) << "\n";

    /*   error: request for member ‘length’ in ‘value’, which is of non-class type ‘const int’
  191 |         return value.length();*/
}

void Condition_In_IF_ELSE_and_SWITCH() {
    if (int x = min(3, 5); x == 3) {
        // do the job  
    }
}

class Foo {
private:
    int ind;
public:
    Foo(int i) : ind(i) {}
    ~Foo() { cout << "Diastructor\n";}
};

// Space sonstructor only
constexpr std::size_t capacity = 100;
void* raw_mem = operator new[](capacity * sizeof(Foo));
Foo *buffer =  static_cast<Foo*>(raw_mem);

void MemPool() {
    // Construct the Object also
    for(std::size_t i=0; i<20; ++i) {
        new (buffer + 1) Foo(i);
    }

    // Sestroy only the object and keep the memory
    std::destroy(buffer, buffer + 20);

    // Eeuse the Menory
    for(std::size_t i=0; i<5; ++i) {
        new (buffer + 1) Foo(100 + i);
    }

    // Finally destroy the object and free the memory also.
    std::destroy(buffer, buffer + 5);
    operator delete[](raw_mem);

    /* WHE N TO USE STD::DISTROY
    ur working with raw/uninitialized memory.
    construct the object manually with placement new.
    u want free-grained destruction in a custom data structore.
    It's a low-level tool, but essential for writing safe and roboust container-like classes
    */
}   

void T_PLACEMENT_NEW() {
    char *memory = new char[10 * sizeof(Base)];

    // New ask memory from the Kurnel and constructing the object in the memory
    Base *obj1 = new (&memory[0]) Base();
    Base *obj2 = new (&memory[4]) Base();
    Base *obj3 = new (&memory[8]) Base();

    obj1->~Base();
    obj2->~Base();
    obj3->~Base();

    delete[] memory;
}

// void vs(std::initializer_list<std::string> arg) {
//     new(&mbuf->vs) vector<string>(allo, arg);
// }

////////////////////////// SPAN C++20/////////////////////////////////////////
/*  Non-owning Reference: std::span does not assume ownership of the data it refers to. It acts as a wrapper around the existing data.
Lightweight: std::span is designed to be efficient, employing a small memory footprint. Typically, it consists of two pointers (begin and end) and a size value.
Contiguous Sequence: std::span is compatible only with contiguous data structures such as arrays, std::vector, or std::array. It cannot be utilized with non-contiguous data structures like linked lists.
Safety Measures: std::span incorporates bounds checking, ensuring secure access to the underlying data. It helps prevent common errors such as buffer overflows or underflows.*/
void Span() {
    vector<int> vec = { 1, 2, 3, 4, 5 };
    span<int> span_vec(vec);

    // Create a subspan form index 1 to 3
    std::span<int> subspan = span_vec.subspan(1, 3);

    for (const auto& num : subspan) {
        std::cout << num << " ";
    }

    // Directly create a subspan from the vector
    std::span<int> subspan2(vec.begin() + 1, 3);
}

// Function that accepts span created from pointer and size
void print_span(std::span<int> s) {
    for (const auto& num : s) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

void T_SPAN() {
    std::vector<int> vec;

    // Dynamically add elements
    for (int i = 1; i <= 10; ++i) {
        vec.push_back(i);
    }


    // Create a span over the entire vector
    std::span<int> span_vec(vec.data(), vec.size());

    // Print using the separate function
    print_span(span_vec);

    /*  OR
    {vec.data(), vec.size()} creates a temporary std::span<int> which is passed to print_span.
This approach is clean, efficient, and avoids extra span variables.*/
    // Directly create and pass the span to the function
    print_span({vec.data(), vec.size()});
}




int main() {
    Test_StringsManager();
    // String_view();

    Clamp();
    Variant();
 
    T_IF_CONSTEXPER();
    return 0;
}