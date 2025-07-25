#include <iostream>
using namespace std;

template<class T1, class T2, int I>
class A {
public:
    static void print() { cout << "Primary Template\n"; }
};

template<class T, int I>
class A<T, T*, I> {
public:
    static void print() { cout << "Partial Specialization #1\n"; }
};

template<class T, class T2, int I>
class A<T*, T2, I> {
public:
    static void print() { cout << "Partial Specialization #2\n"; }
};

template<class T>
class A<int, T*, 5> {
public:
    static void print() { cout << "Partial Specialization #3\n"; }
};

template<class X, class T, int I>
class A<X, T*, I> {
public:
    static void print() { cout << "Partial Specialization #4\n"; }
};

void T_static() {
    A<double, double*, 3>::print(); // #1
    A<float*, char, 10>::print();   // #2
    A<int, double*, 5>::print();    // #3
    A<char, long*, 7>::print();     // #4
    A<char, int, 2>::print();       // primary
}


/*  TEMPLATE INSTANTIATIONS
Compile code for DEDUCED OR SPECIFIED code*/

/*  Iterate over elements of different containers
with the same generic code */
template<typename T>
void print(const T& coll)
{
    for (const auto& elem : coll) {
    std::cout << elem << '\n';
}
}







int main() {

}
