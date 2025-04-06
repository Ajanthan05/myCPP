#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/*
TC = O(2^T * K)

*/
void f(int ind, int target, vector<vector<int>> &ans, vector<int>& arr, int n, vector<int> &comb) {
      if (ind == n) {
        if (target == 0) {
            ans.push_back(comb);
            // return;
        }
        return;
      }
    if (arr[ind] <= target) {
        comb.push_back(arr[ind]);
        f(ind,target - arr[ind], ans, arr, n, comb);
        comb.pop_back();
    }

    f(ind + 1,target, ans, arr, n, comb);
}

vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    int n = candidates.size();
    vector<vector<int>> ans;
    vector<int> comb;
    f(0,target, ans, candidates, n, comb);
    return ans;
}


/* 40. Combination Sum II
find all unique combinations in candidates where the candidate numbers sum to target.

Each number in candidates may only be used once in the combination.

Note: The solution set must not contain duplicate combinations.
Input: candidates = [10,1,2,7,6,1,5], target = 8
Output: 
[
[1,1,6],
[1,2,5],
[1,7],
[2,6]
]

*/
void f(int ind, int target, vector<vector<int>> &ans, vector<int>& arr, int n, vector<int> &comb) {
    if (target == 0) {
        ans.push_back(comb);
        return;
    }

    for(int i=ind; i<n; i++) {
        if (i > ind && arr[i] == arr[i-1]) continue;
        if (arr[i] > target) break;

        comb.push_back(arr[i]);
        f(i + 1,target - arr[i], ans, arr, n, comb);
        comb.pop_back();
    }
}

vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
    int n = candidates.size();
    vector<vector<int>> ans;
    vector<int> comb;
    f(0,target, ans, candidates, n, comb);
    return ans;  
}


int main() {


    return 0;
}