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
    Logger(const Logger &); // Copy constructor as private
    Logger operator=(const Logger &); // operater overaloading as private

public:
    static Logger* getLogger();
    void Log(string msg);
};
#endif