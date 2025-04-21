#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>
#include <cstring> // strlen
// #include <stdint.h>
#include <cstdint>
using namespace std;

class String {
public:
    String() : m_Data(nullptr), m_Size(0) {}

    String(const char* string) {
        std::cout << "Created\n";
        m_Size = strlen(string);
        m_Data = new char[m_Size + 1];
        memcpy(m_Data, string, m_Size);
        m_Data[m_Size] = '\0'; // null-terminate
    }

    // Copy constructor
    String(const String& other) {
        std::cout << "Copied\n";
        m_Size = other.m_Size;
        m_Data = new char[m_Size + 1];
        memcpy(m_Data, other.m_Data, m_Size + 1);
    }

    // Move constructor
    String(String&& other) noexcept {
        std::cout << "Moved\n";
        m_Size = other.m_Size;
        m_Data = other.m_Data; // Insted of copying we are moving the pointer

        // Make old string as empty state
        // When old string deleted in distructor it delete nullptr
        other.m_Data = nullptr;
        other.m_Size = 0;
    }

    // Copy assignment
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] m_Data;
            m_Size = other.m_Size;
            m_Data = new char[m_Size + 1];
            memcpy(m_Data, other.m_Data, m_Size + 1);
        }
        return *this;
    }

    // Move assignment
    String& operator=(String&& other) noexcept {  // It is taking the temprory object String&& other
        if (this != &other) {
            delete[] m_Data;
            m_Size = other.m_Size;
            m_Data = other.m_Data;
            other.m_Data = nullptr;
            other.m_Size = 0;
        }
        return *this;
    }

    void Print() const {
        if (m_Data)
            std::cout << m_Data << "\n";
    }

    ~String() {
        std::cout << "Destroyed\n";
        delete[] m_Data;
    }

private:
    char* m_Data;
    uint32_t m_Size;
};

class Entity {
public:
    Entity(const String& name) : m_Name(name) {}

    // For move
    // we explicitly cast it to a temporary
    // Entity(String&& name) 
    //     : m_Name((String&&)name)  
    // {}
    
    // Insted of casting rvalu reference like (String&&) -> Wecan use move
    Entity(String&& name) 
        : m_Name(move(name))  
    {}

    void PrintName() {m_Name.Print();}

private:
    String m_Name;
};

int main() {

    Entity entity("Ajanthan"); // We need copy constuctr
    // or
    // Entity entity(String("Ajanthan"));

    entity.PrintName();

    /*
    Created
    Copied
    Destroyed
    Ajanthan
    Destroyed
        To avoid copy
*/

/* We are saved using Copy constructor
Created
Moved
Destroyed
Ajanthan
Destroyed
*/

// Move Assignment
    String string = "Hellow";
    // String dest((String&&)string);
    String dest2(std::move(string));

    String dest3; 
    dest3 = std::move(string);
    dest3.operator=(std::move(string)); // Same


    // Move assignment
    String s = "App;e";
    String d;
    d = std::move(s);

    return 0;
}
