#include <iostream>
#include <vector>
#include <string>

/*
Open brackets
Close brackets
Ful brackets

min(opening from        close from
    left                right)

        = full
*/

struct info {
    int open, close, full;

    info() {
        open = 0;
        close = 0;
        full = 0;
    }

    info(int _open, int _close, int _full) : open(_open), close(_close), full(_full) {}
};

info merge(info left, info right) {
    // info *ans = new info(0,0,0);
    info ans;
    int matched = std::min(left.open, right.close);
    
    ans.full = left.full + right.full + matched;
    ans.open = left.open + right.open - matched;
    ans.close = left.close + right.close - matched;

    return ans;
}

void build(int ind, int low, int high, std::string s, info seg[]) {
    if (low == high) {
        seg[ind] = info(s[ind] == '(', s[ind] == ')', 0);
        return;
    }

    int mid = (low + high) >> 1;
    build(2 * ind + 1, low, mid, s, seg);
    build(2 * ind + 2, mid+1, high, s, seg);

    seg[ind] = merge(seg[2*ind+1], seg[2*ind+2]);
}

info query(int ind, int low, int high, int l, int r, info seg[]) {
    if (r < low || high < l) return info();
    if (low >= l && r >= high) return seg[ind];

    int mid = (low + high) >> 1;

    info left = query(2*ind+1, low, mid, l, r, seg);
    info right = query(2*ind+2, mid+1, high, l, r, seg);
    return merge(left, right);
}

void solve() {
    std::string s;
    std::cin >> s;

    int n = s.size();
    info seg[4 * n];

    int q;
    std::cin >> q;
    while(q--) {
        int l, r;
        std::cin >> l >> r;
        l--, r--;
        info ans = query(0, 0, n-1, l, r, seg);
        std::cout << ans.full * 2 << std::endl;
    }
}

int main() {

    freopen("4.txt", "r", stdin);
    freopen("4Out.txt", "w", stdout);

    solve();
    return 0;
}