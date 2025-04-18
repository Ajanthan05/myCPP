#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;
/* Given start, end, and an array arr of n numbers. At each step, the start is multiplied by any number in the array and then a mod operation with 100000 is done to get the new start.
Find the minimum steps in which the end can be achieved starting from the start. If it is not possible to reach the end, then return -1.
*/
int mimumMultiplication(vector<int> &arr, int start, int end) {
    queue<pair<int, int>> q;
    q.push({start, 0});
    vector<int> dist(100000, 1e9);
    dist[start] = 0;
    int mod = 100000;

    // Worst case O(100000 * N)
    while(!q.empty()) {
        int node = q.front().first;
        int steps = q.front().second;
        q.pop();

        for(auto it : arr) {
            int num = (it * node) % mod;
            if (steps + 1 < dist[num]) {
                if (num == end) return steps + 1;
                dist[num] = steps + 1;
                q.push({num, steps + 1});
            }
        }
    }
    return -1;
}

int main() {


    return 0;
}