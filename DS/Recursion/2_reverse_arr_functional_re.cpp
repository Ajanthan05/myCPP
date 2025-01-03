#include <iostream>
#include <string>
#include <vector>

void f(int l, int r, int arr[]) {
    if (l >= r) return;

    std::swap(arr[l], arr[r]);
    f(l+1, r-1, arr);
}

void f2(int l, int n, int arr[]) {
    if (l >= n/2) return;

    std::swap(arr[l], arr[n-l-1]);
    f2(l+1, n, arr);
}

/* Check if a String is Palindrome 
TC = SC = n/2 */
bool palindrome(int i, std::string s) {
    int n = s.length();
    if (i >= n/2) return true;

    if (s[i] != s[n-i-1]) return false;

    return palindrome(i+1, s);
}

/* Fibonacci 
0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55

f[0] = 0, f[1] = 1;
for(i=2 to n)
    f[i] = f[i-1] + f[i-2];
*/

int Fibonacci(int n) {
    if (n <= 1) return n;

    int last = Fibonacci(n-1);
    int slast = Fibonacci(n-2);

    return last + slast;
}

/* TC = O(2^n * n)  => n to print
    SC = O(n) */
void Subsequences( int ind, std::vector<int> &ds, int arr[], int n) {
    if (ind >= n) {
        for (auto it : ds) std::cout << it << " ";
        if (ds.size() == 0) std::cout << "{}";
        std::cout << "\n";
        return;
    }

    // Take or pick tehe particular index into the subsequence
    ds.push_back(arr[ind]);
    Subsequences(ind+1, ds, arr, n);  
    // Not Take or pick tehe particular index into the subsequence 
    ds.pop_back();
    Subsequences(ind+1, ds, arr, n); 

    // We can reverse it also
    /*
    // Take or pick tehe particular index into the subsequence
    Subsequences(ind+1, ds, arr, n); 
    ds.push_back(arr[ind]);
    Subsequences(ind+1, ds, arr, n);  
    // Not Take or pick tehe particular index into the subsequence 
    ds.pop_back();
    */    
}

void Sum_Of_Subsequences( int ind, std::vector<int> &ds, int arr[], int n, int sum, int s) {
    if (ind == n) {
        if (s == sum) {
            for (auto it : ds) std::cout << it << " ";
            if (ds.size() == 0) std::cout << "{}";
            std::cout << "\n";
        }
        return;  
    }

    // Take or pick tehe particular index into the subsequence
    ds.push_back(arr[ind]);
    s += arr[ind];
    Sum_Of_Subsequences(ind+1, ds, arr, n, sum, s);  
    // Not Take or pick tehe particular index into the subsequence 
    ds.pop_back();
    s -= arr[ind];
    Sum_Of_Subsequences(ind+1, ds, arr, n, sum, s); 
}

bool First_Sum_Of_Subsequences( int ind, std::vector<int> &ds, int arr[], int n, int sum, int s) {
    if (ind >= n) {
        // condition satisfied
        if (s == sum) {
            for (auto it : ds) std::cout << it << " ";
            if (ds.size() == 0) std::cout << "{}";
            std::cout << "\n";
            return true;
        }
        else return false;  
    }

    // Take or pick tehe particular index into the subsequence
    ds.push_back(arr[ind]);
    s += arr[ind];
    if (First_Sum_Of_Subsequences(ind+1, ds, arr, n, sum, s) == true) return true;  
    // Not Take or pick tehe particular index into the subsequence 
    ds.pop_back();
    s -= arr[ind];
    if (First_Sum_Of_Subsequences(ind+1, ds, arr, n, sum, s) == true) return true;

    return false; 
}

int Count_Sum_Of_Subsequences( int ind, int arr[], int n, int sum, int s) {
    // To reduce TC
    if (s > sum) return 0; // Strictly done if array contains positives only

    if (ind == n) {
        if (s == sum) return 1;
        else return 0;  
    }

    // Take or pick tehe particular index into the subsequence
    s += arr[ind];
    int left = Count_Sum_Of_Subsequences(ind+1, arr, n, sum, s);  
    // Not Take or pick tehe particular index into the subsequence 
    s -= arr[ind];
    int right = Count_Sum_Of_Subsequences(ind+1, arr, n, sum, s); 

    return left + right;
}

int main() {

    int arr[] = {1,2,3,4,5};

    f(0, 4, arr); 
    for (int i=0; i<std::size(arr); i++) std::cout << arr[i] <<  " "; 
    std::cout << std::endl;

    f2(0, std::size(arr), arr); 
    for (int i=0; i<std::size(arr); i++) std::cout << arr[i] <<  " "; 
    std::cout << std::endl;

    std::string str = "MADAm";
    std::cout << palindrome(0, str) << std::endl;

    std::cout << "Fibonacci: " << Fibonacci(10) << std::endl;


// Subsequences
    int arr_[] = {3, 1, 2};
    int n = 3;
    std::vector<int> ds;
    Subsequences(0, ds, arr_, n);

    std::cout << "\n";

    int arr2[] = {1, 2, 1};
    int n2 = 3;
    int sum = 2;
    std::vector<int> ds2;
    Sum_Of_Subsequences(0, ds2, arr2, n2, sum, 0);

    std::cout << "\n";

    int arr3[] = {1, 2, 1};
    int n3 = 3;
    int sum3 = 2;
    std::vector<int> ds3;
    First_Sum_Of_Subsequences(0, ds3, arr3, n3, sum3, 0);

    std::cout << "\nCount_Sum_Of_Subsequences\n";

    int arr4[] = {1, 2, 1};
    int n4 = 3;
    int sum4 = 2;
    std::cout << Count_Sum_Of_Subsequences(0, arr4, n4, sum4, 0) << "\n";
    
    return 0;
}