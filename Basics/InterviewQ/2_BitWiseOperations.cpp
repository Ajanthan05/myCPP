#include <iostream>


/*  🔹 1. Check if a number is even or odd
Q: Write a function to check if a number is even without using % or /.

bool isEven(int n) {
    return !(n & 1);
}
🧠 Explanation: The least significant bit (LSB) of an odd number is always 1. If (n & 1) is 1, then it's odd.

🔹 2. Count number of set bits in an integer
Q: Write a function to count the number of 1s in the binary representation of an integer.

int countSetBits(int n) {
    int count = 0;
    while (n) {
        n &= (n - 1); // clear the least significant set bit
        count++;
    }
    return count;
}
🧠 Explanation: n & (n - 1) removes the last set bit. This is known as Brian Kernighan's algorithm.

🔹 3. Check if a number is power of 2
Q: Determine if a number is a power of two.

bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}
🧠 Explanation: Power of two has only one bit set in binary, so n & (n-1) will be 0.

🔹 4. Swap two numbers without using a temp variable
void swap(int &a, int &b) {
    a ^= b;
    b ^= a;
    a ^= b;
}
🧠 Explanation: XOR swaps values in-place without extra space.

🔹 5. Find the only non-repeating element in an array where every other element appears twice
int singleNumber(vector<int>& nums) {
    int result = 0;
    for (int num : nums) {
        result ^= num;
    }
    return result;
}
🧠 Explanation: XOR of same numbers cancels out, so we’re left with the unique number.

🔹 6. Get the i-th bit of a number
bool getBit(int n, int i) {
    return (n & (1 << i)) != 0;
}
🔹 7. Set the i-th bit of a number
int setBit(int n, int i) {
    return n | (1 << i);
}
🔹 8. Clear the i-th bit
int clearBit(int n, int i) {
    return n & ~(1 << i);
}
🔹 9. Toggle the i-th bit
int toggleBit(int n, int i) {
    return n ^ (1 << i);
}
*/

// Clear all bits from LSB to i-th bit (inclusive)
int clearBitsUptoI(int n, int i) {
    return n & (~((1 << (i + 1)) - 1));
}








/*  Longest Increasing Subsequence
Input: nums = [10,9,2,5,3,7,101,18]
Output: 4
Explanation: The longest increasing subsequence is [2,3,7,101], therefore the length is 4.
*/


int lengthOfLIS(vector<int>& nums) {
        std::vector<int> tmp;
        tmp.push_back(nums[0]);
        for(int i=1; i<nums.size(); ++i) {
            if (nums[i] > tmp.back()) {
                tmp.push_back(nums[i]); 
            }
            else {
                int ind = std::lower_bound(tmp.begin(), tmp.end(), nums[i]) - tmp.begin();
                tmp[ind] = nums[i];
            }
        }
        return tmp.size();
    }










void Find_Endian() {
    unsigned int i = 1;
    char *c = (char*)&i;
    if (*c) cout << "Little Endian\n";
    else    cout << "Big Endian\n";
}

long long int ConvertEvenBitToZero(long long int n) {
    return (n & 0xAAAAAAAA);
}

int setBit(int N) {
    int count = 0;
    while (N) {
        if (N % 2 == 1) count++;
        N = N/2;
    }
    return count;
}
int setBit_O(int N) {
    int count = 0;
    while (N)
    {
        N = N & (N-1); // Why? Because subtracting 1 flips all bits after the lowest set bit, and & clears it.
        count++;
    }
    return count;
}
/*  
🔹 GCC/Clang:
int count = __builtin_popcount(N);  // O(1), hardware-supported

🔹 MSVC:
int count = __popcnt(N);  // MSVC intrinsic
But note:

These are non-portable (platform/compiler-specific).
Your current method is portable and efficient.*/
int swapNibbles(unsigned char x) {
    return ( (0x0F & x) << 4 | (0xF0 & x)  >> 4 );
}

vector<int> FindPrimeNoInRange(int s, int e) {
    for(int i=s; i<=e; ++i) {

    }
}