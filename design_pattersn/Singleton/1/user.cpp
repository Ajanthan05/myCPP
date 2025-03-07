#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <utility>
#include "Logger.h"

using namespace std;

/* Singleton is Creational Design Pattern 
Single instanc of a class (Only 1 object)

How many instence of a class is created 
Keep static instance
*/
void user1logs() {
    Logger* logger1 = Logger::getLogger();
    logger1->Log("This message from user 1");
}

void user2logs() {
    Logger* logger2 = Logger::getLogger();
    logger2->Log("This message from user 2");
}

int main() {

    // Logger* logger1 = new Logger();
    // logger1->Log("This message from user 1");

    // Logger* logger2 = new Logger();
    // logger2->Log("This message from user 2");
    /*
    New instance created. No of instances: 1
    This message from user 1
    New instance created. No of instances: 2
    This message from user 2
    */
    // Logger* logger1 = Logger::getLogger();
    // logger1->Log("This message from user 1");

    // Logger* logger2 = Logger::getLogger();
    // logger2->Log("This message from user 2");

    /*
    But this is not thread safe

    */
    thread t1(user1logs);
    thread t2(user2logs);

    t1.join();
    t2.join();

    return 0;
}