#include <iostream>
#include <vector>
#include <climits>

/* TC = O(2^N)
   SC = O(N)
*/
int f(int ind, std::vector<int> &a) {
    if (ind == 0) return a[0];
    if (ind < 0) return 0;

    int pick = a[ind] + f(ind-2, a);
    int not_pick = f(ind-1, a);
    return std::max(pick, not_pick);
}

/* TC = O(N)
   SC = O(N) + O(N)
*/
int f_dp(int ind, std::vector<int> &a, std::vector<int> &dp) {
    if (ind == 0) return a[0];
    if (ind < 0) return 0;

    if (dp[ind] != -1) return dp[ind];
    int pick = a[ind] + f_dp(ind-2, a, dp);
    int not_pick = f_dp(ind-1, a, dp);
    return dp[ind] = std::max(pick, not_pick);
}

/* TABULATION 
   TC = O(N)
   SC = O(N)  // No auxiliary stack space
*/
int f_T(int ind, std::vector<int> &a, std::vector<int> &dp) {
    dp[0] = a[0];    // if (ind == 0) return a[0];
    // int neg = 0;    // if (ind < 0) return 0;
    int n = a.size();
    for (int i=1; i<n; i++) {
        int take = a[i];
        if (i>1) take += dp[i-2];
        int not_take = dp[i-1];
        dp[i] = std::max(take, not_take);
    }
    return dp[n-1];
}

/* TABULATION => Space Optimization
   TC = O(N)
   SC = O(1)  // No auxiliary stack space
*/
int f_T_SO(int ind, std::vector<int> &a) {
    int prev = a[0]; // dp[0] = a[0];    // if (ind == 0) return a[0];
    int prev2 = 0;   // int neg = 0;    // if (ind < 0) return 0;
    int n = a.size();
    for (int i=1; i<n; i++) {
        int take = a[i];
        if (i>1) take += prev2;
        int not_take = prev;
        int cur = std::max(take, not_take);

        prev2 = prev;
        prev = cur;
    }
    return prev;
}


int main() {

    std::vector<int> a = {2,1,4,9};
    std::cout << f(a.size() - 1, a) << "\n";

    std::vector<int> a2 = {2,1,4,9};
    std::vector<int> dp2(a2.size()+1, -1);
    std::cout << f_dp(a2.size() - 1, a, dp2) << "\n";

    std::vector<int> a3 = {2,1,4,9};
    std::vector<int> dp3(a2.size()+1, -1);
    std::cout << f_dp(a3.size() - 1, a, dp3) << "\n";

    std::vector<int> a4 = {2,1,4,9};
    std::vector<int> dp4(a2.size(), 0);
    std::cout << f_T(a4.size() - 1, a4, dp4) << "\n";

    std::vector<int> a5 = {2,1,4,9};
    std::cout << f_T_SO(a5.size() - 1, a5) << "\n";

    return 0;
}