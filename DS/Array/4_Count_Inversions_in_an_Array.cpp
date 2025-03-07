#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

void merge(std::vector<int> &arr, int low, int mid, int high, int &cnt) {
    int left = low;
    int right = mid+1;
    std::vector<int> tmp;

    while(left <= mid && right <= high) {
        if (arr[left] <= arr[right]) {
            tmp.push_back(arr[left]);
            left++;
        } 
        // Right is smaller
        else {
            tmp.push_back(arr[right]);
            cnt += (mid - left + 1);
            right++; 
        }
    }

    while(left <= mid) {
        tmp.push_back(arr[left]);
        left++;
    }

    while (right <= high) {
        tmp.push_back(arr[right]);
        right++;
    }

    for (int i=low; i<=high; i++) {
        arr[i] = tmp[i - low];
    }
}

void mergeSort(std::vector<int> &arr, int low, int high, int &cnt) {
    if (low == high) return;

    int mid = (low + high)/2;

    mergeSort(arr, low, mid, cnt);
    mergeSort(arr, mid+1, high, cnt);

    merge(arr, low, mid, high, cnt);
}

int numberOfInversions(vector<int>&a, int n) {
    int cnt = 0;
    mergeSort(a, 0, n-1, cnt);
    return cnt;
}

int main() {


    return 0;
}