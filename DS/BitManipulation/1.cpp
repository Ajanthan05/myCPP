#include <iostream>
using namespace std;

void bitManipulation(unsigned int num, int i) {
    i = i - 1;  // Convert 1-indexed to 0-indexed
    int get_bit = (num & (1 << i)) >> i;  // Extract the i-th bit
    int set_bit = num | (1 << i);           // Set the i-th bit
    int clear_bit = num & (~(1 << i));        // Clear the i-th bit
    cout << get_bit << " " << set_bit << " " << clear_bit;
}

bool checkKthBit(int n, int k) {
    return (n >> k) & 1;
}

bool isEven(int n) {
    return !(n & 1);  // return (n & 1) == 0;
}

/*
n > 0: This ensures that v
n is positive. Negative numbers and zero are not considered powers of two.
!(n & (n - 1)): For positive numbers that are powers of two, only one bit is set, so 
n&(n−1) evaluates to 0.
This way, you avoid subtracting 1 from a negative number, preventing the signed integer overflow error.
*/
bool isPowerOfTwo(int n) {
    return n > 0 && (!(n & (n - 1)));
    //return n && (!(n & (n - 1))); //return (n==0) ? 0 : (!(n&(n-1)));
}

int main() {
    unsigned int num;
    int i;
    // Assuming the input consists of multiple test cases
    while (cin >> num >> i) {
        bitManipulation(num, i);
        cout << "\n";
    }
    return 0;
}
