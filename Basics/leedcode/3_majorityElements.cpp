#include <iostream>
#include <vector>
#include <climits>

int majorityElement(std::vector<int>& nums) {
    std::map<int, int> mpp;
    // /N logN
    for (int i=0; i<nums.size(); i++) {
        mpp[nums[i]]++;
    }

    for (auto it : mpp) {
        if (it.second)
    }
}

int main() {


    return 0;
}