#include <iostream>
#include <vector>
#include <climits>

std::vector<int> fun(std::vector<int> arr) {
    int n = arr.size();
    int hash[n + 1] = {0};

    for(int i=0; i<n; i++) {
        hash[arr[i]]++;
    }

    int repeating = -1;
    int missing = -1;
    for(int i=0; i<n; i++) {
        if(hash[i] == 0) missing = i;
        if(hash[i] == 2) repeating = i;

        if(missing != -1 && repeating != -1) break;
    }
    return {repeating, missing};
}

int main() {

    std::vector<int> nums =  {1,2,3,5,6,7,5}; // {1,10, 25, 10, 3, 4};
    std::vector<int>  ans = fun(nums);
    for(auto it : ans) {
        std::cout << it << "\n";
    }

    return 0;
}