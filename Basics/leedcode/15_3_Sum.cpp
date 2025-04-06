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

// vector<vector<int>> threeSum(vector<int>& nums) {
//     int n = nums.size();
//     sort(nums.begin(), nums.end());
//     int i=0, j=1, k=n;
//     int sum = 0;
//     vector<vector<int>> ans;

//     for(int i=0; i<n; i++) {
//         if (i>0 && nums[i] != nums[i-1]) {
//             4edfc
//         }
//     }

//     return ans;
// }

int main() {
    vector<int> nums = {-1,0,1,2,-1,-4};
    vector<vector<int>> ans = threeSum(nums);
    for (auto it_c : ans) {
        for (auto it_r : it_c) {
            cout << it_r << ", ";
        }
    }
    return 0;
}