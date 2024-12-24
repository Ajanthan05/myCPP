#include <iostream>
using namespace std;


/* It saves the state of the function. Here no need of 
Staric variable*/

class Mul {
    int _val;
public:
    Mul() {}
    Mul(int val): _val(val) {}

    int operator () (int val) {
        return val*_val;
    }
};

int main() {

    Mul mul12(12);
    cout << mul12(2) << endl;
    cout << mul12(3) << endl;
    return 0;
}