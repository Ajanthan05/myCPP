#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

int firstMissingPositive(vector<int>& nums) {
    int len = nums.size();

    for(int i=0; i<len; ++i) {
        if (nums[i] < 0) nums[i] = 0;
    }        

    for(int i=0; i<len; ++i) {
        int val = abs(nums[i]);
        if (1 <= val && val <= len) {
            if (nums[val - 1] > 0) nums[val - 1] *= -1;

            else if (nums[val - 1] == 0) nums[val - 1] = -1 * (len + 1);
        }
    }

    for(int i=1; i<=len; ++i) {
        if (nums[i - 1] >= 0) return i;
    } 
    return len + 1;
}

int main() {


    return 0;
}