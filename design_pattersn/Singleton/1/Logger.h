#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
using namespace std;

class Logger {
private:
    static int ctr;
    static mutex mtx;
    static Logger *loggerInstance;
    Logger();

public:
    static Logger* getLogger();
    void Log(string msg);
};
#endif