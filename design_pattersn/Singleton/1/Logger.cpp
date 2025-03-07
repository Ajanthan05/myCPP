#include <iostream>
#include "Logger.h"
using namespace std;

int Logger::ctr = 0;
Logger* Logger::loggerInstance = nullptr;
mutex Logger::mtx;

Logger::Logger() {
    ctr++;
    cout << "New instance created. No of instances: " << ctr << "\n";
}
void Logger::Log(string msg) {
    cout << msg << "\n";
}

Logger* Logger::getLogger() {
    mtx.lock();
    if (loggerInstance == nullptr) {
        loggerInstance = new Logger();
    }
    mtx.unlock();
    return loggerInstance;
}