#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int numSubarraysWithSum(vector<int>& nums, int goal) {
    int l = 0;
    int n = nums.size();
    int sum = 0;
    int count = 0;

    for(int r=0; r<n; r++) {
        sum += nums[r];
        count++;

        if (sum < goal) {
            count--;
            while(sum < goal) {
                sum -= nums[l];
                if (l >= n) break;
                count++;
                l--;
            }
        }
    }
    return count;
}

int main() {
    vector<int> nums = {1,0,1,0,1};
    cout << numSubarraysWithSum(nums, 2) << "/n";

    return 0;
}