#include <iostream>
#include <thread>
#include <mutex>

#include <condition_variable>
using namespace std;

/*  CV are used in 2 purpose
1) Notify other threads a. notify_one(), b. notify_all()
2) Waiting for some condition*/

std::condition_variable cv;
std::mutex m;
long balance = 0;

void addMoney(int money) {
    std::lock_guard<mutex> lg(m);
    balance += money;
    cv.notify_one();
}

void widrowMoney(int money) {
    std::unique_lock<mutex> ul(m);
    cv.wait(ul, [] () { return (balance != 0) ? true : false; });
    if(balance >= money) {
        balance -= money;
        cout<< "Amount Deducted: " << money << "\n";
    } else {
        cout<< "can't Deducted. Balance Is: " << balance << "\n";
    }
    cout<< "Current Balance: " << balance << "\n";
}

int main() {
    std::thread t1(widrowMoney, 500);
    std::thread t2(addMoney, 500);
    t1.join();
    t2.join();

    return 0;
}