#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;


/* TC= O(3N)
SC = O(1)  not using extra space */ 
void nextPermutation(vector<int>& nums) {
    int n = nums.size();
    int index = -1;
    for (int i=n-2; i>=0; i--) {
        if(nums[i] < nums[i+1]) {
            index = i;
            break;
        }
    }

    if (index == -1) {      // means last index
        std::reverse(nums.begin(), nums.end());  // it will give first index
        return;
    }

    for(int i=n-1; i>index; i--) {
        if(nums[i] > nums[index]) {
            std::swap(nums[i], nums[index]);  // boc right of index is assending order
            break;
        }
    }
    /* TC= O(N)*/
    std::reverse(nums.begin() + index + 1, nums.end());
}

// std::next_permutation

int main() {


    return 0;
}