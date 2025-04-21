#include <iostream>
#include <vector>
#include <string>
using namespace std;

class N_Queens {
public:
    bool isSafe(int row, int col, std::vector<std::string> board, int n) {
        int duprow = row;
        int dupcol = col;

        while(row >= 0 && col >= 0) {
            if (board[row][col] == 'Q') return false;
            row--;
            col--;
        }

        col = dupcol;
        row = duprow;
        while(col>=0) {
            if(board[row][col] == 'Q') return false;
            col--;
        }

        col = dupcol;
        row = duprow;
        while(row<n && col>=0) {
            if(board[row][col] == 'Q') return false;
            row++;
            col--;
        }
        return true;
    }

    void solve(int col, vector<string> &board,vector<vector<string>> &ans, int n) {
        if(col == n) {
            ans.push_back(board);
            return;
        }

        for (int row=0; row<n; row++) {
            if(isSafe(row, col, board, n)) {
                board[row][col] = 'Q';
                solve(col+1, board, ans, n);
                board[row][col] = '.';
            }
        }
    }
    
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> ans;
        vector<string> board(n);
        std::string s(n, '.');
        for(int i=0; i<n; i++) {
            board[i] = s;
        }

        solve(0, board, ans, n);
        return ans;
    }
};


/* OPTIMAL */
class Solution {
    public:
       void solve(int col, vector<vector<string>> &ans, vector<string> &board, 
                  vector<int> &leftrow, vector<int> &upperdiagonal, vector<int> &lowerdiagonal, int n) {
           
           if(col == n) {
               ans.push_back(board);
               return;
           }
           
           for(int row = 0; row < n; row++) {
               if(leftrow[row] == 0 && lowerdiagonal[row + col] == 0 && upperdiagonal[n-1 + col - row] == 0) {
                   
                   board[row][col] = 'Q';
                   leftrow[row] = 1;
                   lowerdiagonal[row + col] = 1;
                   upperdiagonal[n-1 + col - row] = 1;
                   
                   solve(col + 1, ans, board, leftrow, upperdiagonal, lowerdiagonal, n);
                   
                   board[row][col] = '.';
                   leftrow[row] = 0;
                   lowerdiagonal[row + col] = 0;
                   upperdiagonal[n-1 + col - row] = 0;
               }
           }
       }
    
       vector<vector<string>> solveNQueens(int n) {
           vector<vector<string>> ans;
           vector<string> board(n, string(n, '.'));  
           
           vector<int> leftrow(n, 0), upperdiagonal(2 * n - 1, 0), lowerdiagonal(2 * n - 1, 0);
           
           solve(0, ans, board, leftrow, upperdiagonal, lowerdiagonal, n);
           
           return ans;
       }
    };

int main() {


    return 0;
}