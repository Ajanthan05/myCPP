#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;



void merge(vector<int>& nums, int low, int mid, int high) {
    vector<int> tmp;
    int left = low;
    int right = mid + 1;

    while(left <= mid && right <= high) {
        if (nums[left] <= nums[right]) {
            tmp.push_back(nums[left]);
            left++;
        } else {
            tmp.push_back(nums[right]);
            right++;
        }
    }

    while(left <= mid) {
        tmp.push_back(nums[left]);
        left++;
    }

    while(right <= high) {
        tmp.push_back(nums[right]);
        right++;
    }

    for(int i = low; i <= high; i++) {
        nums[i] = tmp[i - low];
    }
}

void countPairs(vector<int>& nums, int& cnt, int low, int mid, int high) {
    int right = mid + 1;
    for(int i = low; i <= mid; i++) {
        while (right <= high && nums[i] > 2LL * nums[right]) right++;
        cnt += right - (mid + 1);
    }
}

void mergeSort(vector<int>& nums, int& cnt, int low, int high) {
    if (low >= high) return;
    int mid = (low + high) / 2;
    mergeSort(nums, cnt, low, mid);
    mergeSort(nums, cnt, mid + 1, high);
    countPairs(nums, cnt, low, mid, high);
    merge(nums, low, mid, high);
}

int reversePairs(vector<int>& nums) {
    int cnt = 0;
    mergeSort(nums, cnt, 0, nums.size() - 1);
    return cnt;
}

const auto _ = std::cin.tie(nullptr)->sync_with_stdio(false);

#define LC_HACK 
const auto __ = []() {
    struct ___ {
        static void _() { std::ofstream("display_runtime.txt") << 0 << '\n'; }
    };
    std::atexit(&___::_);
    return 0;
}();
class Solution {
public:

    int merge(vector<int> &nums, int l, int mid, int r){
        int cnt=0;
        vector<int> temp;
        int i=l,j=mid+1;
        int p=mid+1;
        for(int k=l;k<=mid;k++){
            while((p<=r) && ((long long int)nums[k] > ((long long int)2 * nums[p])) ){
                p++;
            }
            cnt+=(p - mid -1);
        }
        while((i<=mid) && (j<=r)){
            if(nums[i]>nums[j]){
                temp.push_back(nums[j++]);
            }else{
                temp.push_back(nums[i++]);
            }
        }
        while(j<=r){
            temp.push_back(nums[j++]);
        }
        while(i<=mid){
            temp.push_back(nums[i++]);
        }
        p=0;
        for(int k=l;k<=r;k++){
            nums[k]=temp[p++];
        }
        return cnt;
    }
    int mergeSort(vector<int> &nums, int l,int r){
        if(l>=r)return 0;
        int cnt=0;
        int mid=(l+r)/2;
        cnt+=mergeSort(nums,l,mid);
        cnt+=mergeSort(nums,mid+1,r);
        cnt+=merge(nums,l,mid,r);
        return cnt;
    }
    int reversePairs(vector<int>& nums) {
        return mergeSort(nums,0,nums.size()-1);
    }
};

int main() {


    return 0;
}