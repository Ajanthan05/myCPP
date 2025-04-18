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

int main() {


    return 0;
}