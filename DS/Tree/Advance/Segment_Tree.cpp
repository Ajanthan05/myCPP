#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class SegTree {
    std::vector<int> seg;
public:
    SegTree() {}
    
    SegTree(int n) {
        seg.resize( 4 * n + 1);
    } 

    void build(int ind, int low, int high, vector<int> arr) {
        if (low == high) {
            seg[ind] = arr[low];
            return;
        }

        int mid = low + (high - low) >> 1;
        build(2*ind+1, low, mid, arr);
        build(2*ind+2, mid + 1, high, arr);
        seg[ind] = min(seg[2*ind+1], seg[2*ind+2]);
    }

    int quary(int ind, int low, int high, int l, int r) {
        // No overlap
        // [low high] l r   OR  l r [low high]
        if (high < l && r < low) {
            return INT_MAX;
        }

        // complete overlap
        else if (l < low && high < r) {
            return seg[ind];
        }

        // Partial Overlap
        else {
            int mid = low + (high-low)>>1;
            int left  = quary(2 * ind + 1, low, mid, l, r);
            int right = quary(2 * ind + 2, mid + 1, high, l, r);
            return min(left, right);
        }
    }
    void update(int ind, int low, int high, int i, int val) {
        if (low == high) {
            seg[ind] = val; 
        }

        int mid = low + (high-low)>>1;
        if (i <= mid) update(2 * ind + 1, low, mid, i, val);
        else          update(2 * ind + 2, mid + 1, high, i, val); 
        
        sed[ind] = min( seg[2*ind+1], seg[2*ind+2] );
    }

    const std::vector<int>& getSegmentTree() const {
        return seg;
    }
};

int main() {
    int n1;
    cin >> n1;
    vector<int> vec1(n1);
    for (int i=0; i<n1; ++i) {
        // int val;
        // cin >> val;
        // vec.emplace_back(val);
        std::cin >> vec1[i];
    }
    SegTree sg1(n1);
    sg1.build(0, 0, n1-1, vec1);


    int n2;
    cin >> n2;
    vector<int> vec2(n2);
    for (int i=0; i<n2; ++i) {
        // int val;
        // cin >> val;
        // vec.emplace_back(val);
        std::cin >> vec2[i];
    }
    SegTree sg2(n2);
    sg2.build(0, 0, n2-1, vec2);

    int q;
    cin >> q;
    while(q--) {
        int type;
        cin >> type;
        if(type == 1) {
            int l1, r1, l2, r2;
            cin >> l1 >>r1 >> l2 >> r2;
            int min1 = sg1.quary(0, 0, n1-1, l1, r1);
            int min2 = sg2.quary(0, 0, n2-1, l2, r2);
            cout <<min(min1, min2) << "\n";
        }
        else {
            int arrNo, i, val;
            cin >> arrNo >> i >> val;
            if (arrNo == 1) {
                sg1.update(0,0,n1-1,i,val);
                vec1[i] = val;
            }
            else {
                sg2.update(0,0,n2-1,i,val);
                vec2[i] = val;
            }
        }
    }
}

void build3(int ind, int low, int high, int arr[], int seg[], int orr) {
    if (low == high) {
        seg[ind] = arr[low];
        return;
    }

    int mid = (low + high) / 2;
    build3(2*ind+1, low, mid, arr, seg, !orr);
    build3(2*ind+2, mid+1, high, arr, seg, !orr);
    if(orr) seg[ind] = seg[2*ind+1] | seg[2*ind+2];
    else    seg[ind] = seg[2*ind+1] ^ seg[2*ind+2];
}

void solve3() {
    int n, q;
    cin >> n >> q;
    int el = pow(2, n);
    int arr[el];
    for(int i=0; i<el; i++) {
        cin >> arr[i];
    }
    int seg[4 * el];
    if (n % 2 == 0) build3(0, 0, el-1, arr, seg, 0);
    else            build3(0, 0, el-1, arr, seg, 1);
}