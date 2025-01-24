// #include <iostream>
// #include <vector>
// #include <climits>

// int findDuplicate(std::vector<int> &nums) {
//     int slow = nums[0];
//     int fast = nums[0];

//     do {
//         slow = nums[slow];
//         fast = nums[nums[fast]];
//     } while(slow != fast);

//     fast = nums[0];
//     while(slow != fast) {
//         slow = nums[slow];
//         fast = nums[fast];
//     }
//     return slow;
// }

// int main() {
//     std::vector<int> nums =  {1,2,3,5,6,7,5}; // {1,10, 25, 10, 3, 4};
//     std::cout << findDuplicate(nums) << "\n";

//     return 0;
// }

#include <iostream>
#include <vector>
#include <stdexcept>

int findDuplicate(std::vector<int> &nums) {
    // Validate the input
    if (nums.empty()) throw std::invalid_argument("Input array cannot be empty.");
    for (int num : nums) {
        if (num < 1 || num >= nums.size()) {
            throw std::invalid_argument("All numbers must be in the range [1, n], where n = nums.size() - 1.");
        }
    }

    // Floyd's Tortoise and Hare algorithm for cycle detection
    int slow = nums[0];
    int fast = nums[0];

    // Phase 1: Find the intersection point in the cycle
    do {
        slow = nums[slow];
        fast = nums[nums[fast]];
    } while (slow != fast);

    // Phase 2: Find the entrance to the cycle (duplicate element)
    fast = nums[0];
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
    }

    return slow;
}

int main() {
    std::vector<int> nums = {1, 3, 4, 2, 2, 5, 10}; //{1, 2, 3, 5, 6, 7, 5}; // Example input with a duplicate
    try {
        std::cout << "Duplicate: " << findDuplicate(nums) << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
