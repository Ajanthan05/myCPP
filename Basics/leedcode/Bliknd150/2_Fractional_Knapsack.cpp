#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool compare(vector<int> a, vector<int> b) {
    return (a[0]/a[1] >= b[0]/b[1]);
}

int maximumUnits(vector<vector<int>>& boxTypes, int truckSize) {
    int n = boxTypes.size();
    sort(boxTypes.begin(), boxTypes.end(), compare);
    double totalval = 0;

    for(int i=0; i<n; i++) {
        if (boxTypes[i][1] < truckSize) {
            totalval += boxTypes[i][0];
            truckSize -= boxTypes[i][1];
        }
        else {
            totalval += (double)boxTypes[i][1]/boxTypes[i][0] * truckSize;
            break;
        }
    }
    return totalval;
}


int main() {


    return 0;
}