#include <iostream>

using namespace std;

class Singleton {
private:
    Singleton() {}

    static Singleton s_Instance;

    float m_Member = 0.0f;

public:
    Singleton(const Singleton&) = delete; // Delete copy constructor

    static Singleton& Get() {
       return s_Instance; 
    }

    void func() { cout << "Func\n"; }
};

Singleton Singleton::s_Instance;  // static member we have to define outside 

void Test() {
    Singleton::Get().func();  // Called func in singleton instance
    // OR
    // Singleton instance = Singleton::Get();  This throw error after delete copy constructor
    // instance.func();

    // But & it work
    Singleton &instance = Singleton::Get();  
    instance.func();
}

////////////////////////////////

class Random {
private:
    Random() {}

    static Random s_Instance;

    float m_RandomGenerator = 0.5f;

    
public:
    Random(const Random&) = delete; // Delete copy constructor

    static Random& Get() {
       return s_Instance; 
    }

    float Float() { return m_RandomGenerator; }
};

Random Random::s_Instance;

void Test2() {
    float num = Random::Get().Float();
    // OR

    auto &random = Random::Get();
    float num2 = random.Float();
}

//////////////// To avoid ::Get().Float() ///////////////////////////
class Random_Imp {
private:
    Random_Imp() {}

    // static Random_Imp s_Instance;

    float m_RandomGenerator = 0.5f;

    float Float_Implementation() { return m_RandomGenerator; }
public:
    Random_Imp(const Random_Imp&) = delete; // Delete copy constructor

    static Random_Imp& Get() {
        static Random_Imp s_Instance; // static variable inside a function
        return s_Instance; 
    }

    // Here Float_Implementation is not static so it can access all class members
    static float Float() { return Get().Float_Implementation(); }
};

// Random_Imp Random_Imp::s_Instance;

void Test_Imp() {
    float num = Random_Imp::Get().Float();
    // OR

    auto &random = Random_Imp::Get();
    float num2 = random.Float();
}

/*  U dont need a class to define Singletons u can use namespace
But u will looce private public => But can over come this by defie in headae
and declare in cpp file

We actually loose that, we can define it in a instance
Singleton &instance = Singleton::Get(); 


Advantages in previous design
Stored as a function-local static object, created on first use.

Automatic storage (on stack) — destroyed automatically when the program ends.
Uses function-local static, which is guaranteed thread-safe initialization in C++11 and later.
No need for a mutex.

*/

int main() {
    Test();
    Test2();

    Test_Imp();
}

// https://www.youtube.com/watch?v=PPup1yeU45I&ab_channel=TheCherno