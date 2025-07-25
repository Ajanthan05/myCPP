#include <iostream>
using namespace std;

/*  🔹 1. Basic Bit Manipulation Questions
Check if a number is even or odd using bitwise operator.

Swap two numbers without using a temporary variable.

Find the only non-repeating element in an array where every other element repeats twice.

Find the two non-repeating elements in an array where every other element repeats twice.

Check if a number is a power of two.

Count the number of set bits (1s) in an integer.

Turn off the rightmost set bit.

Check if a particular bit is set in a number.

Set, clear, or toggle a specific bit in a number.

Find XOR of all elements in an array.*/



void test() {
    int n = 4;
    if(n % 2 == 0) cout << "Even\n";

    int x=3, y = 5;
    x = x ^ y;
    // y = 

    // check for power of 2
    while(n) {
        if(n%2 == 1) cout << "Not Power of 2\n";
        n = n >> 1;
    }

    int setBits = 0;
    while(n) {
        setBits += (n>>1) && 1;
        n = n >> 1;
    }
}