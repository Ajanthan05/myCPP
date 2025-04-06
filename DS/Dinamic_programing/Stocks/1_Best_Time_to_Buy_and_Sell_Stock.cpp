#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int maxProfit(vector<int>& prices) {
    int mini = prices[0];
    int maxProfit = 0;
    for(int i=0; i<prices.size(); i++) {
        int cost = prices[i] - mini;
        maxProfit = max(maxProfit, cost);
        mini = min(mini, prices[i]);
    } 
    return maxProfit;
}

/*  Buy and Sell Stock - II 
after u buy u need to sell before buy
*/
int fun(int ind, int buy, vector<int>& prices)  {

    if (ind == prices.size()) return 0; // if u dont sell it in the end of the day u won't get profit

    int profit = 0;
    if (buy) {
        profit = max( -prices[ind] + fun(ind+1, 0, prices), // take
                                0 + fun(ind+1, 1, prices));        // Not take
    }
    else {
        profit = max(prices[ind] + fun(ind+1, 1, prices),
                               0 + fun(ind+1, 0, prices));
    }
    return profit;
}

int maxProfit(vector<int>& prices) {
    int n = prices.size();
    return fun(0, 1, prices);
}

/* MEMOIZATION 
TC = O(N*2)
SC = O(N*2) + O(N) // auxalary space 
*/
int fun(int ind, int buy, vector<int>& prices, vector<vector<int>> &dp)  {

    if (ind == prices.size()) return 0; // if u dont sell it in the end of the day u won't get profit

    if (dp[ind][buy] != -1) return dp[ind][buy];

    int profit = 0;
    if (buy) {
        profit = max( -prices[ind] + fun(ind+1, 0, prices, dp), // take
                                0 + fun(ind+1, 1, prices, dp));        // Not take
    }
    else {
        profit = max(prices[ind] + fun(ind+1, 1, prices, dp),
                               0 + fun(ind+1, 0, prices, dp));
    }
    return dp[ind][buy] = profit;
}

int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<vector<int>> dp(n, vector<int>(2, -1));
    return fun(0, 1, prices, dp);
}

/*TABULATION

*/
int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<vector<int>> dp(n+1, vector<int>(2, 0));

    // BASE CASE
    // if (ind == prices.size()) return 0;
    // dp[n][0] = dp[n][1] = 0;

    for(int ind=n-1; ind>=0; ind--) {
        for(int buy=0; buy<=1; buy++) {
            int profit = 0;
            if (buy) {
                profit = max( -prices[ind] + dp[ind+1][0], // take
                                        0 + dp[ind+1][1]);        // Not take
            }
            else {
                profit = max(prices[ind] + dp[ind+1][1],
                                       0 + dp[ind+1][0]);
            }
            dp[ind][buy] = profit;
        }
    }

    return dp[0][1];
}

/* SOACE OPTIMIZE */
int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<int> prev(2, 0), cur(2, 0);

    for(int ind = n - 1; ind >= 0; ind--) {
        for(int buy = 0; buy <= 1; buy++) {
            if (buy) {
                cur[buy] = max(-prices[ind] + prev[0],  // Buy
                                0 + prev[1]);          // Skip buying
            } else {
                cur[buy] = max(prices[ind] + prev[1],   // Sell
                                0 + prev[0]);          // Skip selling
            }
        }
        prev = cur;  // Move to the next state
    }

    return prev[1];  // Max profit when buying is allowed at day 0
}


/*      III     */
int fun3(int ind, int buy, int cap, vector<int>& prices)  {

    // BASE CASE
    if (cap == 0 || ind == prices.size()) return 0;

    int profit = 0;
    if (buy) {
        profit = max(-prices[ind] + fun3(ind + 1, 0, cap, prices),
                                0 + fun3(ind + 1, 1, cap, prices));
    }
    else {
        profit = max( prices[ind] + fun3(ind + 1, 1, cap - 1, prices),
                                0 + fun3(ind + 1, 0, cap, prices));
    }
    return profit;
}

int maxProfit(vector<int>& prices) {
    int n = prices.size();
    // vector<vector<int>> dp(n, vector<int>(2, -1));
    return fun3(0, 1, 2, prices);
}

/* */
int fun3(int ind, int buy, int cap, vector<int>& prices, vector<vector<vector<int>>> &dp)  {

    // BASE CASE
    if (cap == 0 || ind == prices.size()) return 0;

    if (dp[ind][buy][cap] != -1) return dp[ind][buy][cap];

    int profit = 0;
    if (buy) {
        profit = max(-prices[ind] + fun3(ind + 1, 0, cap, prices, dp),
                                0 + fun3(ind + 1, 1, cap, prices, dp));
    }
    else {
        profit = max( prices[ind] + fun3(ind + 1, 1, cap - 1, prices, dp),
                                0 + fun3(ind + 1, 0, cap, prices, dp));
    }
    return dp[ind][buy][cap] = profit;
}

int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<vector<vector<int>>> dp(n, vector<vector<int>>(2, vector<int>(3, -1)));
    return fun3(0, 1, 2, prices, dp);
}

/* TABULATION 
TC = (N*2*3)
SC = (N*2*3)
*/
int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(2, vector<int>(3, 0)));

    for(int ind= n-1; ind>=0; ind--) {
        for(int buy=0; buy<=1; buy++) {
            for(int cap=1; cap<=2; cap++) { // all cap=0  is dp = 0
                int profit = 0;
                if (buy) {
                    profit = max(-prices[ind] + dp[ind + 1][0][cap],
                                            0 + dp[ind + 1][1][cap]);
                }
                else {
                    profit = max( prices[ind] + dp[ind + 1][1][cap - 1],
                                            0 + dp[ind + 1][0][cap]);
                }
                dp[ind][buy][cap] = profit;
            }
        }
    }
    return dp[0][1][2];
}

/* SPACE OPTIMIZE 
TC = (N*2*3)
SC = (1)*/
int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<vector<int>> after(2, vector<int>(3, 0));
    vector<vector<int>> cur(2, vector<int>(3, 0));

    for(int ind= n-1; ind>=0; ind--) {
        for(int buy=0; buy<=1; buy++) {
            for(int cap=1; cap<=2; cap++) { // all cap=0  is dp = 0
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
    return after[1][2];
}
int main() {


    return 0;
}