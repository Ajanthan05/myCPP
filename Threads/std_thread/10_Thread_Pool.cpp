#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include <queue>

#include <sstream> //stringstream
#include <condition_variable>
using namespace std;
#include <future>

class ThreadPool {
public:
    ThreadPool(size_t numThread) : stop(false) {
        for(size_t i=0; i<numThread; ++i) {
            workers.emplace_back([this] {
                for(;;) {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this] { return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) return;
                    auto task = std::move(tasks.front());
                    tasks.pop();
                    lock.unlock();
                    task();
                }
            });
        }
    }

    template<class F>
    void enqueue(F&& task) {
        std::unique_lock<std::mutex> lock(queueMutex);
        task.emplase(std::forward<F>(task));
        lock.unlock();
        condition.notify_one();  // notify any one
    }

    ~ThreadPool() {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
        lock.unlock();
        condition.notify_all();
        for(std::thread& worker : workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

std::string get_thread_id() {
    auto myid = std::this_thread::get_id();
    std::stringstream ss;
    ss << myid;
    std::string mystr = ss.str();
    ss << myid;
    std::string mystfr = ss.str();
    return mystr;
}

int main() {
    ThreadPool pool(4); // Create a pool with n num of woker thread

    for(int i=0; i<8; ++i) {
        pool.enqueue([i] {
            cout << "Task: " << i << get_thread_id().c_str() << "executed by thread\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }

    return 0;
}