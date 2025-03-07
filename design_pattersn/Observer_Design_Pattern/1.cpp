#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <list>
#include <algorithm>

using namespace std;

class ISubscribers {
public: 
    virtual void notify(string msg) = 0;
};

class User : public ISubscribers {
private:
    int userId;
public:
    User(int userId) {
        this->userId = userId;
    }

    void notify(string msg) {
        cout << "User " << userId << " received msg " << msg << endl;
    }
};

class Group {
private:
    list<ISubscribers*> users;
public:
    void subscribe(ISubscribers* subscriber) {
        users.push_back(subscriber);
    }
    void unsubscribe(ISubscribers* subscriber) {
        users.remove(subscriber);
    }
    void notify(string msg) {
        for(auto user : users)
            user->notify(msg);
    }
};

int main() {

    Group* group = new Group();

    User* user1 = new User(1);
    User* user2 = new User(2);
    User* user3 = new User(3);

    group->subscribe(user1);
    group->subscribe(user2);
    group->subscribe(user3);

    group->notify("new Msg");

    group->unsubscribe(user1);
    group->notify("new new Msg");
    
    return 0;
}