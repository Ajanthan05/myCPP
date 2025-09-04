#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;


/* 
for outer loop count number of lines

For the inner loop, focus on the columns
and connect them somehow to the rows

Print them inside the inner for loop

Observe symmetry
*/

/* 

    *    
   ***   
  *****  
 ******* 
*********
Space,    Star,   Space
(n-i-1),  (2i+1), (n-i-1)
*/
void Pattern1(int n) {
    for(int i=0; i<n; ++i) {
        // Space
        for(int j=0; j<n-i-1; ++j) {
            cout << " ";
        }
        // Star
        for(int j=0; j<2*i+1; ++j) {
            cout << "*";
        }
        // Space
        for(int j=0; j<n-i-1; ++j) {
            cout << " ";
        }
        cout << "\n";
    }   
}

/*
*********
 ******* 
  *****  
   ***   
    *  
*/
void Pattern2(int n) {
    for(int i=0; i<n; ++i) {
        // Space
        for(int j=0; j<i; ++j) {
            cout << " ";
        }
        // Star
        for(int j=0; j<2*n - (2*i + 1); ++j) {
            cout << "*";
        }
        // Space
        for(int j=0; j<i; ++j) {
            cout << " ";
        }
        cout << "\n";
    }   
}

/* OBSERVE SYMETRY (2n-1)
*
**
***
****
*****
****
***
**
*
*/
void Pattern3(int n) {
    for(int i=0; i<=2*n-1; ++i) {
        int stars = i;
        if (i > n) stars = 2*n - i;
        for(int j=1; j<=stars; ++j) {
            cout << "*";
        }
        cout << "\n";
    }   
}

/*
1
01
101
0101
10101
*/
void Pattern4(int n) {
    int start = 1;
    for(int i=0; i<n; ++i) {
        if (i % 2 == 0) start = 1;
        else start = 0;
        for(int j=1; j<=i+1; ++j) {
            cout << start;
            start = 1 - start;
        }
        cout << "\n";
    }
}

/*
1
2 3
4 5 6 
7 8 9 10
11 12 13 14 15
*/
void Pattern5(int n) {
    int start = 1;
    for(int i=1; i<=n; ++i) {
        for(int j=1; j<=i; j++) {
            cout << start++ << " ";
        }
        cout << "\n";
    }
}

/*
A
AB
ABC
ABCD
ABCDE
*/

void Pattern6(int n) {
    for(int i=0; i<n; ++i) {
        for(char ch = 'A'; ch <= 'A' + i; ch++) {
            cout << ch;
        }
        cout << "\n";
    }
}


void Pattern1(int n) {
    for(int i=0; i<n; ++i) {
        // Space
        for(int j=0; j<n-i-1; ++j) {
            cout << " ";
        }
        // Char
        
        for(int j=0; j<2*i+1; ++j) {
            cout << "*";
        }
        // Space
        for(int j=0; j<n-i-1; ++j) {
            cout << " ";
        }
        cout << "\n";
    }   
}

int main() {
    Pattern1(5);
    cout << "\n\n";

    Pattern2(5);
    cout << "\n\n";

    Pattern3(5);
    cout << "\n\n";

    Pattern4(5);

    cout << "\n\n";

    Pattern5(5);
    cout << "\n\n";

    Pattern6(5);

    return 0;
}