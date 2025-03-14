#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// int numSubarraysWithSum(vector<int>& nums, int goal) {
//     int l = 0;
//     int n = nums.size();
//     int sum = 0;
//     int count = 0;

//     for(int r=0; r<n; r++) {
//         sum += nums[r];
//         if (sum == goal) count++;

//         if (sum < goal) {
//             count--;
//             while(sum < goal) {
//                 sum -= nums[l];
//                 if (l >= n) break;
//                 count++;
//                 l--;
//             }
//         }
//     }
//     return count;
// }

int atMost(vector<int>& nums, int s) {
    if (s < 0) return 0;  // No subarray can have a negative sum.
    int count = 0;
    int left = 0, sum = 0;
    for (int right = 0; right < nums.size(); right++) {
        sum += nums[right];
        while (sum > s) {
            sum -= nums[left++];
        }
        // Every subarray ending at right with starting index between left and right is valid.
        count += right - left + 1;
    }
    return count;
}

int numSubarraysWithSum(vector<int>& nums, int goal) {
    // The number of subarrays with sum exactly `goal` equals:
    // atMost(goal) - atMost(goal - 1)
    return atMost(nums, goal) - atMost(nums, goal - 1);
}

int main() {
    vector<int> nums = {1,0,1,0,1};
    cout << numSubarraysWithSum(nums, 2) << "/n";

    return 0;
}