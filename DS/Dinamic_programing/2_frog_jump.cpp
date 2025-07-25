#include <iostream>
#include <vector>
#include <climits>
/*
1. Express the problem in terms of index
2. Do all stuffs on that index
3. Take the min of all stuffs

f(n-1) => mimimum energy requir to reach (n-1) from 0

*/


int frogJump(int ind, std::vector<int> &heights)
{
    if (ind == 0) return 0;
    int left = frogJump(ind-1, heights) + abs(heights[ind] - heights[ind-1]);
    int right = INT_MAX;
    if (ind > 1) right = frogJump(ind-2, heights) + abs(heights[ind] - heights[ind-2]);
    return std::min(left, right);
}

/* Recursion to DP */
int frogJump_dp(int ind, std::vector<int> &heights, std::vector<int> &dp)
{
    if (ind == 0) return 0;
    if (dp[ind] != -1) return dp[ind];
    int left = frogJump_dp(ind-1, heights) + abs(heights[ind] - heights[ind-1]);
    int right = INT_MAX;
    if (ind > 1) right = frogJump_dp(ind-2, heights) + abs(heights[ind] - heights[ind-2]);
    return dp[ind] = std::min(left, right);
}



int main() {

    std::vector<int> heights = {30, 10, 60, 10, 60, 50};
    int n = 6;
    int min = frogJump(n-1, heights);
    std::cout << min << "\n";

    
    std::vector<int> heights2 = {10, 20, 30, 10};
    int n2 = 4;
    std::vector<int> dp2(n2+1, -1);
    int min2 = frogJump_dp(n2-1, heights2, dp2);
    std::cout << min2 << "\n";

/* TABULATION 
We are redusing the stack space but we still use dp[n] array  SC = O(N)
*/
    std::vector<int> heights3 = {10, 20, 30, 10};
    int n3 = 4;
    std::vector<int> dp3(n3, 0);

    dp3[0] = 0;
    for (int i=1; i<n3; i++) {
        int fs = dp3[i-1] + abs(heights3[i] - heights3[i-1]);
        int second_step = INT_MAX;
        if(i>1) second_step = dp3[i-2] + abs(heights3[i] - heights3[i-2]);
        dp3[i] = std::min(fs, second_step);
    }

    std::cout << dp3[n3-1] << "\n";


    /* SPACE OPTIMIZATION */

    std::vector<int> heights4 = {10, 20, 30, 10};
    int n4 = 4;

    int prev = 0;    // dp4[0] = 0;
    int prev2 = 0;
    for (int i=1; i<n4; i++) {
        int fs = prev + abs(heights4[i] - heights4[i-1]);
        int second_step = INT_MAX;
        if(i>1) second_step = prev2 + abs(heights4[i] - heights4[i-2]);
        int curi = std::min(fs, second_step);
        prev2 = prev;
        prev = curi;
    }

    std::cout << prev << "\n";

    return 0;
}