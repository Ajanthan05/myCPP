#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

// int fun(int ind, vector<int>& coins, int t) {
    
//     if (ind == 0) {
//         if (t % coins[0] == 0) return t / coins[0];
//         else return INT_MAX;
//     }
//     int notPick = fun(ind-1, coins, t);
//     // if (notPick == -1) return -1;
//     int pick = INT_MAX;  
//     // stay at the same index  no ind-1
//     if(t >= coins[ind]) pick = 1 + fun(ind, coins, t-coins[ind]); 
//     // if (pick == -1) return -1;
//     return min(notPick, pick);
// }

// int coinChange(vector<int>& coins, int amount) {
//     return fun(coins.size()-1, coins, amount);
// }

/* TC > O(2^N) 
   SC > O(N)
*/
int fun(int ind, vector<int>& coins, int t) {
    if (ind == 0) {
        return (t % coins[0] == 0) ? (t / coins[0]) : 1e9; // Large value instead of INT_MAX
    }
    
    int notPick = fun(ind - 1, coins, t);
    int pick = 1e9; // Large value to avoid overflow

    if (t >= coins[ind]) pick = 1 + fun(ind, coins, t - coins[ind]);

    return min(notPick, pick);
}


int coinChange(vector<int>& coins, int amount) {
    int res = fun(coins.size() - 1, coins, amount);
    return (res >= 1e9) ? -1 : res;  // Convert impossible cases to -1
}

int fun_dp(int ind, vector<int>& coins, int t, vector<vector<int>> &dp) {
    if (ind == 0) {
        return (t % coins[0] == 0) ? (t / coins[0]) : 1e9; // Large value instead of INT_MAX
    }

    if (dp[ind][t] != -1) return dp[ind][t];

    int notPick = fun_dp(ind - 1, coins, t, dp);
    int pick = 1e9; // Large value to avoid overflow

    if (t >= coins[ind]) pick = 1 + fun_dp(ind, coins, t - coins[ind], dp);

    return dp[ind][t] = min(notPick, pick);
}

int coinChange_dp(vector<int>& coins, int amount) {
    int n = coins.size();
    vector<vector<int>> dp(n, vector<int>(amount + 1, -1));
    int res = fun_dp(n - 1, coins, amount, dp);
    return (res >= 1e9) ? -1 : res;  // Convert impossible cases to -1
}
/* TABULATION 

*/
int coinChange_T(vector<int>& coins, int target) {
    int n = coins.size();
    vector<vector<int>> dp(n, vector<int>(target + 1, 1e9));

    for(int t=0; t <= target; t++) {
        if(t % coins[0] == 0) dp[0][t] = t / coins[0];
        // else dp[0][t] = 1e9;
    }

    for(int ind=1; ind < n; ind++) {
        for(int t=0; t <= target; t++) {

            int notPick = dp[ind - 1][t];
            int pick = 1e9; // Large value to avoid overflow

            if (t >= coins[ind]) pick = 1 + dp[ind][t - coins[ind]];

            dp[ind][t] = min(notPick, pick);
        }
    }

    return (dp[n-1][target] >= 1e9) ? -1 : dp[n-1][target];  // Convert impossible cases to -1
}

/* SPACE OPTIMIZE */
int coinChange_S(vector<int>& coins, int target) {
    int n = coins.size();
    vector<int> prev(target+1, 1e9), cur(target+1, 1e9);

    for(int t=0; t <= target; t++) {
        if(t % coins[0] == 0) prev[t] = t / coins[0];
        // else dp[0][t] = 1e9;
    }

    for(int ind=1; ind < n; ind++) {
        for(int t=0; t <= target; t++) {

            int notPick = prev[t];
            int pick = 1e9; 

            if (t >= coins[ind]) pick = 1 + cur[t - coins[ind]];

            cur[t] = min(notPick, pick);
        }
        prev = cur;
    }

    return (prev[target] >= 1e9) ? -1 : prev[target];  
}

int main() {
    vector<int> coins = {1, 2, 5};
    int amount = 11;
    cout << coinChange(coins, amount) << endl;  // Expected output: 3 (5+5+1)
    return 0;
}
