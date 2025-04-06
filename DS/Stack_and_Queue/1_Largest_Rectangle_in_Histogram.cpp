#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

int largestRectangleArea(vector<int>& heights) {
    int n = heights.size();
    stack<int> st;
    int maxArea = 0;

    for(int i = 0; i < n; i++) {
        while (!st.empty() && heights[st.top()] > heights[i]) {
            int element = st.top();
            st.pop();
            int nse = i;
            int pse = st.empty() ? -1 : st.top();
            maxArea = max(maxArea, heights[element] * (nse - pse - 1));
        }
        st.push(i);
    } 

    // Process remaining elements in stack
    while (!st.empty()) {
        int element = st.top();
        st.pop();
        int nse = n;
        int pse = st.empty() ? -1 : st.top();
        maxArea = max(maxArea, heights[element] * (nse - pse - 1));
    } 
    return maxArea;
}

int largestRectangleArea(vector<int>& heights) {
    int n=heights.size();
    stack<int>s;
    int ans=0;
    for(int i=0;i<n;i++)
    {
        while(!s.empty()&&heights[i]<heights[s.top()])
        {
            int nse,pse;
            nse=i;
            int elementi=s.top();
            s.pop();
            if(s.empty())
            {
                pse=-1;
            }
            else
            {
                pse=s.top();
            }
            
            int ta=heights[elementi]*(nse-pse-1);
            ans=max(ta,ans);
           
        }
         s.push(i);
    }
        while(!s.empty())
        {
            int nse=n;
            int pse;
            int elementi=s.top();
            s.pop();
            if(s.empty())
            {
                pse=-1;
            }
            else
            {
                pse=s.top();
            }
            int ta=heights[elementi]*(nse-pse-1);
            ans=max(ta,ans);
        }
        return ans;
    
}

int maximalRectangle(vector<vector<char>>& matrix) {
    if(matrix.empty()) return 0;  // Handle empty matrix case
    int n = matrix.size();
    int m = matrix[0].size();
    int maxArea = 0;
    vector<int> height(m, 0);

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            if(matrix[i][j] == '1') height[j]++;
            else height[j] = 0;
        }
        int area = largestRectangleArea(height);
        maxArea = max(maxArea, area);
    }
    return maxArea;
}

int main() {


    return 0;
}