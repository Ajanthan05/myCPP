#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

class Car {

public:
    virtual void drive() {
        cout << "Drive\n";
    }
};

class Tata : public Car {
public:
    void drive() override {
        cout << "Drive Tata\n";
    }
};

int fun(const vector<int> &arr, int x, int low, int high) {
    if (low > high) return -1;
    int mid = low + (high - low)/ 2;
    if(arr[mid] == x) return mid;
    else if( x < arr[mid]) {
        return fun(arr, x, low, mid-1);
    }
    else {
        return fun(arr, x, mid+1, high);
    }
}

int BS(vector<int> arr, int x) {
    int n = arr.size();
    
    return fun(arr, x, 0, n-1);
}

void T() {
    vector<int> v = {1,2,4,5,6};
    cout << BS(v, 2) << "\n";
    cout << BS(v, -1) << "\n";
    cout << BS(v, 7) << "\n";
}

int main() {
    T();
    int n = 2, m = 3;

    int **mat = new int*[2];
    for(int i=0; i<n; ++i) {
        mat[i] = new int[m];

    }

    for (int i=0; i<n; ++i) {
        delete[] mat[i];
    }
    delete[] mat;

    Car *myCar = new Tata();
    myCar->drive();


    return 0;
}