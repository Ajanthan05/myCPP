#include <iostream>
#include <vector>
#include <climits>
#include <thread>
#include <utility>
#include <mutex>
#include <memory>

using namespace std;

class Child;

// class Base {
// public:

//     Base(std::shared_ptr<Child> p) : cptr(p) {
//         cout << "COnstruct Base\n";
//     }

//     std::shared_ptr<Child> cptr;

// };

// class Child {
// public:
//     Child(std::shared_ptr<Base> p) : bptr(p) {
//         cout << "Construct child\n";
//     }

//     std::weak_ptr<Base> bptr;
// };

std::mutex m1;
int val = 0;

void Thread() {
    m1.lock();
    val++;
    m1.unlock();

}

void add(int &i, int &j) { a+b;}

void T() {

    std::thread t1(add, std::ref(a), std::ref(b));


    t1.join();

    int pipfd[2];
    if (pipe(pipfd) == -1) {

    }

    pid_t pid = fork();
    if (pid == -1) {

    }


}

int main() {

    Base *cptr = new Child(cptr);

    // int** row = new int*[3];
    // for(int i=0; i<3; ++i) {
    //     row[i] = new int[3];
    // }
    
    // for(int i=0; i<3; ++i) {
    //     for(int j=0; j<3; ++j) {
    //         row[i][j] = i*j + j +1;
    //     }
    // }
    // for(int i=0; i<3; ++i) {
    //     for(int j=0; j<3; ++j) {
    //         cout << row[i][j] << " ";
    //     }
    // }

    // for(int i=0; i<3; ++i) {
    //     delete[] row[i];
    // }
    // delete[] row;

    return 0;
}