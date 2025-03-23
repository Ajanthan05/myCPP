#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int fun(int ind, int w, vector<int> &val, vector<int> &wt) {
    
    if (ind == 0) {
        if (wt[ind] <= w) {
            return val[0];
        }
        else {
            return 0;
        }
    }

    int notPick = 0 + fun(ind-1, w, val, wt);
    int pick    = INT_MIN;
    if (wt[ind] <= w) {
        pick = val[ind] + fun(ind-1, (w - wt[ind]), val, wt);
    }

    return max(pick, notPick);
}

int knapsack(int W, vector<int> &val, vector<int> &wt) {
    int n = val.size();
    return fun(n-1, W, val, wt);
}


int fun_memoization(int ind, int w, vector<int> &val, vector<int> &wt, vector<vector<int>> &dp) {
    // if (ind == 0) {
    //     if (wt[ind] <= w) {
    //         return val[0];
    //     }
    //     else {
    //         return 0;
    //     }
    // }

    if (ind == 0) {  
        return dp[ind][w] = (wt[0] <= w) ? val[0] : 0;
    }
    if(dp[ind][w] != -1) return dp[ind][w];

    int notPick = 0 + fun_memoization(ind-1, w, val, wt, dp);
    int pick    = INT_MIN;
    if (wt[ind] <= w) {
        pick = val[ind] + fun_memoization(ind-1, (w - wt[ind]), val, wt, dp);
    }

    return dp[ind][w] = max(pick, notPick);
}

int knapsack_memoization(int W, vector<int> &val, vector<int> &wt) {
    int n = val.size();
    vector<vector<int>> dp(n, vector<int>(W + 1, -1));
    return fun_memoization(n-1, W, val, wt, dp);
}


int main() {

    vector<int> val = {60, 100, 120};
    vector<int> wt = {10, 20, 30};
    int W = 50;
    cout << knapsack_memoization(W, val, wt) << endl;  // Expected output: 220

    return 0;
}