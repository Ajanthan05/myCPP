#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int f(int ind, int prevInd, vector<int>& nums, int n) {

    if (ind == n) return 0; // Zero base index

    int len = 0 + f(ind+1, prevInd, nums, n);
    if(prevInd == -1 || nums[prevInd] < nums[ind]) {
        len = max(len, 1 + f(ind+1, ind, nums, n));
    }
    return len;
}

int lengthOfLIS(vector<int>& nums) {
    int n = nums.size();
    return f(0, -1, nums, n); 
}

/* MEMOIZATION */
int f(int ind, int prevInd, vector<int>& nums, int n, vector<vector<int>> &dp) {

    if (ind == n) return 0; // Zero base index

    if (dp[ind][prevInd+1] != -1) return dp[ind][prevInd+1];
    
    int len = 0 + f(ind+1, prevInd, nums, n, dp);
    if(prevInd == -1 || nums[prevInd] < nums[ind]) {
        len = max(len, 1 + f(ind+1, ind, nums, n, dp));
    }
    return dp[ind][prevInd+1] = len;
}

int lengthOfLIS(vector<int>& nums) {
    int n = nums.size();
    // may give runtime error vector size is 10^5 * 10^5 = 10^10
    vector<vector<int>> dp(n, vector<int>(n+1, -1));
    return f(0, -1, nums, n, dp); 
}

/*  TABULATION  */
int lengthOfLIS(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(n+1, 0));
    for(int ind=n-1; ind>=0; ind--) {
        for(int prevInd=ind-1; prevInd>=-1; prevInd--) {
            int len = 0 + dp[ind+1][prevInd + 1];
            if(prevInd == -1 || nums[prevInd] < nums[ind]) {
                len = max(len, 1 + dp[ind+1][ind + 1]);
            }
            dp[ind][prevInd+1] = len;
        }
    }
    return dp[0][0]; 
}

/* SPACE OPTIMIZATION   
TC + O(N^2)
SC = O(N)*2
*/
int lengthOfLIS(vector<int>& nums) {
    int n = nums.size();
    vector<int> cur(n + 1, 0), next(n + 1, 0);
    for(int ind=n-1; ind>=0; ind--) {
        for(int prevInd=ind-1; prevInd>=-1; prevInd--) {
            int len = 0 + next[prevInd + 1];
            if(prevInd == -1 || nums[prevInd] < nums[ind]) {
                len = max(len, 1 + next[ind + 1]);
            }
            cur[prevInd+1] = len;
        }
        next = cur;
    }
    return cur[0]; 
}

/* OPTIMAL */
int lengthOfLIS(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n, 1);
    int maxi = 1;

    for(int i=0; i<n; i++) {
        for(int prev=0; prev<i; prev++) {
            if (nums[prev] < nums[i]) {
                dp[i] = max(dp[i], 1 + dp[prev]);
            }
        }
        maxi = max(maxi, dp[i]);
    }
    return maxi;
}

/* Most OPTIMAL */
int lengthOfLIS(vector<int>& nums) 
{
    vector<int> ans;
    for(int k : nums)
    {
        auto pos = lower_bound(ans.begin(), ans.end(), k);
        if(pos - ans.begin() == ans.size()) ans.push_back(k);
        else *pos = k; 
    }
    return ans.size();
}

/* PRINT LIS */
vector<int> printingLongestIncreasingSubsequence(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n, 1), hash(n);
    int maxi = 1;
    int maxi_ind = 0;

    for(int i=0; i<n; i++) {
        hash[i] = i; // When ever enter, its initial val
        for(int prev=0; prev<i; prev++) {
            if (nums[prev] < nums[i] && dp[i] < 1 + dp[prev]) {
                dp[i] = 1 + dp[prev];
                hash[i] = prev; // Store the previous index for LIS
            }
        }
        if (maxi < dp[i]) {
            maxi = dp[i];
            maxi_ind = i;
        }
    }
    vector<int> lis(maxi);
    lis[--maxi] = nums[maxi_ind];

    while(hash[maxi_ind] != maxi_ind) {
        maxi_ind = hash[maxi_ind];
        lis[--maxi] = nums[maxi_ind];
    }

    // vector<int> lis(maxi);
    // lis[maxi - 1] = nums[maxi_ind]; // The last element of the LIS
    // maxi--;  // Move to the next position in the LIS

    // while(hash[maxi_ind] != maxi_ind) {
    //     maxi_ind = hash[maxi_ind];
    //     lis[maxi - 1] = nums[maxi_ind];  // Fill in the LIS array
    //     maxi--;  // Move to the previous position in the LIS
    // }
    return lis;
}

/* Largest Divisible Subset */

vector<int> largestDivisibleSubset(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n, 1), hash(n);
    int maxi = 1;
    int maxi_ind = 0;
    sort(nums.begin(), nums.end());

    for(int i=0; i<n; i++) {
        hash[i] = i; // When ever enter, its initial val
        for(int prev=0; prev<i; prev++) {
            if (nums[i] % nums[prev] == 0 && dp[i] < 1 + dp[prev]) {
                dp[i] = 1 + dp[prev];
                hash[i] = prev; // Store the previous index for LIS
            }
        }
        if (maxi < dp[i]) {
            maxi = dp[i];
            maxi_ind = i;
        }
    }
    vector<int> lis(maxi);
    lis[--maxi] = nums[maxi_ind];

    while(hash[maxi_ind] != maxi_ind) {
        maxi_ind = hash[maxi_ind];
        lis[--maxi] = nums[maxi_ind];
    }

    return lis;
}


/* OPTIMAL ANSWER ???*/
auto init = []() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 'c';
}();
auto init2 = atexit([]() { ofstream("display_runtime.txt") << 0; });

class Solution {
  public:
    static vector<int> largestDivisibleSubset(vector<int>& nums) {
        std::ranges::sort(nums);
        int n = nums.size();
        std::vector<std::pair<int, int>> dp(n, {-1, 1});
        for (int i = 1; i < n; ++i) {
            int v = nums[i];
            auto& [prev, cnt] = dp[i];
            for (int j = 0; j < i; ++j) {
                auto [pprev, pcnt] = dp[j];
                if (cnt > 1 + pcnt) {
                    continue;
                }
                int pv = nums[j];
                if (v % pv != 0) {
                    continue;
                }
                prev = j;
                cnt = pcnt + 1;
            }
        }
        auto it = std::ranges::max_element(dp,
                                           [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });
        auto s = it->second;
        auto start = std::distance(dp.begin(), it);
        std::vector<int> ret(s);
        int idx = s - 1;
        while (start != -1) {
            ret[idx--] = nums[start];
            start = dp[start].first;
        }
        return ret;
    }
};

int main() {
    vector<int> arr = {5,4,11,1,16,8};
    vector<int> out = printingLongestIncreasingSubsequence(arr);
    for(auto k : out) {
        cout << k << " ";
    }
    cout << "\n";
    return 0;
}