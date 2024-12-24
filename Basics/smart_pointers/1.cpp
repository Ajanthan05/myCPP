#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

class Wrapper
{
private:
    int *mem;
public:
    Wrapper(int *a) : mem(a)  // we r initializing row pointer inside the constructor => resource acquisition incide initialization
    {
        cout << "inside constructor\n";  
    }
    ~Wrapper()
    {
        cout << "distructor\n";
        delete mem;
    }
};

/* UNIQUE PTR =>  perticular resource is exclusively owned by only
by only perticular object at that point of tine you will use unique PTR for that reources 

SHARED_PTR => ONE PERTICULAR RESOURCE shared by multople objects
*/

int main() {
    Wrapper obj(new int(10));    // obj will be in stack memory it will get deleted automatically when it go out of scope

    return 0;
}