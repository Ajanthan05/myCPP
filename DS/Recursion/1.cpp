#include <iostream>
// using namespace std;

void f(int i, int n) {

    if(i>n) return;  // Base condition

    std::cout << "Ajanthan: " << i << "\n";

    f(i+1, n);
}


void f_reverse(int i, int n) {

    if(i < 1) return;  // Base condition
 
    std::cout << "Ajanthan: " << i << "\n";

    f_reverse(i-1, n);
}

// 3. Parameterised
void sum_of_N(int i, int sum) {
    if(i < 1) {
        std::cout << sum << "\n";
        return;
    }
    sum_of_N(i-1, sum+i);
}

// Functional Recursion
int sum(int n) {
    if(n == 0) return 0;
    return n + sum(n-1);
}

int main() {

    int n = 10;
    f(1,n);

    std::cout << std::endl;

    f_reverse(12, 10);

// 3. Parameterised
    std::cout << std::endl;
    sum_of_N(n, 0);

// 2. Functional Recursion
    int out = sum(n);
    std::cout << out << std::endl;

    return 0;
}