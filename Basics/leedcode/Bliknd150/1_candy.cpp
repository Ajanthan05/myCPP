#include <iostream>
#include <vector>

using namespace std;

int candy(vector<int>& ratings) {
    int n = ratings.size();
    int cL[n], cR[n];
    cL[0] = 1;
    cR[n-1] = 1;
    
    for(int i=0; i<n; i++) {
        if(i>0 && ratings[i] > ratings[i-1]) {
            cL[i] = cL[i-1] + 1;
        }
        else {
            cL[i] = 1;
        }

        if(i<n-1 && ratings[n-i] > ratings[n-i-1]) {
            cR[i] = cR[i-1] + 1;
        }
        else {
            cR[i] = 1;
        }
    }
}

int candy(vector<int>& ratings) {
    int n = ratings.size();
    int sum = 1;
    int i = 1;

    while(i<n) {
        if (ratings[i] == ratings[i-1]) {
            sum += 1;
            i++;
            continue;
        }

        int peak = 1;
        while(i<n && ratings[i] > ratings[i-1]) {
            peak++;
            sum += peak;
            i++;       
        }

        int down = 1;
        while(i<n && ratings[i] < ratings[i-1]) {
            sum += down;
            i++;
            down++;
        }
        if (down > peak) {
            sum += (down - peak);
        }
    }
    return sum;
}

int main() {


    return 0;
}