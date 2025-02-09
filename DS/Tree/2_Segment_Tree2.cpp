#include <iostream>
#include <vector>
#include <climits>

void build(int ind, int low, int high, int arr[], int seg[]) {
    if (low == high) {
        seg[ind] = arr[low];
        return;
    }

    int mid = (low + high) / 2;

    build(2 *ind + 1, low, mid, arr, seg);
    build(2 * ind + 2, mid+1, high, arr, seg);

    seg[ind] = std::min(seg[2*ind+1], seg[2*ind+2]);
}

int query(int ind, int low, int high, int l, int r, int seg[]) {
    // No overlap
    if(high < l || low > r) return INT_MAX;

    // Complete overlap
    if (low >= l && high <= r) return seg[ind];

    int mid = (low + high) / 2;
    
    int left = query(2*ind+1, low, mid, l, r, seg);
    int right = query(2*ind+2, mid+1, high, l, r, seg);
    return std::min(left, right);
}

void update(int ind, int low, int high, int i, int val, int seg[]) {
    if (low == high) {
        seg[ind] = val;
        return;
    }

    int mid = (low + high) / 2;
    if (i <= mid) update(2*ind+1, low, mid, i, val, seg);
    else update(2*ind+2, mid+1, high, i, val, seg);
    
    seg[ind] = std::min(seg[2*ind+1], seg[2*ind+2]);
}

void solve() {
    int n;
    std::cin >> n;
    int arr[n];
    for (int i=0; i<n; i++) std::cin >> arr[i];
    int seg[4 * n];
    build(0, 0, n-1, arr, seg);
    std::cout << seg[0] << std::endl;
    int q;
    std::cin >> q;
    while(q--) {
        int type;
        std::cin >> type;
        if(type == 1) {
            int l, r;
            std::cin >> l >> r;
            std::cout << query(0, 0, n-1, l, r, seg) << std::endl;
        }
        else {
            int i, val;
            std::cin >> i >> val;
            update(0, 0, n-1, i, val, seg);
            arr[i] = val;
        }
    }
}

signed main() {

    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);

    solve();
    return 0;
}