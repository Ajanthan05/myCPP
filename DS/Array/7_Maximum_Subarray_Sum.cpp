#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/* Kadane's Algorithm */
int maxSubArray(vector<int>& nums) {
    long long maxi = LONG_MIN;
    int n = nums.size();
    long long sum = 0;

    for(int i=0; i<n; i++) {
        sum += nums[i];

        if (sum > maxi) {
            maxi = sum;
        }

        if (sum < 0) sum = 0;
    }
    // here we are considering empty sub array
    return maxi<0 ? 0 : maxi;
}

/* Print any of those sub array (can be multiple) which give max sum*/
int maxSubArray(vector<int>& nums) {
    int start, ansStart = -1, ansEnd = -1;

    long long maxi = LONG_MIN;
    int n = nums.size();
    long long sum = 0;

    for(int i=0; i<n; i++) {
        if (sum == 0) {
            start = i;
            ansEnd = i;
        }
        sum += nums[i];

        if (sum > maxi) {
            maxi = sum;
            ansStart = start;
            ansEnd = i;
        }

        if (sum < 0) sum = 0;
    }
    // here we are considering empty sub array
    return maxi<0 ? 0 : maxi;
}

int main() {


    return 0;
}