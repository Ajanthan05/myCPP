#include <iostream>
#include <vector>

void merge(std::vector<int> &arr, int low, int mid, int high) {
    int left = low;
    int right = mid+1;
    std::vector<int> tmp;

    while(left <= mid && right <= high) {
        if (arr[left] <= arr[right]) {
            tmp.push_back(arr[left]);
            left++;
        } else {
            tmp.push_back(arr[right]);
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

void mergeSort(std::vector<int> &arr, int low, int high) {
    if (low == high) return;

    int mid = (low + high)/2;

    mergeSort(arr, low, mid);
    mergeSort(arr, mid+1, high);

    merge(arr, low, mid, high);
}

int main() {

    std::vector<int> arr = {3,1,2,4,1,5,6,2,4};
    mergeSort(arr, 0, 8);

    for (auto it : arr) std::cout << it << " ";
    std::cout << "\n"; 

    return 0;
}