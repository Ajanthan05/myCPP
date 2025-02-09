#include <iostream>
#include <vector>
#include <climits>

int a[100005], seg[4 * 100005];

void build(int ind, int low, int high) {
    if(low == high) {
        seg[ind] = a[low];
        return;
    }

    int mid = (low + high) / 2;
    build(2*ind+1, low, mid);
    build(2*ind+2, mid+1, high);

    seg[ind] = std::max(seg[2*ind+1], seg[2*ind+2]);
}

int quary(int ind, int low, int high, int l, int r) {
    if (low >= l && high <= r) return seg[ind];

    if (high<l || low>r) return INT_MIN;

    int mid = (low + high) / 2;
    int left = quary(2*ind+1, low, mid, l, r);
    int right = quary(2*ind+1, mid+1, high, l, r);

    return std::max(left, right);
}

void update(int ind, int low, int high, int i, int val) {
    if(low == high) {
        if(low == high) 
        seg[ind] = val;
        return;
    }
    
    int mid = (low + high) / 2;
    if(i <= mid) update(2*ind+1, low, mid, i, val);
    if(i > mid) update(2*ind+2, mid+1, high, i, val);

    seg[ind] = std::max(seg[2*ind+1], seg[2*ind+2]);
}

int main() {
    int n;
    std::cin >> n;
    for(int i=0; i<n; i++) {
        std::cin >> a[i];
    }

    build(0, 0, n-1);

    int q;
    std::cin >> q;
    while(q--) {
        int l, r;
        std::cin >> l >> r;
        std::cout << quary(0, 0, n-1, l, r) << "\n";
    }
    return 0;
}