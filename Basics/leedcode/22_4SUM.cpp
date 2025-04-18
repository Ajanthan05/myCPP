#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

vector<vector<int>> fourSum(vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<int>> ans;
    if (nums.empty()) return ans;

    sort(nums.begin(), nums.end());

    for(int i=0; i<n - 3; i++) {
        for(int j=i + 1; j<n - 2; j++) {
            int left  = j + 1;
            int right = n-1;
            // int remain = target - nums[i] - nums[j];
            long long remain = (long long)target - nums[i] - nums[j]; // Use long long to prevent overflow
            // Using long long for remain avoids integer overflow when target is large (e.g. INT_MAX, INT_MIN).
            
            while(left < right) {
                int two_sum = nums[left] + nums[right];

                if (two_sum < remain) left++;

                else if (two_sum > remain) right--;

                else {      // if (remain == two_sum)
                    ans.push_back({nums[i], nums[j], nums[left], nums[right]});

                    while(left < right && nums[left] == nums[left+1]) left++;
                    while(left < right && nums[right] == nums[right-1]) right--;
                    left++;
                    right--;
                }
            }
            while(j+1 < n-2 && nums[j] == nums[j+1]) ++j;
        }
        while(i+1 < n-3 && nums[i] == nums[i+1]) ++i;
    }
    return ans;
}

int main() {


    return 0;
}