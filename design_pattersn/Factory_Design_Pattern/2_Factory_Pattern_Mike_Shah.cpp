#include <iostream>
#include <vector>
#include <memory>
#include <map>

using namespace std;

class IGameObject {
public:
    virtual ~IGameObject() {}

    virtual void ObjectPlayDefaultAnimation() = 0;
    virtual void ObjectMoveIngame() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
};

class ObjectType1 : public IGameObject {
public:
    ObjectType1(int x, int y) {}

    void ObjectPlayDefaultAnimation() {}
    void ObjectMoveIngame() {}
    void Update() {}
    void Render() {}
};

class ObjectType2 : public IGameObject {
public:
    ObjectType2(int x, int y) {}

    void ObjectPlayDefaultAnimation() {}
    void ObjectMoveIngame() {}
    void Update() {}
    void Render() {}
};

class Plane : public IGameObject {
public:
    Plane(int x, int y) {}

    void ObjectPlayDefaultAnimation() {}
    void ObjectMoveIngame() {}
    void Update() {}
    void Render() {}
};

class Boat : public IGameObject {
public:
    Boat(int x, int y) {}

    void ObjectPlayDefaultAnimation() {}
    void ObjectMoveIngame() {}
    void Update() {}
    void Render() {}
};

enum class ObjectType { PLANE, BOAT };


std::shared_ptr<IGameObject> CreateObjectFactory(ObjectType o) {
    switch (o)
    {
        case ObjectType::PLANE:
            return make_shared<Plane>(0, 0);  
        case ObjectType::BOAT:
            return make_shared<Boat>(0, 0);    
        default:
            break;
    }
}

int T() {
    vector<shared_ptr<IGameObject>> gameObjectCollection;
    gameObjectCollection.push_back(CreateObjectFactory(ObjectType::PLANE));

    while(true) {
        for(auto& e : gameObjectCollection) {
            e->Update();
            e->Render();
        }
    }

    return 0;
}



class Ant : public IGameObject {
public:
    Ant(int x, int y) {
        ObjectsCreated++;
    }

    void ObjectPlayDefaultAnimation() {}
    void ObjectMoveIngame() {}
    void Update() {}
    void Render() {}

    static IGameObject* Create() {
        return new Ant(0, 0);
    }
// private:
    static int ObjectsCreated;
};
int Ant::ObjectsCreated = 0;



class MyGameObjectFactory {
public:
    // Callback function for creating object
    typedef IGameObject *(*CreateObjectCallback)();
    static void registerObject(const std::string& type, CreateObjectCallback cb) {
        s_Objects[type] = cb;
    }

    static void UnregisterObjct(const std::sting& type) {
        s_Objects.erase(type);
    }

    static IGameObject* CreaateSingleObject(const std::string& type) {
        CallBackMap::iterator it = s_Objects.find(type);
        if(it != s_Objects.end()) {
            // Call the CallBack function
            return (it->second)();
        }
        return nullptr;
    }

private:
    typedef std::map<std::string, CreateObjectCallback> CallBackMap;
    // Map of all the different objects that we can create
    static CallBackMap s_Objects;
}

// Abstract factory, extensible factory, distributed factory
int main() {

}