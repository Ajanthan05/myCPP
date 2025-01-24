#include <iostream>
#include <vector>
#include <climits>

std::vector<int> getSecondOrderElimant(std::vector<int> nums) {
    int slargest = INT_MIN;
    int ssmallest = INT_MAX;  // insted of -1,  consider -ve nums also

    int largest = nums[0];
    int smallest = nums[0];
    for (int i=1; i<nums.size(); i++) {
        if (nums[i] > largest) {
            slargest = largest;
            largest = nums[i];
        }
        else if (nums[i] < largest && nums[i] > slargest) {
            slargest = nums[i];
        }

        if (nums[i] < smallest) {
            ssmallest = smallest;
            smallest = nums[i];
        }
        else if (nums[i] != smallest && nums[i] < ssmallest) {
            ssmallest = nums[i];
        }
    }
    return {slargest, ssmallest};
}

int remove_duplicate_inplace_from_sorted_array(std::vector<int> &arr) {
    int index = 0;

    for (int i=1; i<arr.size(); i++) {
        if (arr[index] != arr[i]) {
            index++;
            arr[index] = arr[i];
        }
    }
    return (index + 1);
}

/*
TC = O(N)
SC = O(1)
*/
void move_all_zeros_to_the_end_of_array(int arr[], int n) {
    int j = -1;
  
    for (int i=0; i<n; i++) {
        if (arr[i] == 0) {
            j = i;
            break;
        }
    }

    for (int i=j; i<n; i++) {
        if(arr[i] != 0) {
            std::swap(arr[i], arr[j]);
            j++;
        } 
    }
}

/* BRUT FORSE
TC = O(N1+N2)
SC = O(N2)
*/
std::vector<int> findArrayIntersection(std::vector<int> &a, std::vector<int> &b) {
    std::vector<int> ans;
    int visited[]
}

int main() {

    std::vector<int> nums = {1,7,7,7,4,5,5,2,2,2,1};

    std::vector<int> ans = getSecondOrderElimant(nums);

    for (auto it : ans) {
        std::cout << it << "\n";
    }

    std::vector<int> nums2 = {1,1,1,2,3,3,4,5,6,6};
    int length = remove_duplicate_inplace_from_sorted_array(nums2);

    for (auto it : nums2) {
        std::cout << it << " ";
    }
    std::cout << "\n" << length << "\n";


    int arr[] = {1,0,2,3,2,0,0,4,5,1};
    move_all_zeros_to_the_end_of_array(arr, 10);
    std::cout << "\n move_all_zeros_to_the_end_of_array: ";
    for (int i=0; i<10; i++) {
       std::cout << arr[i] << " "; 
    }
    std::cout << "\n";

    return 0;
}