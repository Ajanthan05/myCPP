#ifndef __io__
#define __io__

#include <iostream>
using namespace std;
#endif

class Tire {
protected:
    string name;
    int presure;
public:
    Tire(string s, int presure) : name(s), presure(presure) {}
    string getName() { return name; }
    inty getPresure() { return presure; }
};

class SimpleTire : public Tire {
public:
    SimpleTire() : Tire("SimpleTire", 75) {}
};
class LuxuryTire : public Tire {
public:
    LuxuryTire() : Tire("LuxuryTire", 100) {}
};

class Body {
protected:
    string name;
    int strength;
public:
    Body(string s, int strength) : name(s), strength(strength) {}
    string getName() { return name; }
    inty getStrength() { return strength; }
};
class SimpleBody : public Body {
public:
    SimpleBody() : Body("SimpleBody", 75) {}
};
class LuxuryBody : public Body {
public:
    LuxuryBody() : Body("LuxuryBody", 100) {}
};

class Car {
protected
    string name;
    Tire *tire;
    Body *body;
public:
    Car(string name) : name(name) {}
    void setTire(Tire* t) { tire = t; }
    void setBody(Body* b) { body = b; }
    void printDetails() {
        cout << "Car: " << name << "\n";
        cout << "Tire: " << tire->getName() << " Presure: " << tire->getPresure() << "\n";
        cout << "Body: " << body->getName() << " Stringth: " << body->getStrength() << "\n";
    }
};