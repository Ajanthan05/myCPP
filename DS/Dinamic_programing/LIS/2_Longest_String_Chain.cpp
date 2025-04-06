#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

bool checkPossible(string &s1, string &s2) {
    if (s1.size()+1 < s2.size()) return -1;
    int first = 0;
    int second = 0;

    while(first < s1.size()) {
        if (s1[first] == s2[second]) {
            first++;
            second++;
        }
        else {
            first++;
        }

        if (first == s1.size() && second == s2.size()) return true;
        else return false;
    }
}

bool comp(string &s1, string &s2) {
    return s1.size() < s2.size();
}

int longestStrChain(vector<string>& words) {
    int n = words.size();
    vector<int> dp(n, 1), hash(n);
    int maxi = 1;
    int maxi_ind = 0;
    sort(words.begin(), words.end(), comp);

    for(int i=0; i<n; i++) {
        hash[i] = i; // When ever enter, its initial val
        for(int prev=0; prev<i; prev++) {
            if (checkPossible(words[i], words[prev]) && dp[i] < 1 + dp[prev]) {
                dp[i] = 1 + dp[prev];
                hash[i] = prev; // Store the previous index for LIS
            }
        }
        if (maxi < dp[i]) {
            maxi = dp[i];
            maxi_ind = i;
        }
    }
    // vector<int> lis(maxi);
    // lis[--maxi] = words[maxi_ind];

    // while(hash[maxi_ind] != maxi_ind) {
    //     maxi_ind = hash[maxi_ind];
    //     lis[--maxi] = words[maxi_ind];
    // }

    return maxi;
}


/*  Longest Bitonic Subsequence */
int longestBitonicSubsequence(vector<int>& arr)
{
	int n = arr.size();
    vector<int> dp1(n, 1), dp2(n, 1);

    for(int i=0; i<n; i++) {
        for(int prev=0; prev<i; prev++) {
            if (arr[prev] < arr[i] && 1 + dp1[prev] > dp1[i]) {
                dp1[i] = 1 + dp1[prev];
            }
        }
    }

    int maxi = 0;
    for(int i=n-1; i>=0; i--) {
        for(int prev=n-1; prev>i; prev--) {
            if (arr[prev] < arr[i] && 1 + dp2[prev] > dp2[i]) {
                dp2[i] = 1 + dp2[prev];
            }
        }
        maxi = max(maxi, dp1[i] + dp2[i] - 1);
    }

    // int maxi = 0;
    // for(int i=0; i<n; i++) {
    //     maxi = max(maxi, dp1[i] + dp2[i] - 1);
    // }
    return maxi;
}

/////////////////////////////////////
/* Key Fixes
Ensured the peak is not at the first or last index: The loop for (int i = 1; i < n - 1; i++) ensures that a valid peak exists.

Checked dp1[i] > 1 and dp2[i] > 1: This ensures that i is a valid peak (i.e., it has both an increasing and a decreasing subsequence).

Kept the formula dp1[i] + dp2[i] - 1: This correctly calculates the length of the bitonic subsequence.*/
int longestBitonicSubsequence(vector<int>& arr) {
    int n = arr.size();
    vector<int> dp1(n, 1), dp2(n, 1);

    // Compute LIS (Longest Increasing Subsequence) from left to right
    for(int i = 0; i < n; i++) {
        for(int prev = 0; prev < i; prev++) {
            if (arr[prev] < arr[i] && 1 + dp1[prev] > dp1[i]) {
                dp1[i] = 1 + dp1[prev];
            }
        }
    }

    // Compute LIS from right to left (Longest Decreasing Subsequence)
    for(int i = n - 1; i >= 0; i--) {
        for(int prev = n - 1; prev > i; prev--) {
            if (arr[prev] < arr[i] && 1 + dp2[prev] > dp2[i]) {
                dp2[i] = 1 + dp2[prev];
            }
        }
    }

    // Find the longest bitonic subsequence ensuring it's a valid mountain
    int maxi = 0;
    for (int i = 1; i < n - 1; i++) {  // Ensure peak is not at the start or end
        if (dp1[i] > 1 && dp2[i] > 1) {  // Ensure valid peak
            maxi = max(maxi, dp1[i] + dp2[i] - 1);
        }
    }
    
    return maxi;
}

int minimumMountainRemovals(vector<int>& nums) {
    return nums.size() - longestBitonicSubsequence(nums);
}

/* Number of Longest Increasing Subsequences */
int findNumberOfLIS(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n, 1), cnt(n, 1);
    int maxi = 1;
    for(int i=0; i<n; i++) {
        for(int prev=0; prev<i; prev++) {
            if(nums[prev] < nums[i] && 1 + dp[prev] > dp[i]) {
                dp[i] = 1 + dp[prev];
                // Inherit
                cnt[i] = cnt[prev];
            }
            else if(nums[prev] < nums[i] && 1 + dp[prev] == dp[i]) {
                // increase count
                cnt[i] += cnt[prev];
            }
        }
        if (dp[i] > maxi) {
            maxi = dp[i];
        }
    }
    int nos = 0;
    for(int i=0; i<n; i++) {
        if (dp[i] == maxi) nos += cnt[i];
    }
    return nos;
}
int main() {


    return 0;
}