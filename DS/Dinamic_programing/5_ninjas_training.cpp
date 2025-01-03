#include <iostream>
#include <vector>
#include <climits>

int f(int day, int last, std::vector<std::vector<int>> &points) {
    int maxi = 0;
    if (day == 0) {

        for (int task=0; task<3; task++) {
            if (task != last) 
                maxi = std::max(maxi, points[0][task]);
        }
        return maxi;
    }

    for (int task=0; task<3; task++) {
        if (task != last) {
            int point = points[day][task] + f(day-1, task, points);
            maxi = std::max(maxi, point);
        }
    }
    return maxi;
}

/* MEMOIZATION 
TC = O(N*4*3)
SC = O(N) + O(N*4);
*/
int f_dp(int day, int last, std::vector<std::vector<int>> &points, std::vector<std::vector<int>> &dp) {
    int maxi = 0;
    if (day == 0) {
        for (int task=0; task<3; task++) {
            if (task != last) 
                maxi = std::max(maxi, points[0][task]);
        }
        return maxi;
    }

    if (dp[day][last] != -1) return dp[day][last];
    for (int task=0; task<3; task++) {
        if (task != last) {
            int point = points[day][task] + f_dp(day-1, task, points, dp);
            maxi = std::max(maxi, point);
        }
    }
    return dp[day][last] = maxi;
}

/* TABULATION 
TC = O(N*4*3)
SC = O(N*4);
*/
int f_T(int days, int last, std::vector<std::vector<int>> &points) {

    std::vector<std::vector<int>> dp(days, std::vector<int>(4, 0));

    dp[0][0] = std::max(points[0][1], points[0][2]);
    dp[0][1] = std::max(points[0][0], points[0][2]);
    dp[0][2] = std::max(points[0][0], points[0][1]);
    dp[0][3] = std::max(points[0][1], std::max(points[0][2], points[0][3]));

    for (int day=1; day<days; day++) {
        for (int last=0; last<=3; last++) {
            // dp[day][last] = 0;
            // int maxi = 0;
            for (int task=0; task<3; task++) {
                if (task != last) {
                    int point = points[day][task] + dp[day-1][task];
                    dp[day][last] = std::max(dp[day][last], point);
                }
            } 
        }
    }
    return dp[days-1][3];  // buttom up , dp[days-1][3] this is the initial call
}

/* TABULATION => space optimization 
TC = O(N*4*3)
SC = O(4);
*/
int f_T_SO(int days, int last, std::vector<std::vector<int>> &points) {

    // std::vector<std::vector<int>> dp(days, std::vector<int>(4, 0));
    std::vector<int> prev(4, 0);

    prev[0] = std::max(points[0][1], points[0][2]);
    prev[1] = std::max(points[0][0], points[0][2]);
    prev[2] = std::max(points[0][0], points[0][1]);
    prev[3] = std::max(points[0][1], std::max(points[0][2], points[0][3]));

    for (int day=1; day<days; day++) {
        std::vector<int> today_task(4, 0);
        for (int last=0; last<=3; last++) {
            today_task[last] = 0;
            for (int task=0; task<3; task++) {
                if (task != last) {
                    // int point = points[day][task] + prev[task];
                    today_task[last] = std::max(today_task[last], points[day][task] + prev[task]);
                }
            } 
        }
        prev = today_task;
    }
    return prev[3];  // buttom up , dp[days-1][3] this is the initial call
}

int main() {

    std::vector<std::vector<int>> points = { {2,1,3}, {3,4,6}, {10,1,6}, {8,3,7}};
    int n = 4;
    std::cout << f(n-1, 3, points) << "\n";

    std::vector<std::vector<int>> points2 = { {2,1,3}, {3,4,6}, {10,1,6}, {8,3,7}};
    int n2 = 4;
    std::vector<std::vector<int>> dp(n, std::vector<int>(4, -1));
    std::cout << f_dp(n2-1, 3, points2, dp) << "\n";

    std::vector<std::vector<int>> points3 = { {2,1,3}, {3,4,6}, {10,1,6}, {8,3,7}};
    int n3 = 4;
    std::cout << f_T(n3, 3, points3) << "\n";

    std::vector<std::vector<int>> points4 = { {2,1,3}, {3,4,6}, {10,1,6}, {8,3,7}};
    int n4 = 4;
    std::cout << f_T_SO(n4, 3, points4) << "\n";
    return 0;
}