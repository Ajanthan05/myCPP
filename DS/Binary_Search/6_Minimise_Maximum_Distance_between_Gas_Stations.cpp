#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

double minimiseMaxDistance(vector<int> &arr, int k) {
    int n = arr.size();
    vector<int> howMeny(n-1, 0);

    for(int gasStation=0; gasStation<k; gasStation++) {
        int maxInd = -1;
        long double maxSection = -1;

        for(int i=0; i<n-1; i++) {
            long double diff = arr[i+1] - arr[i];
            long double sectionLength = diff / (long double)(howMeny[i] + 1);

            if (sectionLength > maxSection) {
                maxSection = sectionLength;
                maxInd = i;
            }
        }
        howMeny[maxInd]++;
    }

    long double maxAns = -1;
    for (int i=0; i<n-1; i++) {
        long double diff = arr[i+1] - arr[i];
        long double sectionLength = diff / (long double)(howMeny[i] + 1);
        maxAns = max(maxAns, sectionLength);
    }
    return maxAns;
}

/*
TC = O(Nlog(N)) + o(Klog(N))
SC = O(N-1)
*/
double minimiseMaxDistance_Optimal(vector<int> &arr, int k) {
    int n = arr.size();
    vector<int> howMeny(n-1, 0);

    
    int maxInd = -1;
    long double maxSection = -1;

    priority_queue<pair<long double, int>> pq;
    // nlog(n)
    for(int i=0; i<n-1; i++) {
        pq.push({(arr[i+1] - arr[i]), i});
    }
    // {maxSection, maxInd} = pq.top();

    // klog(n)
    for(int gasStation=0; gasStation<k; gasStation++) {
        auto tp = pq.top();
        pq.pop();
        int secInd = tp.second;
        howMeny[secInd]++;
        long double diff = arr[secInd + 1] - arr[secInd];
        long double newSecLen = diff / (long double)(howMeny[secInd] + 1);
        pq.push({newSecLen, secInd});
    }

    return pq.top().first;
}

/* BST
*/
int numberOfGaseStarionRequired(long double dist, vector<int> &arr) {
    int cnt = 0;
    for(int i=1; i<arr.size(); i++) {
        int numberInBetween = (arr[i] - arr[i-1]) / dist;
        if ( (arr[i] - arr[i-1]) / dist == numberInBetween * dist) {
            numberInBetween--;
        }
        cnt += numberInBetween;
    }
    return cnt;
}

double minimiseMaxDistance_BST(vector<int> &arr, int k) {
    int n = arr.size();
    long double low = 0;
    long double high = 0;
    for(int i=0; i<n-1; i++) {
        high = max(high, (long double)(arr[i+1]-arr[i]));
    }

    long double diff = 1e-6;
    while(high-low > diff) {
        long double mid = (low + high) / (2.0);
        int cnt = numberOfGaseStarionRequired(mid, arr);
        if(cnt > k) {
            low = mid;
        }
        else {
            high = mid;
        }
    }
    return high;
}

/*
Imagine:

Segment length: arr[i] - arr[i-1] = 10

dist: 2

Then:

10 / 2 = 5

Initially:

numberInBetween = 5

But 5 intervals of length 2 would be:

2 2 2 2 2 = 10
Here, you only need 4 gas stations (not 5) because:

The segment can be perfectly divided with the original two endpoints and 4 extra gas stations.

Example:
|--2--|--2--|--2--|--2--|--2--|
Positions:

Start: 0

2, 4, 6, 8

End: 10

Gas stations added:

At 2, 4, 6, 8 (4 new stations)
*/
int main() {


    return 0;
}