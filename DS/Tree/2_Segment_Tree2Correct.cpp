#include <iostream>
#include <vector>
#include <climits>

class SGTree {
    std::vector<int> seg;
    
public:
    SGTree(int n) {
        seg.resize(4 * n + 1);
    }
    void build(int ind, int low, int high, std::vector<int>& arr, std::vector<int>& seg) {
        if (low == high) {
            seg[ind] = arr[low];
            return;
        }

        int mid = (low + high) / 2;

        build(2 * ind + 1, low, mid, arr, seg);
        build(2 * ind + 2, mid + 1, high, arr, seg);

        seg[ind] = std::min(seg[2 * ind + 1], seg[2 * ind + 2]);
    }

    int query(int ind, int low, int high, int l, int r, std::vector<int>& seg) {
        // No overlap
        if (high < l || low > r) return INT_MAX;

        // Complete overlap
        if (low >= l && high <= r) return seg[ind];

        int mid = (low + high) / 2;

        int left = query(2 * ind + 1, low, mid, l, r, seg);
        int right = query(2 * ind + 2, mid + 1, high, l, r, seg);
        return std::min(left, right);
    }

    void update(int ind, int low, int high, int i, int val, std::vector<int>& seg) {
        if (low == high) {
            seg[ind] = val;
            return;
        }

        int mid = (low + high) / 2;
        if (i <= mid) update(2 * ind + 1, low, mid, i, val, seg);
        else update(2 * ind + 2, mid + 1, high, i, val, seg);

        seg[ind] = std::min(seg[2 * ind + 1], seg[2 * ind + 2]);
    }
};

void solve() {
    int n1;
    std::cin >> n1;
    
    std::vector<int> arr1(n1);
    for (int i = 0; i < n1; i++) std::cin >> arr1[i];
    SGTree sg1(n1);

    std::vector<int> seg1(4 * n1, INT_MAX);
    sg1.build(0, 0, n1 - 1, arr1, seg1);

    // std::cout << seg[0] << std::endl;

    int n2;
    std::cin >> n2;
    
    std::vector<int> arr2(n2);
    for (int i = 0; i < n2; i++) std::cin >> arr2[i];
    SGTree sg2(n2);

    std::vector<int> seg2(4 * n2, INT_MAX);
    sg2.build(0, 0, n2 - 1, arr2, seg2);



    int q;
    std::cin >> q;
    while (q--) {
        int type;
        std::cin >> type;
        if (type == 1) {
            int l1, r1, l2, r2;
            std::cin >> l1 >> r1 >> l2 >> r2;
            int min1 = sg1.query(0, 0, n1 - 1, l1, r1, seg1);
            int min2 = sg2.query(0, 0, n2 - 1, l2, r2, seg2);
            std::cout << std::min(min1, min2) << std::endl;
        } else {
            int arrNo, i, val;
            std::cin >> arrNo >> i >> val;
            
            if(arrNo == 1) {
                sg1.update(0, 0, n1 - 1, i, val, seg1);
                arr1[i] = val;
            }
            else {
                sg2.update(0, 0, n2 - 1, i, val, seg2);
                arr2[i] = val;
            }
        }
    }
}

int main() {
    freopen("Inclass.txt", "r", stdin);
    freopen("OutClass.txt", "w", stdout);

    solve();
    return 0;
}


/*
1  // Initial minimum of full array
2  // Min in range [1,3] -> min(3,2,7) = 2
1  // Min in range [0,4] after update at index 2 -> min(1,3,4,7,5) = 1
1  // Min in range [2,4] after update at index 3 -> min(4,1,5) = 1
1  // Min in range [0,4] -> min(1,3,4,1,5) = 1
*/