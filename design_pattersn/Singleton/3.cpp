#include <iostream>
#include <string>
#include <fstream>
#include <mutex>
using namespace std;

class GameSetting {
    static GameSetting *_instance;
    int _power;
    GameSetting() : _power(0) {}  // default constructor
    GameSetting(int p) : _power(p) {}
public:
    static GameSetting* getInstance() {
        if(_instance == NULL) {
            _instance = new GameSetting();
        }
        return _instance;
    }
    void setPower(int p) { _power = p; }

    int getPower() { return _power; }

    void displaySetting() {
        cout << "Power: " << _power << "\n";
    }
};
GameSetting * GameSetting::_instance = nullptr;

void T() {
    GameSetting *setting = GameSetting::getInstance();
    setting->setPower(10);
    setting->displaySetting();
}

/*  🔹 The Problem With Classic Singleton (your version)

Uses raw pointers (new) → memory leaks if not deleted.

Needs manual delete.

Not thread-safe unless extra code is added.

NULL / nullptr checks every time.

Constructor has to be private, but people sometimes misuse it.*/

class GameSetting2 {
    int _power;
    GameSetting2(int p = 0) : _power(p) {}   // private constructor with default
public:
    GameSetting2(const GameSetting2&) = delete;            // no copy
    GameSetting2& operator=(const GameSetting2&) = delete; // no assignment

    static GameSetting2& getInstance() {
        static GameSetting2 instance(0);  // created once, destroyed at program end
        return instance;
    }

    void setPower(int p) { _power = p; }
    int getPower() { return _power; }

    static int Power() { return getInstance().getPower(); }

    void displaySetting() {
        std::cout << "Power: " << _power << "\n";
    }
};
/*  🔹 Why is this good?

Thread-safe (C++11 and later):
The standard guarantees that static local variables are initialized exactly once, even in multi-threaded programs.

Automatic cleanup:
instance is destroyed when the program exits → no memory leaks.

No raw pointers:
No new or delete.*/
void mayers_singleton() {
    GameSetting2 &setting = GameSetting2::getInstance();
    setting.setPower(10);
    setting.displaySetting();

    // Another reference points to the same instance
    GameSetting2 &setting2 = GameSetting2::getInstance();
    setting2.displaySetting(); // still Power: 10

// Simple syntax:
    GameSetting2::getInstance().setPower(10);
    GameSetting2::getInstance().displaySetting();

    cout << GameSetting2::Power() << "\n";
}




/*  🔹 Why use Singleton for Logging?

Single access point → All parts of your application log through the same instance.

Consistency → Avoid multiple log files with different formats.

Resource management → File handles, DB connections, etc., are limited.

Thread-safety → Prevent race conditions when multiple threads log simultaneously.*/
class Logger {
    std::ofstream logFile;
    std::mutex mtx;

    // Private constructor so no external object can be created
    Logger() {
        logFile.open("db_app.log", std::ios::app);  // append mode
        if (!logFile.is_open()) {
            throw std::runtime_error("Unable to open log file");
        }
    }

    ~Logger() {
        if (logFile.is_open()) logFile.close();
    }

public:
    Logger(const Logger&) = delete;            // no copy
    Logger& operator=(const Logger&) = delete; // no assignment

    static Logger& getInstance() {
        static Logger instance;  // Meyers’ Singleton
        return instance;
    }

    void log(const std::string& msg) {
        std::lock_guard<std::mutex> lock(mtx);  // thread-safe
        logFile << msg << std::endl;
    }
};
class Database {
public:
    void connect() {
        Logger::getInstance().log("Database connected.");
    }

    void executeQuery(const std::string& query) {
        Logger::getInstance().log("Executing query: " + query);
    }

    void disconnect() {
        Logger::getInstance().log("Database disconnected.");
    }
};
void LoggingClass() {
    Database db;
    db.connect();
    db.executeQuery("SELECT * FROM users;");
    db.disconnect();
}

int main() {
    mayers_singleton();

    return 0;
}