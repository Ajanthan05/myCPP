// JOIN
// 1. Once athread is created we wait for this thread to finish by calling join() function on thread object
// 2. Double join will result into program termination.
// 3. If needed we should check thread is joinable before joining. => joinable()

// DETACH
// 1. This is used to detach newly created thread from the parent thread.
// 2. Check before detaching athread is joinable. Otherwise we may end up double detach will result into program termination
// 3. If we detached thread and main function is returning then teh detached thread exicution is suspended.

// NOTE
// Either join() or detach() shold be called on thread object, otherwise during thread object's destructor will
// terminate the program. Boc inside destructor it check if thread is still joinable? if yes then it's terminate the program



#include <iostream>
using namespace std;


int main() {


    return 0;
}