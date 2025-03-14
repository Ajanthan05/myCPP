#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

// Count Subarray sum Equals K
/*
TC = O(N)  OR O(NlogN) if consider map TC
SC = O(N)
*/
int subarraySum(vector<int>& nums, int k) {
    map<int, int> mpp;
    mpp[0] = 1;
    int prefixSum = 0;
    int cnt = 0;

    for (int i=0; i<nums.size(); i++) {
        prefixSum += nums[i];
        int remove = prefixSum - k;  //s-k
        cnt += mpp[remove];
        mpp[prefixSum] += 1;
    }
    return cnt;
}

int main() {


    return 0;
}