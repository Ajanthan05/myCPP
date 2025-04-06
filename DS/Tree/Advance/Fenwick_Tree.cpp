#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class FenwickTree {
private:
    vector<int> fenwick;
    int N;

public:
    // Constructor to initialize Fenwick Tree
    FenwickTree(int size) {
        N = size;
        fenwick.assign(N + 1, 0);  // 1-based indexing
    }

    // Point Update: Add 'value' at index 'i'
    void update(int i, int value) {
        while (i <= N) {
            fenwick[i] += value;
            i += (i & -i); // Move to the next responsible index
        }
    }

    // Prefix Sum: Sum of values from index 1 to i
    int sum(int i) {
        int s = 0;
        while (i > 0) {
            s += fenwick[i];
            i -= (i & -i); // Move to parent index
        }
        return s;
    }

    // Range Sum Query: Sum between index l and r
    int rangeSum(int l, int r) {
        return sum(r) - sum(l - 1);
    }

    // Find index with given cumulative frequency 'k' (Binary Lifting)
    int find(int k) {
        int curr = 0, prevSum = 0;
        for (int i = log2(N); i >= 0; i--) {
            int nextIndex = curr + (1 << i);
            if (nextIndex <= N && prevSum + fenwick[nextIndex] < k) {
                curr = nextIndex;
                prevSum += fenwick[nextIndex];
            }
        }
        return curr + 1;  // 1-based index
    }

    // Print Fenwick Tree for debugging
    void printFenwick() {
        for (int i = 1; i <= N; i++) {
            cout << fenwick[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    int n = 10;
    FenwickTree ft(n);

    // Example updates
    ft.update(1, 5);
    ft.update(3, 7);
    ft.update(5, 6);
    ft.update(7, 8);
    ft.update(9, 2);

    // Queries
    cout << "Sum from 1 to 5: " << ft.sum(5) << endl;
    cout << "Range Sum (3 to 7): " << ft.rangeSum(3, 7) << endl;
    cout << "Find position of cumulative sum 10: " << ft.find(10) << endl;

    // Print the Fenwick Tree
    cout << "Fenwick Tree State: ";
    ft.printFenwick();

    return 0;
}

/*
Update a value (update function)

Compute prefix sum (sum function)

Compute range sum (rangeSum function)

Find the k-th smallest prefix sum (find function)

Explanation
Update: Adds a value to a specific index and updates the relevant nodes.

Sum: Computes the prefix sum from index 1 to i.

Range Sum: Uses sum(r) - sum(l-1) for efficient range queries.

Find: Locates the smallest index such that prefix sum is at least k.

This implementation ensures O(log N) operations for update, sum, and range queries.

Let me know if you need modifications
*/
// int fen[N];
// void update(int i, int add) {
//     while(i < N) {
//         fen[i] += add;
//         i += (i & (-i));
//     }
// }
// int sum(int i) {
//     int s = 0;
//     while (i > 0) {
//         s += fen[i];
//         i = i - (i & (-i));
//     }
//     return s;
// }

// int rangeSum(int l, int r) {
//     return sum(r) - sum(l-r);
// }

// int find(int k) {
//     int curr = 0, ans = 0, prevsum = 0;
//     for(int i=log2(n); i>=0; i--) {
//         if(ft[curr + (1<<i)] + prevsum < k) {
//             curr = curr + (1<<i);
//             prevsum += ft[curr];
//         }
//     }
//     return (curr + 1);
// }

