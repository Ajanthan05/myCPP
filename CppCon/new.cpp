#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

struct  A {
    int i;
    int u;

    A
};

void fun(A *a) {
    cout << a.i << "\n";
}

int add(int x, int y) { return x+y; }

int (*add)(int, int) = fun2;

int main() {
    A a;
    a.i = 5;

    func(&a);

    cout << func2(4,5) << "\n";


    int i = 5;
    int x = i | ((i << 14) & 1)>>14;

    return 0;
}