#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* Cmins[ind] >= 1*/

int fun(int ind, int t, vector<int>& coins) {
    // if (target === 0) return 1;
    if (ind == 0) {
        return (t % coins[0] == 0) ? 1 : 0;
    }

    int notTake = fun(ind-1, t, coins);
    int take = 0;
    if (t >= coins[ind]) take = fun(ind, t-coins[ind], coins);

    return notTake + take;
}

int change(int amount, vector<int>& coins) {
    int n = coins.size();
    return fun(n-1, amount, coins);
}

/*
TC = O(N*T)
SC = O(N*T) + O(target)  //  + ASS
*/
int fun_dp(int ind, int t, vector<int>& coins, vector<vector<long>> &dp) {
    if (t == 0) return 1;
    

    if (ind == 0) {
        return (t % coins[0] == 0) ? 1 : 0;
    }
    if (dp[ind][t] != -1) return dp[ind][t];  

    long notTake = fun_dp(ind-1, t, coins, dp);
    long take = 0;
    if (t >= coins[ind]) take = fun_dp(ind, t-coins[ind], coins, dp);

    return dp[ind][t] = notTake + take;
}

int change_dp(int amount, vector<int>& coins) {
    int n = coins.size();
    vector<vector<long>> dp(n, vector<long>(amount + 1, -1));
    return fun_dp(n-1, amount, coins, dp);
}

int main() {

    vector<int> coins = {2};  
    int amount = 3;  
    cout << change_dp(amount, coins) << endl;  

    vector<int> coins2 = {1, 2, 5};
    int amount2 = 5;
    cout << change_dp(amount2, coins2) << endl;  // Expected output: 4 (ways to make 5)

    return 0;
}