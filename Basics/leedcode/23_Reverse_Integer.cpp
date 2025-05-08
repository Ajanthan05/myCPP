#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

int reverse(int x) {
    int positive = 1;
    if(x<0) {
        positive = -1;
        x *= -1;
    }
    int ans = 0;
    while(x) {
        ans = ans*10 + x % 10;
        x = x/10;
    }
    return ans*positive;
}

int reverse(int x) {
    int ans = 0;

    while (x != 0) {
        int digit = x % 10;
        x /= 10;

        // Check if the new ans will overflow
        if (ans > INT_MAX/10 || (ans == INT_MAX/10 && digit > 7)) return 0;
        if (ans < INT_MIN/10 || (ans == INT_MIN/10 && digit < -8)) return 0;

        ans = ans * 10 + digit;
    }

    return ans;
}

int main() {
    int x = -123;
    cout << reverse(x) << "\n";

    return 0;
}