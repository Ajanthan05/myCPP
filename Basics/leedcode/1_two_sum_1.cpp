#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

/*
TC = O(N*logN)
When using unorderde map
TC = O(N) // worst casr it goes upto O(N^2)
SC = O(N)
*/
std::vector<int> twoSum(std::vector<int>& nums, int target) {
    std::map<int, int> mpp;
    for (int i=0; i<nums.size(); i++) {
        int more = target - nums[i];
        if (mpp.find(more) != mpp.end()) return {mpp[more], i};
        mpp[nums[i]] = i;
    }
    return {0,0};
}

/*
TC = O(N) + O(N*logN) // for sorting
SC = O(1)  // sorting requir distorted SC of O(N)
*/
std::vector<int> twoSum2(std::vector<int>& nums, int target) {
    int r = nums.size() -1;
    int l = 0;

    while (l < r) {
        if (target == (nums[l] + nums[r])) return {l, r}; 
        else if (target < (nums[l] + nums[r])) r--;
        else l++;
    }
    return {0,0};
}

int main() {

    std::vector<int> nums = {2,7,11,15};
    int target = 9;

    std::vector<int> ans = twoSum(nums, target);
    for (auto it : ans) {
        std::cout << it << " ";
    }
    std::cout << "\n";

    std::sort(nums.begin(), nums.end());
    std::vector<int> ans_sort = twoSum2(nums, target);
    for (auto it : ans_sort) {
        std::cout << it << " ";
    }
    std::cout << "\n";

    return 0;
}