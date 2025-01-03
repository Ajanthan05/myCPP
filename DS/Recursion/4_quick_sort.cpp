#include <iostream>
#include <vector>

int placeItInRightPlace(std::vector<int> &arr, int low, int high) {
    int i = low;
    int j = high;
    int pivat = arr[low]; // We dont need it, use directly => just for understanding

    while(i < j) {
        // if (arr[i] > pivat)
        while (arr[i] <= pivat && i <= high - 1) {  // == will be place at left
            i++;
        }

        while (pivat < arr[j] && j >= low + 1) {
            j--;
        }
        if (i < j) std::swap(arr[i], arr[j]);
            
    }
    // pivat to come to dthe correct place
    std::swap(arr[low], arr[j]); // swap with the last element of left index
    
    return j;
}


void quickSort(std::vector<int> &arr, int low, int high) {
    if (low < high) {
        int partion_idx = placeItInRightPlace(arr, low, high);
        quickSort(arr, low, partion_idx-1);
        quickSort(arr, partion_idx + 1, high);
    }
}

int main() {

    std::vector<int> arr = {3,1,2,4,1,5,6,2,4}; // 1 1 2 2 3 4 4 5 6 
    quickSort(arr, 0, arr.size()-1);

    for (auto it : arr) std::cout << it << " ";
    std::cout << "\n"; 

    return 0;
}