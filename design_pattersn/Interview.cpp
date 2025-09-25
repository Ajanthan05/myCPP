#include <iostream>
#include <memory>
using namespace std;

class IEngine {
public:
    virtual void start() = 0;
    virtual ~IEngin() = default; 
};

class PetrolEngine : public IEngine {
public:
    void start() override { cout << "Petrol Engin\n"; }
};

class ElectricEngine : public IEngine {
public:
    void start() override { cout << "Petrol Engin\n"; }
};

class Car {
    std::unique_ptr<IEngine> engine;
public:
    Car(std::unique_ptr<IEngine> e) : engine(std::move(e)) {
        void drive() { engine->start(); }
    }
};

void T() {
    Car petrolCar(std::make_unique<PetrolEngine>());
    petrolCar.drive();
}
/*  The degree of dependency between two modules/classes.

Tight coupling → one class heavily depends on another’s implementation. 
Changes in one often break the other.

Loose coupling → classes interact via interfaces/abstractions, so changes in one 
don’t force changes in the other

Coupling = dependency between classes.
Decoupling = reducing that dependency (via abstraction/patterns).


Composition (has-a relationship)
    A class contains objects of other classes to reuse functionality.
    Pros: Flexible, can change behavior at runtime (swap components).
    Cons: More boilerplate (delegation).

***** Rule of thumb:
    Use inheritance when there’s a clear is-a relationship and polymorphism is needed.
    Use composition when you want flexibility and loose coupling.*/




/*  When to use Observer

Use when one-to-many objects must be notified of state changes (e.g., UI event listeners, publish/subscribe).

Strategy vs State

Strategy: encapsulates interchangeable algorithms; client picks strategy.

State: object behavior changes with internal state; the object switches state objects.

Game engine patterns

Adding new game objects at runtime: Factory (registration/factory method) or Prototype with cloning.

Handling events: Observer / Event Bus (or Message Queue) for decoupled event delivery.

Multiple loggers (console, file, network)

Use Observer or a Composite/Chain of Responsibility: notify multiple sinks; CoR useful if you want filtering/short-circuiting.

Singleton + lazy initialization

Use function-local static (Meyers’) or std::call_once/std::once_flag to initialize safely on first use.

Decorator vs Subclassing

Decorator: add responsibilities at runtime via composition — flexible, avoids class explosion.

Subclassing: compile-time, simpler but leads to rigid hierarchies and combinatorial subclasses.

Command pattern for Undo/Redo

Wrap each action in a Command with execute() and undo(); push commands onto history stack for undo and a redo stack for redo.

Singleton anti-pattern in high-performance systems

Avoid hidden globals: prefer dependency injection, immutable objects, or explicitly-initialized single instances; if needed, use thread-local or read-only singletons and ensure lock-free/low-overhead access.

Combine Builder and Factory Method

Use Factory Method (or Abstract Factory) to choose the appropriate Builder subclass; Builder then constructs the complex object step-by-step.

Proxy in microservices

Use a proxy (API Gateway / client-side proxy) to add caching, auth, rate-limiting, or retries and to hide network details from clients.

Mediator vs Observer

Mediator: centralizes complex interactions and protocols between many colleagues.

Observer: simple one-to-many notification; use when no central orchestration is required.

Double-Checked Locking pitfalls

It can fail due to instruction reordering and visibility; in C++ use proper memory ordering (std::atomic with acquire/release) or avoid it and use std::call_once/function-local static.*/
int main() {

    }