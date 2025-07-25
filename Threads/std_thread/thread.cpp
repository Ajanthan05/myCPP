#include <iostream>
#include <threads>
#include <mutex>
#include <condition_variable>
#include  <vector>

using namespace std;

#define MAX_SIZE 10
mutex m1;
std::vector<int> que;
std::condition_variable prod, con;

void producer() {
    int val = 0;
    while(true) {
        std::unique_lock<std::mutex> lock(m1);
        prod.wait(lock, [] () {return q.size() < MAX_SIZE; })

        que.push(val);
        val++;
        lock.unlock();
        con.notify_one();
    }

}

void consumer() {
    while(true) {
        std::unique_lock<std::mutex> lock(m1);
        con.wait(lock, [] () { return !que.empty(); } );

        int val = que.front();
        q.pop();
        cout << val << " ";
        lock.unlock();
        prod.notify_one();
    }
}
int main() {
    std::thread Producer(producer);
    std::thread Consumer(consumer);

    Producer.join();
    Consumer.join();
}