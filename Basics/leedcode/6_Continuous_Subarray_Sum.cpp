#include <iostream>
#include <vector>
#include <climits>

// Kadane's Algorithm
long long maxSubArray(vector<int>& nums) {
    long long maxi = LONG_MIN;
    int sum = 0;
    for (int i=0; i<nums.size(); i++) {
        sum += nums[i];

        if (sum > maxi) maxi = sum;

        if (sum < 0) sum = 0;
    }
    if (maxi < 0) return 0; // if consider empty array
    else return maxi;
}

long long maxSubArrayPosisan(vector<int>& nums) {
    long long maxi = LONG_MIN;
    int sum = 0;
    int ansStart = -1, ansEnd = -1, start;
    for (int i=0; i<nums.size(); i++) {
        if (sum == 0) start = i;
        sum += nums[i];

        if (sum > maxi) {
            maxi = sum;
            ansStart = start;
            ansEnd = i;    
        }

        if (sum < 0) sum = 0;
    }
    if (maxi < 0) return 0; // if consider empty array
    else return maxi;
}

int main() {


    return 0;
}