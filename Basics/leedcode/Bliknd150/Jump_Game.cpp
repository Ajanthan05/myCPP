#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

bool canJump(vector<int>& nums) {
    int n = nums.size();
    int maxInd = 0;
    for(int i=0; i<n; i++) {
        if(i > maxInd) return false;
        maxInd = max(maxInd, i + nums[i]);
    }
    return true;
}

////////////////////    JUMP 2      //////////////////
int f(int ind, int jumps, vector<int>& nums, int n) {
    if(ind >= n-1) return jumps;
    int mini = INT_MAX;
    for(int i=0; i<n; i++) {
        mini = min(mini, f(ind + i, jumps + 1, nums, n));
    }
    return mini;
}
int jump(vector<int>& nums) {
    int n = nums.size();
    return f(0, 0, nums, n);
}



int f(int ind, vector<int>& nums, vector<int>& dp, int n) {
    if (ind >= n - 1) return 0; // Already at or beyond the end
    if (dp[ind] != -1) return dp[ind];

    int minJumps = INT_MAX;
    int maxJump = nums[ind];

    for (int j = 1; j <= maxJump; ++j) {
        if (ind + j < n) {  //  if you accidentally use nums[ind + j] without checking, that will cause out-of-bounds access and undefined behavior.
            int res = f(ind + j, nums, dp, n);
            if (res != INT_MAX)         // Prevent overflow
                minJumps = min(minJumps, 1 + res);
        }
    }

    return dp[ind] = minJumps;
}

int jump(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n, -1);
    return f(0, nums, dp, n);
}

int jump_o(vector<int>& nums) {
    int n = nums.size();
    int jumps = 0;
    int l = 0, r = 0;
    while(r < n-1) {
        int farthest = 0;
        for(int ind = l; ind <= r; ind++) {
            farthest = max(farthest, ind + nums[ind]);
        }
        l = r+1;
        r = farthest;
        jumps++;
    }
    return jumps;
}


/*  1306. Jump Game III
Given an array of non-negative integers arr, you are initially positioned at start 
index of the array. When you are at index i, you can jump to i + arr[i] or i - arr[i], 
check if you can reach any index with value 0.*/

int rec(vector<int>& arr, int start) {

}

bool canReach(vector<int>& arr, int start) {
    int n = arr.size();
}


int main() {


    return 0;
}