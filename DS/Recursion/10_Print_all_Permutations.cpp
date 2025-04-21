#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

void returnPermute(vector<int> &ds, vector<int>& nums, vector<vector<int>> &ans, vector<bool> &freq, int n) {
    if (ds.size() == n) {
        ans.push_back(ds);
        return;
    }

    for(int i=0; i<n; i++) {
        if (!freq[i]) {
            ds.push_back(nums[i]);
            freq[i] = true;
            returnPermute(ds, nums, ans, freq, n);
            ds.pop_back();
            freq[i] = false;
        }
    }
}

vector<vector<int>> permute(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> ans;  
    vector<int> ds; 
    vector<bool> freq(n, 0);
    returnPermute(ds, nums, ans, freq, n);
    return ans;
}


/* 2nd Approach*/
void returnPermute(int ind, vector<int>& nums, vector<vector<int>> &ans, int n) {
    if (ind == n) {
        ans.push_back(nums);
        return;
    }
    
    for(int i=ind; i<n; i++) {
        swap(nums[ind], nums[i]);
        returnPermute(ind + 1, nums, ans, n);
        swap(nums[ind], nums[i]);
    }
}

vector<vector<int>> permute(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> ans;  
    returnPermute(0, nums, ans, n);
    return ans;
}

int main() {


    return 0;
}