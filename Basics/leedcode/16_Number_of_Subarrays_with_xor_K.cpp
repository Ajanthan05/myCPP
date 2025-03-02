#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

int subarraysWithSumK(vector<int> arr, int k) {
    int xr = 0;
    map<int, int> mpp;
    mpp[xr]++;  // {0, 1}
    int cnt = 0;

    for(int i=0; i<arr.size(); i++) {
        xr = xr ^ arr[i];

        // x
        int x = xr ^ k;
        cnt += mpp[x];
        mpp[xr]++;
    }
}

int subarrayBitwiseORs(vector<int>& arr) {
    unordered_set<int> globalSet; // To store all distinct OR results.
    unordered_set<int> currentSet; // OR results for subarrays ending at the previous index.
    
    for (int num : arr) {
        unordered_set<int> nextSet;
        nextSet.insert(num); // Start new subarray with the current element.
        globalSet.insert(num);
        
        // Extend each subarray ending at the previous index.
        for (int prev : currentSet) {
            int newOr = prev | num;
            nextSet.insert(newOr);
            globalSet.insert(newOr);
        }
        
        // Prepare for next iteration.
        currentSet = move(nextSet);
    }
    
    return globalSet.size();
}


int main() {


    return 0;
}