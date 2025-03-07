#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* You are given an integer array bloomDay, an integer m and an integer k.

You want to make m bouquets. To make a bouquet, you need to use k adjacent flowers from the garden.

The garden consists of n flowers, the ith flower will bloom in the bloomDay[i] and then can be used in exactly one bouquet.

Return the minimum number of days you need to wait to be able to make m bouquets from the garden. If it is impossible to make m bouquets return -1.
*/

/*
TC = O( (MAXI - MIN + 1) x N)
*/
bool isPossible(vector<int>& bloomDay, int day, int m, int k) {
    // for_each(bloomDay.begin(), bloomDay.end(), [&])
    int cnt=0;
    int noOfB = 0;
    for(int i=0; i<bloomDay.size(); i++) {
        if (bloomDay[i] <= day) {
            cnt++;
        }
        else {
            noOfB += cnt/k;
            cnt = 0;
        }
    }
    noOfB += cnt/k;
    if (noOfB >= m) return true;
    return false;
}

int minDays(vector<int>& bloomDay, int m, int k) {
    int n = bloomDay.size();
    if (m*k > n) return -1;
    auto minMax = minmax_element(bloomDay.begin(), bloomDay.end());

    for(int i=*minMax.first; i<=*minMax.second; i++) {
        if (isPossible(bloomDay, i, m, k) == true) return i;
    }
    return -1;
}

int minDays_BS(vector<int>& bloomDay, int m, int k) {
    int n = bloomDay.size();
    if (m*k > n) return -1;
    auto minMax = minmax_element(bloomDay.begin(), bloomDay.end());

    int low = *minMax.first, high = *minMax.second;

    int ans = -1;
    while(low <= high) {
        int mid = low + (high-low)/2;
        if (isPossible(bloomDay, mid, m, k) == true) {
            ans = mid;
            high = mid-1;
        }
        else {
            mid = low+1;
        }
    }

    return low;
}


int main() {


    return 0;
}