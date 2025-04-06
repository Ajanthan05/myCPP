#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* SPACE OPTIMIZE 
TC = (N*2*3)
SC = (1)*/
int maxProfit(int k, vector<int>& prices) {
    int n = prices.size();
    vector<vector<int>> after(2, vector<int>(k+1, 0));
    vector<vector<int>> cur(2, vector<int>(k+1, 0));

    for(int ind= n-1; ind>=0; ind--) {
        for(int buy=0; buy<=1; buy++) {
            for(int cap=1; cap<=k; cap++) { // all cap=0  is dp = 0
                int profit = 0;
                if (buy) {
                    profit = max(-prices[ind] + after[0][cap],
                                            0 + after[1][cap]);
                }
                else {
                    profit = max( prices[ind] + after[1][cap - 1],
                                            0 + after[0][cap]);
                }
                cur[buy][cap] = profit;
            }
        }
        after = cur;
    }
    return after[1][k];
}


/*
Method - II
*/
int f(int ind, int tranNo, int k, vector<int>& prices, int n, vector<vector<int>> &dp) {
    if(ind == n || tranNo == 2*k) return 0;

    if(dp[ind][tranNo] != -1) return dp[ind][tranNo];

    // buy
    if (tranNo % 2 == 0) {
        return dp[ind][tranNo] = max(-prices[ind] + f(ind+1, tranNo+1, k, prices, n, dp), 
                             0  + f(ind+1, tranNo, k, prices, n, dp));
    }
    else {
        return dp[ind][tranNo] = max(prices[ind] + f(ind+1, tranNo+1, k, prices, n, dp), 
                            0  + f(ind+1, tranNo, k, prices, n, dp));
    }
}

int maxProfit(int k, vector<int>& prices) {
    int n = prices.size();
    vector<vector<int>> dp(n, vector<int>(2*k, -1));
    return f(0, 0, k, prices, n, dp);
}

/* TABULATION*/
int maxProfit(int k, vector<int>& prices) {
    int n = prices.size();
    vector<vector<int>> dp(n + 1, vector<int>(2*k + 1, 0));

    for(int ind=n-1; ind>=0; ind--) {
        for(int tranNo=2*k-1; tranNo>=0; tranNo--) {
            // buy
            if (tranNo % 2 == 0) {
                dp[ind][tranNo] = max(-prices[ind] + dp[ind+1][tranNo+1], 
                                    0  + dp[ind+1][tranNo]);
            }
            else {
                dp[ind][tranNo] = max(prices[ind] + dp[ind+1][tranNo+1], 
                                    0  + dp[ind+1][tranNo]);
            }
        }
    }
    return dp[0][0];
}

int maxProfit(int k, vector<int>& prices) {
    int n = prices.size();
    vector<int> after(2*k + 1, 0);
    vector<int> cur(2*k + 1, 0);

    for(int ind=n-1; ind>=0; ind--) {
        for(int tranNo=2*k-1; tranNo>=0; tranNo--) {
            // buy
            if (tranNo % 2 == 0) {
                cur[tranNo] = max(-prices[ind] + after[tranNo+1], 
                                    0  + after[tranNo]);
            }
            else {
                cur[tranNo] = max(prices[ind] + after[tranNo+1], 
                                    0  + after[tranNo]);
            }
        }
        after = cur;
    }
    return cur[0];
}
        
////////////////////////////////
/* Buy and Sell Stocks With Cooldown */
int fun(int ind, int buy, vector<int>& prices, int n)  {

    if (ind >= n) return 0; // if u dont sell it in the end of the day u won't get profit

    if (buy) {
        return max( -prices[ind] + fun(ind+1, 0, prices, n), // take
                                0 + fun(ind+1, 1, prices, n));        // Not take
    }
    else {
        return max(prices[ind] + fun(ind + 2, 1, prices, n), // +2 for cooldown
                               0 + fun(ind+1, 0, prices, n));
    }
}

int maxProfit(vector<int>& prices) {
    int n = prices.size();
    return fun(0, 1, prices, n);
}

/* TABULATION */
int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<vector<int>> dp(n + 2, vector<int>(2, 0));

    for(int ind=n-1; ind>=0; ind--) {
        for(int buy=0; buy<=1; buy++) {
            if (buy) {
                dp[ind][buy] =  max( -prices[ind] + dp[ind+1][0], // take
                                       0 + dp[ind+1][1]);        // Not take
            }
            else {
                dp[ind][buy] =  max(prices[ind] + dp[ind + 2][1], // +2 for cooldown
                                     0 + dp[ind+1][0]);
            } 
        }
    }
    return dp[0][1];
}

/* SPACE OPTIMIZE */
int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<int> front2(2, 0);
    vector<int> front1(2, 0);
    vector<int> cur(2, 0);

    for(int ind=n-1; ind>=0; ind--) {
        for(int buy=0; buy<=1; buy++) {
            // if (buy) {
            cur[1] =  max( -prices[ind] + front1[0], // take
                                    0 + front1[1]);        // Not take
        
            cur[0] =  max(prices[ind] + front2[1], // +2 for cooldown
                                    0 + front1[0]);
        }
        front2 = front1;
        front1 = cur;
    }
    return cur[1];
}


///////////////////////////
/*  Buy and Sell Stocks With Transaction Fee    */
int maxProfit(vector<int>& prices, int fee) {
    int n = prices.size();
    vector<int> prev(2, 0), cur(2, 0);

    for(int ind = n - 1; ind >= 0; ind--) {
        for(int buy = 0; buy <= 1; buy++) {
            cur[1] = max(-prices[ind] - fee + prev[0],  // Buy
                            0 + prev[1]);          // Skip buying
            cur[0] = max(prices[ind] + prev[1],   // Sell
                            0 + prev[0]);          // Skip selling
        }
        prev = cur;  // Move to the next state
    }

    return prev[1];  // Max profit when buying is allowed at day 0
}

int main() {


    return 0;
}