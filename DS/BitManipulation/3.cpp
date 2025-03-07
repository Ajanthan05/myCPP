#include <iostream>
using namespace std;

string addBinary(string a, string b) {
    string result = "";
    int i = a.size() - 1, j = b.size() - 1, carry = 0;

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';  // Convert char to int
        if (j >= 0) sum += b[j--] - '0';  
        
        result = char((sum % 2) + '0') + result; // Append binary digit
        carry = sum / 2;  // Carry for next iteration
    }

    return result;
}

int main() {
    string a = "1010", b = "1101";
    cout << "Sum: " << addBinary(a, b) << endl;  // Output: 10111
    return 0;
}
