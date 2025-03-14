#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int numberOfSubstrings(string s) {
        
    int n = s.size();
    int cnt = 0;

    for(int i= 0; i<n; i++) {
        char hash[3] = {0};
        for(int j=i; j<n; j++) {
            hash[s[j] - 'a'] = 1;
            if(hash[0] + hash[1] + hash[2] == 3) {
                cnt = cnt + n - j;
                break;
            }
        }
    } 
    return cnt;
}

/* With every charactor there is a substring that ends 
*/
int numberOfSubstrings_Optimal(string s) {
        
    int n = s.size();
    int cnt = 0;
    int lastSean[3] = {-1, -1, -1};

    for(int i= 0; i<n; i++) {
        lastSean[s[i] - 'a'] = i;
        // if (lastSean[0] != -1 && lastSean[1] != -1 && lastSean[2] != -1) {
        //     cnt = cnt + (1 + min(lastSean[0], min(lastSean[1], lastSean[2])) );
        // }

        cnt = cnt + (1 + min(lastSean[0], min(lastSean[1], lastSean[2])) );
    } 
    return cnt;
}

int main() {


    return 0;
}