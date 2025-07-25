#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/*  
1 
1 3 
1 3 4
1 3 4 8*/

int main() {
    int n  = 10;
    std::string str;
    
    for(int i=1; i<=n; i++) {
        int sum = 1;
        // for(int j=1; j<i; j++) {
            sum += i;
            
        // }
        str = str + " " + std::to_string(sum);
        cout << str << "\n";
    }

    return 0;
}