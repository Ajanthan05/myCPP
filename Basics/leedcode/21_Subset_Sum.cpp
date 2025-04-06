#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;
/*
TC = O(2^N) + 2^Nlog(2^N)//sorting size 
SC = 
*/
void f(int ind, int sum, vector<int>& sums, int n, vector<int> &ans) {
    if (ind == n) {
        ans.push_back(sum);
        return;
    }

    // notPick 
    f(ind+1, sum, sums, n, ans);
    // Pick 
    f(ind+1, sum + sums[ind], sums, n, ans);
}
vector<int> subsetSum(vector<int> &sums){
    int n = sums.size();
    vector<int> ans;
    f(0, 0, sums, n, ans);
    sort(ans.begin(), ans.end());
    return ans;
}

/*Given an integer array nums that may contain duplicates, return all possible subsets (the power set).

The solution set must not contain duplicate subsets. Return the solution in any order.
*/

void findSubsets(int ind, vector<int>& nums,  vector<int> &ds, vector<vector<int>> &ans, int n) {
    ans.push_back(ds);
    for(int i=ind; i<nums.size(); i++) {
        if (i != ind && nums[i] == nums[i-1]) continue;
        ds.push_back(nums[i]);
        findSubsets(i+1, nums, ds, ans, n);
        ds.pop_back();
    }
}

vector<vector<int>> subsetsWithDup(vector<int>& nums) {
    vector<vector<int>> ans;
    vector<int> ds;
    sort(nums.begin(), nums.end());
    int n = nums.size();
    findSubsets(0, nums, ds, ans, n);
    return ans;    
}

int main() {


    return 0;
}