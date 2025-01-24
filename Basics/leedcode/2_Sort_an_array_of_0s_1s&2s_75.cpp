#include <iostream>
#include <vector>
#include <climits>

/*
TC = O(N)
SC = O(1)
*/
void sortColors( std::vector<int>& nums) {
    int low =0, mid = 0, high = nums.size()-1;
    while (mid <= high) {
        if (nums[mid] == 0) {
            std::swap(nums[low], nums[mid]);
            low++;
            mid++;
        }
        else if (nums[mid] == 1) {
            mid++;
        }
        else if (nums[mid] == 2) {
            std::swap(nums[high], nums[mid]);
            high--;
        }
    }
}
int main() {

    std::vector<int> nums = {2,0,2,1,1,0};
    sortColors(nums);
    for (auto it : nums) {
        std::cout << it << " ";
    }
    std::cout << "\n";
    return 0;
}