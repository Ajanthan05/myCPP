#include <iostream>
#include <vector>
#include <climits>

class Solution {
public:

    int First_Occurrences(std::vector<int>& nums, int target, int n) {
        int low = 0;
        int high = n - 1;
        int mid;
        int first = -1; // Initialize with -1 for "not found"

        while (low <= high) {
            mid = low + (high - low) / 2; // Prevent overflow

            if (target == nums[mid]) {
                first = mid;
                high = mid - 1; // Move left to find earlier occurrence
            }
            else if (target > nums[mid]) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }
        return first;
    }

    int Last_Occurrences(std::vector<int>& nums, int target, int low, int high) {
        int mid;
        int last = -1; // Initialize with -1 for "not found"

        while (low <= high) {
            mid = low + (high - low) / 2; // Prevent overflow

            if (target == nums[mid]) {
                last = mid;
                low = mid + 1; // Move right to find later occurrence
            }
            else if (target > nums[mid]) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }
        return last;
    }

    std::vector<int> searchRange(std::vector<int>& nums, int target) {
        int n = nums.size();
        if (n == 0) return {-1, -1}; // Edge case: empty vector

        int first = First_Occurrences(nums, target, n);
        if (first == -1) return {-1, -1}; // Target not found

        // Call Last_Occurrences with the range starting from `first`
        int last = Last_Occurrences(nums, target, first, n - 1);
        return {first, last};
    }
};

int searchInRotatedSortedArray(std::vector<int>& arr, int target) {
    int n = arr.size();
    int low = 0;
    int high = n-1;
    int mid;

    while(low <= high) {
        mid = low + (high - low) / 2;
        if (arr[mid] == target) return mid;

        // here always left or write will be sorted

        // left sorted
        if (arr[low] <= arr[mid]) {
            if (arr[low] <= target && target <= arr[mid]) {
                high = mid-1;
            }
            else {
                low = mid + 1;
            }
        }
        // right sorted
        else {
            if (arr[mid] <= target && target <= arr[high]) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }
    }

    // If not found 
    return -1;
}


int main() {

    Solution obj1;
    std::vector<int> arr = {3,4,6,7,9,12,12,12,12,12,16,17}; 
    std::vector<int> ans = obj1.searchRange(arr, 12);
    std::cout << "Direct_First_and_Last_Occurrences: " << ans[0] << ", " << ans[1] << "\n";

    std::cout << "\n";
    std::vector<int> arr2 = {7,8,9,1,2,3,4,5,6};
    return 0;
}