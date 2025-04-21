#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

int maxProduct(vector<int>& nums) {
    int n = nums.size();
    int prefix = 1, sufix = 1;
    int maxi = INT_MIN;

    for(int i=0; i<n; i++) {
        if(!prefix) prefix = 1;
        if(!sufix) sufix = 1;

        prefix = prefix * nums[i];
        sufix = sufix * nums[n-1 -i];
        
        maxi = max(maxi, max(prefix, sufix));
    }
    return maxi;
}

int main() {


    return 0;
}