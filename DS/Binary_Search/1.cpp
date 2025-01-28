#include <iostream>
#include <vector>
#include <climits>
// #include <pair>

// TC = O(log2(N))
int srarch(int arr[], int n, int target) {
    int low = 0;
    int high = n-1;
    int mid;

    while(low <= high) {
        // mid = (low + high) / 2;
        mid = low + (high - low ) / 2; // Overflow case

        if(target == arr[mid]) return mid;
        else if(target < arr[mid]) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    return -1;
}

int recursive_srarch(int arr[], int low, int high, int target) {

        if (low > high) return -1;

        int mid = (low + high) / 2;

        if(target == arr[mid]) return mid;

        else if(target < arr[mid]) {
            return recursive_srarch(arr, low, mid-1, target);
            // high = mid - 1;
        }
        else {
            return recursive_srarch(arr, mid+1, high, target);
            // low = mid + 1;
        }
}

/*      LOWER BOUND */

// TC = O(log2(N))
int lower_bound(int arr[], int n, int target) {
    int low = 0;
    int high = n-1;
    int mid;
    int ans = n;

    while(low <= high) {
        // mid = (low + high) / 2;
        mid = low + (high - low ) / 2; // Overflow case

        if(target <= arr[mid]) {
            high = mid - 1;
            ans = mid;
        }
        else {
            low = mid + 1;
        }
    }
    return ans;
}


/* */

int Lower_bound(std::vector<int>&  arr, int n, int target) {
    int low = 0;
    int high = n-1;
    int mid;
    int ans = n;

    while(low <= high) {
        // mid = (low + high) / 2;
        mid = low + (high - low ) / 2; // Overflow case

        if(target <= arr[mid]) {
            high = mid - 1;
            ans = mid;
        }
        else {
            low = mid + 1;
        }
    }
    return ans;
}

int Upper_bound(std::vector<int>&  arr, int n, int target) {
    int low = 0;
    int high = n-1;
    int mid;
    int ans = n;

    while(low <= high) {
        // mid = (low + high) / 2;
        mid = low + (high - low ) / 2; // Overflow case

        if(target < arr[mid]) {
            high = mid - 1;
            ans = mid;
        }
        else {
            low = mid + 1;
        }
    }
    return ans;
}

std::pair<int, int> First_and_Last_Occurrences(std::vector<int>& nums, int target) {
    int n = nums.size();
    int lb = Lower_bound(nums, n, target);
    if (lb == n || nums[lb] == target) return {-1, -1};
    return {lb, Upper_bound(nums, nums.size(), target) - 1};
}

/* Directely find*/

int First_Occurrences(std::vector<int>& nums, int target, int n) {
    int low = 0;
    int high = n-1;
    int mid;
    int first = n;

    while(low <= high) {
        // mid = (low + high) / 2;
        mid = low + (high - low ) / 2; // Overflow case

        if (target == nums[mid]) {
            first = mid;
            high = mid-1;
        }
        else if (target >= nums[mid]) {
            low = mid + 1;
        }
        else {
            high = mid -1;
        }
    }
    return first; 
}

int Last_Occurrences(std::vector<int>& nums, int target, int low, int n) {
    int high = n-1;
    int mid;
    int last = n;

    while(low <= high) {
        // mid = (low + high) / 2;
        mid = low + (high - low ) / 2; // Overflow case

        if (target == nums[mid]) {
            last = mid;
            high = mid-1;
        }
        else if (target >= nums[mid]) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return last; 
}

std::pair<int, int> Direct_First_and_Last_Occurrences(std::vector<int>& nums, int target) {
    int n = nums.size();
    int first = First_Occurrences(nums, target, n);
    if (first == -1) return {-1, -1};
    int last = Last_Occurrences(nums, target, first, n);
    return {first, last};
}

int main() {

    int arr[] = {3,4,6,7,9,12,16,17}; 
    std::cout << srarch(arr,8,3) << "\n";
    std::cout << srarch(arr,8,17) << "\n";
    std::cout << srarch(arr,8,9) << "\n";
    std::cout << srarch(arr,8,1) << "\n";
    std::cout << srarch(arr,8,30) << "\n";

    std::cout << "\n";
    std::cout << recursive_srarch(arr, 0, 7, 3) << "\n";
    std::cout << recursive_srarch(arr,0, 8, 17) << "\n";
    std::cout << recursive_srarch(arr,0, 7, 9) << "\n";
    std::cout << recursive_srarch(arr,0, 7, 1) << "\n";
    std::cout << recursive_srarch(arr,0, 7, 30) << "\n";

    std::cout << "\n";
    auto lb = std::lower_bound(arr, arr + 7, 13) - arr;
    std::cout << "std::lower_bound: " << lb << "\n";

    std::cout << "\n";
    std::vector<int> arr2 = {3,4,6,7,9,12,12,12,12,12,16,17}; 
    std::pair<int, int> ans2 = Direct_First_and_Last_Occurrences(arr2, 12);
    std::cout << "Direct_First_and_Last_Occurrences: " << ans2.first << ", " << ans2.second << "\n";

    std::cout << "\n";
    int arr3[] = {3,4,6,7,9,12,12,12,12,12,16,17};
    return 0;
}