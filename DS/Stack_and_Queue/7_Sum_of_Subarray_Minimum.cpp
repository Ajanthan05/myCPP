#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/* Given an array of integers arr, find the sum of min(b), where b ranges over every (contiguous) subarray of arr. Since the answer may be large, return the answer modulo 109 + 7.
*/
// void NSE(vector<int> &suffixMin, vector<int> &arr, int n) {
//     suffixMin[n-1] = arr[n-1];
//     for(int i=n-2; i>=0; i--) {
//         suffixMin[i] = min(suffixMin[i+1], arr[i]);
//     }
// }
// void PSE(vector<int> &prefixMin,vector<int> &arr, int n) {
//     prefixMin[0] = arr[0];
//     for(int i=1; i<n; i++) {
//         prefixMin[i] = min(prefixMin[i-1], arr[i]);
//     }
// }

/*
TC = O(5N)
SC = O(5N)
*/
void NSE(vector<int> &suffixMin, vector<int> &arr, int n) {
    stack<int> st;
    for(int i=n-1; i>=0; i--) {
        while(!st.empty() && arr[st.top()] >= arr[i]) {
            st.pop();
        }
        suffixMin[i] = st.empty() ? n : st.top();
        st.push(i);
    }
}
void PSEqual_Element(vector<int> &prefixMin,vector<int> &arr, int n) {
    stack<int> st;
    for(int i=0; i<n; i++) {
        while(!st.empty() && arr[st.top()] > arr[i]) { // looking for smaller or equal. dont remove equal
            st.pop();
        }
        prefixMin[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }
}

long long sumSubarrayMins(vector<int>& arr) {
    int n = arr.size();
    vector<int> nse(n), pse(n);
    NSE(nse, arr, n);
    PSEqual_Element(pse, arr, n);
    
    long long total = 0;
    int mod = (int)(1e9 + 7);

    for(int i=0; i<n; i++) {
        int left = i - pse[i];
        int right = nse[i] - i;
        /* By converting one operand to long long (1LL), the entire 
        expression gets promoted to long long, avoiding overflow.*/
        total = (total + (1LL * left * right * arr[i]) % mod) % mod;
    }
    return total;
}


//////////////////////////////
void NGE(vector<int> &suffixMax, vector<int> &arr, int n) {
    stack<int> st;
    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && arr[st.top()] <= arr[i]) { // Find next greater element
            st.pop();
        }
        suffixMax[i] = st.empty() ? n : st.top();
        st.push(i);
    }
}

void PGE(vector<int> &prefixMax, vector<int> &arr, int n) {
    stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && arr[st.top()] < arr[i]) {  // Find previous greater element
            st.pop();
        }
        prefixMax[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }
}

long long sumSubarrayMaxs(vector<int>& arr) {
    int n = arr.size();
    vector<int> nge(n), pge(n);
    NGE(nge, arr, n);
    PGE(pge, arr, n);

    long long total = 0;
    int mod = (int)(1e9 + 7);

    for (int i = 0; i < n; i++) {
        int left = i - pge[i];  // Number of subarrays ending at i
        int right = nge[i] - i;  // Number of subarrays starting at i
        total = (total + (1LL * left * right * arr[i]) % mod) % mod;
    }

    return total;
}

long long subArrayRanges(vector<int>& nums) {
    // return sumSubarrayMaxs(nums) - sumSubarrayMins(nums);
    return (sumSubarrayMaxs(nums) - sumSubarrayMins(nums) + (int)1e9 + 7) % ((int)1e9 + 7);
}

int main() {


    return 0;
}