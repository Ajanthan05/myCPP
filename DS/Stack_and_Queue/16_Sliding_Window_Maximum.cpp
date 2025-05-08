#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>
#include <deque>

using namespace std;
/*  
TC = O(2N)
SC = O(K) + O(N-K)
*/
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> ans;
    ans.reserve(n - k + 1);  // Reserve space to avoid reallocations
    deque<int> dq;

    for(int i=0; i<n; i++) {
        if(!dq.empty() && dq.front() <= i-k) {
            dq.pop_front();
        }
       
        while(!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        
        dq.push_back(i);

        if (i >= k-1) ans.push_back(nums[dq.front()]);
    }
    return ans; 
}

// OPTIMAL
vector<int> maxSlidingWindow_O(vector<int>& nums, int k) 
{
    // -------- Sliding window + Deque(Monotonic queue), O(n) ------------

    deque<int> dq;
    vector<int> ans;
    for(int i=0;i<k;i++)
    {
        while(!dq.empty() && nums[i] > nums[dq.back()])
        {
            dq.pop_back();
        }
        dq.push_back(i);
    }
    // for(auto i :dq)cout<<nums[i]<<", ";cout<<endl;
    ans.push_back(nums[dq.front()]);
    for(int i=1;i<nums.size()-(k-1);i++)
    {
        // cout<<i<<endl;
        if(!dq.empty() && dq.front()<i) dq.pop_front();
        while(!dq.empty() && nums[i+k-1] > nums[dq.back()])
        {
            dq.pop_back();
        }
        dq.push_back(i+k-1);
        // for(auto i :dq)cout<<nums[i]<<", ";cout<<endl;
        ans.push_back(nums[dq.front()]);
    }
    return ans;



    // -------- Sliding window + map(tree based), O(n*log(k)) -------

    // map<int,int> m;
    // for(int i=0;i<k;i++)
    // {
    //     m[nums[i]]++;
    // }
    // vector<int> ans;
    // ans.push_back(m.rbegin()->first);
    // for(int i=1;i<nums.size()-k+1;i++)
    // {
    //     m[nums[i-1]]--;
    //     if(m[nums[i-1]]==0) m.erase(nums[i-1]);
    //     m[nums[i+k-1]]++;
    //     ans.push_back(m.rbegin()->first);
    // }
    // return ans;        
}


int main() {


    return 0;
}