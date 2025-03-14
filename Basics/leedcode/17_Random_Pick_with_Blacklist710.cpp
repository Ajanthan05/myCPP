#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
    int valid_num = 0;
    unordered_map<int, int> m;
public:
    Solution(int n, vector<int>& blacklist) {
        set<int> s;

        for(const auto& num : blacklist) s.insert(num);

        int index = n-1;
        valid_num = n - s.size();

        for(const auto& num : s) {
            if(num < valid_num) {
                while(s.count(index)) {
                    index--;
                }
                m[num] = index;
                index--;
            }
        }
    }
    
    int pick() {
        int res = rand() % valid_num;

        if(m.count(res)) {
            return m[res]

        }
        return res;
    }
};

int main() {


    return 0;
}