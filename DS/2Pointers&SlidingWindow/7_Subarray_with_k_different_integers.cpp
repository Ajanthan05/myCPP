#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

int fun(vector<int>& nums, int k) {
    int n = nums.size();
    int l=0, r=0;
    int cnt = 0;
    map<int, int> mpp;

    while(l<=n) {
        mpp[nums[r]]++;
        while(mpp.size() > k) {
            mpp[nums[l]]--;
            if (mpp[nums[l]] == 0) {
                mpp.erase(nums[l]);
            }
            l++;
        }  
        cnt = cnt + (r - l + 1);
        r++;
    }
    return cnt;
}

int subarraysWithKDistinct(vector<int>& nums, int k) {
    return fun(nums, k) - fun(nums, k - 1);
}

int main() {


    return 0;
}