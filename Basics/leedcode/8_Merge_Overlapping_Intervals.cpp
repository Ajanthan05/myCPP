#include <iostream>
#include <vector>
#include <climits>
/*
TC = O(NLOG(N))
SC = O(2N)*/
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    int n = intervals.size();
    std::sort(intervals.begin(), intervals.end());
    std::vector<std::vector<int>> ans;

    for(int i=0; i<n; i++) {
        int start = intervals[i][0];
        int end = intervals[i][1];

        if(!ans.empty() && end <= ans.back()[1]) {
            continue;
        }
        for(int j=i+1; j<n; j++) {
            if(intervals[j][0] <= end) {
                end = std::max(end, intervals[j][1]);
            }
            else {
                break;
            }
        }
        ans.push_back({start, end});
    }
    return ans;
}



/* OPTMIZE
TC = O(NLOG(N))
SC = O(2N)*/
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    int n = intervals.size();
    std::sort(intervals.begin(), intervals.end());
    std::vector<std::vector<int>> ans;

    for(int i=0; i<n; i++) {

        if(ans.empty() || ans.back()[1] < intervals[i][0]) {
            ans.push_back(intervals[i]);
        }
        else {
            ans.back()[1] = std::max(ans.back()[1], intervals[i][1]);
            // ans.back()[0] = std::min(ans.back()[0], interval[i][0]); // no need we already sorted
        }
    }
    return ans;
}

int main() {


    return 0;
}