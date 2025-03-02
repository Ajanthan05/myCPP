#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
    int n = nums.size();
    sort(nums.begin(), nums.end());
    int i=0, j=1, k=n;
    int sum = 0;
    vector<vector<int>> ans;

    while(i <= j && j <= k) {
        sum = nums[i] + nums[j] + nums[k];
        if (sum == 0) {
            ans.push_back({i,j,k});

            while(nums[j] == nums[j+1]) {
                i++;
            }
        
            while(nums[k] == nums[k-1]) {
                k--;
            }
        }
        else if (sum < 0) {

            while(nums[i] == nums[i+1]) {
                i++;
            }
            j = j+1;
        }
    }
    return ans;
}

int main() {


    return 0;
}