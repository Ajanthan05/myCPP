#include <iostream>
#include <vector>

/* TC = O(N)
   SC = O(n) + O(N) // RECUTION STACK SPACE + aRRAY
*/
int fibonacci(int n, std::vector<int> &dp) {
    if (n <= 1) return n;

    if (dp[n] != -1) return dp[n];

    dp[n] = fibonacci(n-1, dp) + fibonacci(n-2, dp);
    return dp[n];

}




int main() {

    int n = 10;
    std::vector<int> dp(n+1, -1);
    std::cout << fibonacci(n, dp) << "\n";


/* Recurtion (Top-Down) -> Tabulation( Bottom-Up) 
    TC = O(N)
    SC = O(n) // No RECUTION STACK SPACE
*/
    int prev2 = 0;
    int prev = 1;
    for(int i=2; i<=n; i++) {
        int curi = prev + prev2;
        prev2 = prev;
        prev = curi;
    }
    std::cout << prev << "\n"; 

    return 0;
}