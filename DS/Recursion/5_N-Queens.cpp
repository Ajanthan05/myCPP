#include <iostream>
#include <vestor>
#include <string>

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

        col = dupclo;
        row = duprow;
        while(col>=0) {
            if(board[row][col] == 'Q') return false;
            col--;
        }

        col = dupclo;
        row = duprow;
        while(row<n && col>=0) {
            if(board[row][col] == 'Q') return false;
            row++;
            col--;
        }
        return true;
    }

    void solv(int col, vector<string> &board,vector<vector<string>> ans, int n) {
        if(col == n) {
            ans.push_back(board);
            return;
        }

        for (int row=0; row<n; row++) {
            if(isSafe(row, cl, board, n)) {
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

}


int main() {


    return 0;
}