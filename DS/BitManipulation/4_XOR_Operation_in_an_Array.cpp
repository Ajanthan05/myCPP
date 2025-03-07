#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int xorUpto(int x) {
    if (x % 4 == 0) return x;
    if (x % 4 == 1) return 1;
    if (x % 4 == 2) return x + 1;
    return 0;
}

int xorOperation(int n, int start) {
    int xor_result = xorUpto(start / 2 + n - 1) ^ xorUpto(start / 2 - 1);
    return xor_result << 1 | (n & start & 1);
}

int main() {
    int n = 4, start=9;
    cout << (n & start & 1) << "\n";  // adjust the least significant bit (LSB) only when both n and start are odd.
    // adjust the least significant bit (LSB) only when both n and start are odd.

    /* The previous calculation only considers the halved numbers (ignoring the factor of 2).
We multiply the result by 2 (xor_result << 1), so we must preserve the least significant bit if needed.

Final Effect
If n and start are both odd, we set the LSB to 1.
Otherwise, we keep it 0.
*/
    return 0;
}