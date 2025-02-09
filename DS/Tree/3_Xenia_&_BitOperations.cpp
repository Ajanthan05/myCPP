#include <iostream>
#include <vector>
#include <climits>
#include <cmath>

class Xdenia {

public:
    void build(int ind, int low, int high, std::vector<int> &arr, std::vector<int> &seg, bool orr) {
        if (low == high) {
            seg[ind] = arr[low];
            return;
        }

        int mid = (low + high) / 2;
        build(2*ind+1, low, mid, arr, seg, !orr);
        build(2*ind+2, mid+1, high, arr, seg, !orr);

        if(orr) seg[ind] = seg[2*ind+1] | seg[2*ind+2];
        else seg[ind] = seg[2*ind+1] ^ seg[2*ind+2];
    }

    void update(int ind, int low, int high, std::vector<int> &seg, bool orr, int i, int val) {
        if (low == high) {
            seg[ind] = val;
            return;
        }
        int mid = (low + high) / 2;
        if(i<=mid) update(2*ind+1, low, mid, seg, !orr, i, val);
        else update(2*ind+2, mid+1, high, seg, !orr, i, val);

        if(orr) seg[ind] = seg[2*ind+1] | seg[2*ind+2];
        else seg[ind] = seg[2*ind+1] ^ seg[2*ind+2];
    }

};

void solve() {
    int n, q;
    std::cin >> n >> q;
    int el = std::pow(2, n);
    std::vector<int> arr(el);
    for (int i=0; i< el; i++) std::cin >> arr[i];

    std::vector<int> seg(4 * el);

    Xdenia obj1;
    if(n % 2 == 0) obj1.build(0, 0, el-1, arr, seg, 0); // first xor
    else obj1.build(0, 0, el-1, arr, seg, 1); // second or

    while(q--) {
        int i, val;
        std::cin >> i >> val;
        i--;  // to make zero base index

        if(n % 2 == 0) obj1.update(0, 0, el-1, seg, 0, i, val);
        else obj1.update(0, 0, el-1, seg, 1, i, val);

        std::cout << seg[0] << "\n";
    }
}

signed main() {

    freopen("XwniaIN.txt", "r", stdin);
    freopen("XwniaOUT.txt", "w", stdout);

    solve();
    return 0;
}