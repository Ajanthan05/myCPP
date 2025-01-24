#include <iostream>
#include <string>
#include <map>
#include <climits>

std::string convert2Binary(int n) {
    std::string res;

    while (n > 0) {
        if (n%2 == 1) res = '1' + res;
        else          res = '0' + res;
        n = n/2;
    }
    return res.empty() ? "0" : res;
}

/* Pass String by Reference:

Use const std::string& bin instead of std::string bin to avoid unnecessary copying. */
int bin2Decimal(const std::string &bin) {
    int length = bin.length();
    int pof2 = 1;
    int num = 0;

    for (int i=length-1; i>=0; i--) {
        if (bin[i] == '1') num += pof2;

        pof2 *= 2;
    }    
    return num;
}

/*
TC = O(nLOG(m) + m)  // here m = N/2 + 1
SC = O(m)
*/
int findSingleElement(const int arr[], int size) {
    std::map<int, int> mpp;
    for (int i=0; i<size; i++) {
        mpp[arr[i]]++;
    }   // O(Nlog(m))

    for(auto it : mpp) {
        if (it.second == 1) return it.first;
    }
    return -1;
}

/*
TC = O(N) 
SC = O(1)
*/
int findSingleElement_xor(const int arr[], int size) {
    int xor_ = 0;
    for (int i=0; i<size; i++) {
        xor_ ^= arr[i];
    }
    return xor_;
}

/*
TC = (O(log2(N)))^2
SC = O(1)
*/
int divide_WoithOut_mul_devide(int dividend, int divisor) {
    // Handle edge case of overflow
        if (dividend == INT_MIN && divisor == -1) return INT_MAX;  /*
The issue occurs because dividing INT_MIN (-2147483648) by -1 causes overflow, as the result 
2147483648 exceeds the range of a 32-bit signed integer ([-2147483648, 2147483647]).*/

        if (dividend == divisor) return 1;

        // Determine the sign of the result
        bool sign = !((dividend < 0) ^ (divisor < 0));

        // Work with absolute values to simplify computation
        long n = std::abs(static_cast<long>(dividend));
        long d = std::abs(static_cast<long>(divisor));
        long ans = 0;

        while (n >= d) {
            int cnt = 0;
            while (n >= (d << (cnt + 1))) cnt++;
            ans += (1 << cnt);
            n -= (d << cnt);
        }

        // Apply the sign to the result
        ans = sign ? ans : -ans;

        // Ensure the result is within the 32-bit integer range
        if (ans >= INT_MAX) return INT_MAX;
        if (ans <= INT_MIN) return INT_MIN;
        return ans;
}

int main() {

    std::string ans = convert2Binary(4);
    std::cout << ans << "\n";

    std::cout << bin2Decimal("100") << "\n";
    std::string binary = "1101"; // Binary for 13
    int decimal = bin2Decimal(binary);
    std::cout << "Decimal value: " << decimal << std::endl;

    int arr[] = {4,1,2,1,2};
    int size = sizeof(arr) / sizeof(arr[0]);
    std::cout << findSingleElement(arr, size) << "\n";

    std::cout << "divide_WoithOut_mul_devide: " << divide_WoithOut_mul_devide(22, 3) << "\n";
    std::cout << "divide_WoithOut_mul_devide: " << divide_WoithOut_mul_devide(22, -3) << "\n";
    return 0;
}