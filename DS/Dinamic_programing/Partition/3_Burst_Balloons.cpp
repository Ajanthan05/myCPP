#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int f(int ind, int i, int j, vector<int>& nums) {
    if (i > j) return 0;

    int minc = 0;
    for(int ind=i; ind<=j; ind++) {
        int cost = nums[i-1] + nums[ind] + nums[j+1] + f(ind, j, ind-1, nums)
                                                     + f(ind, ind+1, j, nums);
    }
}

int maxCoins(vector<int>& nums) {
    int n = nums.size();
    nums.push_back(1);
    nums.insert(nums.begin(), 1);
    return f(0, 1, n, nums)
}

int main() {


    return 0;
}