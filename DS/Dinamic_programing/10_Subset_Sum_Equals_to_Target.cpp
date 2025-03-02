#include <iostream>
#include <vector>
#include <climits>

using namespace std;

bool fun(int ind, int target, int n, int k, vector<int> &arr) {
    if (target == 0) return true;
    if (ind == 0) return (arr[0] == target);

    bool not_take = fun(ind-1, target, n, k, arr);
    bool take = false;
    if (target >= arr[ind]) {
        take = fun(ind-1, target - arr[ind], n, k, arr);
    }
    
    return take || not_take;
}

bool subsetSumToK(int n, int k, vector<int> &arr) {
    return fun(n-1, k, n, k, arr);
}

/* DP
*/
bool fun_dp(int ind, int target, int n, int k, vector<int> &arr, vector<vector<int>> &dp) {
    if (target == 0) return true;
    if (ind == 0) return (arr[0] == target);
    if (dp[ind][target] != -1) return dp[ind][target];

    bool not_take = fun_dp(ind-1, target, n, k, arr, dp);
    bool take = false;
    if (target >= arr[ind]) {
        take = fun_dp(ind-1, target - arr[ind], n, k, arr, dp);
    }
    
    return dp[ind][target] = (take || not_take);
}

bool subsetSumToK_dp(int n, int k, vector<int> &arr) {
    vector<vector<int>> dp(n, vector<int>(k+1, -1));
    return fun_dp(n-1, k, n, k, arr, dp);
}

/*  MEMOIZATION 
*/

bool subsetSumToK_MEMOIZATION(int n, int k, vector<int> &arr) {
    vector<vector<bool>> dp(n, vector<bool>(k+1, 0));
    for(int i=0; i<n; i++) dp[i][0] = true;
    if(arr[0] <= k) dp[0][arr[0]] = true;

    for(int ind=1; ind<n; ind++) {
        for(int target=1; target<k; target++) {
            bool not_take = dp[ind-1][target];
            bool take = false;
            if (target >= arr[ind]) {
                take = dp[ind-1][target-arr[ind]];
            }
            
            dp[ind][target] = (take | not_take);
        }
    }
    return dp[n-1][k];
}

int main() {


    return 0;
}