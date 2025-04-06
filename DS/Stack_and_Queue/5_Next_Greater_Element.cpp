#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;
/*
TC = O(2N) // worst case
SC = O(N) + O(N){for ans}
*/
vector<int> nextGreaterElement(vector<int>& arr)
{
	int n = arr.size();
    stack<int> st;
    vector<int> ans(n);
    int nG = -1;
    for(int i=n-1; i>=0; i--) {
        while(!st.empty() && st.top() <= arr[i]) {
            st.pop();
        }
        if (st.empty()) ans[i] = -1;
        else {
            ans[i] = st.top();
        }
        st.push(arr[i]);
    }

    return ans;
}

/* Next Greater Element - II
For gnext greater element u have to think as cyrcle

TC = O(4N) // worst case
SC = O(2N) + O(N){for ans}
*/
vector<int> nextGreaterElement(vector<int>& arr)
{
	int n = arr.size();
    stack<int> st;
    vector<int> ans(n);
    int nG = -1;
    for(int i=2*n-1; i>=0; i--) {
        while(!st.empty() && st.top() <= arr[i % n]) {
            st.pop();
        }
        if (i < n) {
            if (st.empty()) ans[i] = -1;
            else {
                ans[i] = st.top();
            }
        }
        st.push(arr[i % n]);
    }

    return ans;
}

int main() {
    
    return 0;
}