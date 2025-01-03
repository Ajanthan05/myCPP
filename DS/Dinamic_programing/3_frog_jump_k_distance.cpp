#include <iostream>
#include <vector>
#include <climits>



int frogJump(int ind, std::vector<int> &heights, int k)
{
    if (ind == 0) return 0;
    int minimumStep = INT_MAX;

    for (int j=1; j<=k; j++) {
        int jump = INT_MAX;
        if (ind >= j) jump = frogJump(ind-j, heights, k) + abs(heights[ind] - heights[ind-j]);
        minimumStep = std::min(minimumStep, jump);
    }
    return minimumStep;
}

/* MEMIOZATION 
TC = O(N)*K
SC = O(N) + O(N)  // STACK_SPACE + DP
*/

void frogJump_dp(int n, std::vector<int> &heights, int k, std::vector<int> &dp)
{
    dp[0] = 0;
    // if(dp[ind] != -1) return dp[ind];

    for (int i=1; i<n; i++) {
        int minimumStep = INT_MAX;

        for (int j=1; j<=k; j++) {
            int jump = INT_MAX;
            if (i >= j) jump = dp[i-j] + abs(heights[i] - heights[i-j]);
            minimumStep = std::min(minimumStep, jump);
        }
        dp[i] = minimumStep;
    }
    std::cout << dp[n-1] << "\n";
}

/* SPACE OPTIMIZE */

void frogJump_SO(int n, std::vector<int> &heights, int k, std::vector<int> &dp)
{
    dp[0] = 0;
    for (int l)
    for (int i=1; i<n; i++) {
        int minimumStep = INT_MAX;

        for (int j=1; j<=k; j++) {
            int jump = INT_MAX;
            if (i >= j) jump = dp[i-j] + abs(heights[i] - heights[i-j]);
            minimumStep = std::min(minimumStep, jump);
        }
        dp[i] = minimumStep;
    }
    std::cout << dp[n-1] << "\n";
}

int main() {

    std::vector<int> heights = {30, 10, 60, 10, 60, 50};
    int n = 6;
    std::vector<int> dp(n+1, -1);
    int min = frogJump(n-1, heights, 2);
    std::cout << min << "\n";

    std::vector<int> heights2 = {30, 10, 60, 10, 60, 50};
    int n2 = 6;
    std::vector<int> dp2(n2+1, -1);
    frogJump_dp(n2, heights2, 2, dp2);

    return 0;
}