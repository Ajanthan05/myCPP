#include <iostream>
#include <vector>
#include <map>

using namespace std;

/* Work for Positive and negatives 
TC = O(N LOG(N))
TC = O(N * 1) UNORDERED MAP WORST CASE ~O(N^2)
SC = O(N)
*/
int longestSubarrayWithSumK(vector<int> a, long long k) {
    long long sum = 0;
    int maxLen = 0;
    map<long long, int> preSumMap;

    for (int i=0; i<a.size(); i++) {
        sum += a[i];
        if (sum == k) {
            maxLen = max(maxLen, i+1);
        }

        long long remain = sum - k;
        
        if (preSumMap.find(remain) != preSumMap.end()) {
            int len = i - preSumMap[remain];
        }
        // When zeros in the array
        if (preSumMap.find(sum) == preSumMap.end()) {
            preSumMap[sum] = i;  // If sum previously exit do not update
        }
    }
    return maxLen;
}

/* Only Work for Positive (zero also ) */
int longestSubarrayWithSumK_optimal(vector<int> a, long long k) {
    long long sum = 0;
    int maxLen = 0;
    int l=0 , r=0;
    int n = a.size();
    int len = 0;

    while(r < n) {
        if (r < n) sum += a[r];

        while(l <= r && sum > k) {
            sum -= a[l];
            l++;
        }
        if (sum == k) {
            maxLen = max(maxLen, r-l+1);
        }
        r++;  
    }
    return maxLen;
}


/* Only Work for Positive (zero also ) */
int longestSubarrayWithSumK2(vector<int>& a, long long k) {
    long long sum = 0;
    int maxLen = 0;
    int l = 0;
    int n = a.size();
    
    // Expand the right end of the window.
    for (int r = 0; r < n; r++) {
        sum += a[r];
        
        // While the current sum is greater than k, shrink the window from the left.
        while (l <= r && sum > k) {
            sum -= a[l];
            l++;
        }
        
        // If the current window's sum equals k, update the maximum length.
        if (sum == k) {
            maxLen = max(maxLen, r - l + 1);
        }
    }
    
    return maxLen;
}

int main() {

    vector<int> a = {1,2,3,1,1,1,1,4,2,3};
    cout << longestSubarrayWithSumK2(a, 3) << "\n";
    cout << longestSubarrayWithSumK_optimal(a, 3) << "\n";

    cout << longestSubarrayWithSumK(a, 3) << "\n";
    return 0;
}