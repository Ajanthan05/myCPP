#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include <queue>

#include <sstream> //stringstream
#include <condition_variable>
using namespace std;
#include <future>

#include <memory>
// #include <future>

/* Each thread has it own stack
Global state of memory is shared

Propcess:

CODRE : Core count is the total number of instruction that can be 
executed simultaneously

Not all core are equal
    HyperThreading NUMA, AMP

RACE CONDITION when resource is accessed by multiple threads
simultaneously, and at least one access is a write
undefine behavior

Stack  is an area of used for data whose size is 
determine at compile time
It belong to a specific thread

HEAP:   An area of memory used for data which is allocated at 
runntime
Shared among all threads.

*/


class Pie {
public:
    void makeCrust() { std::cout << "Crust made.\n"; }
    void putApplesInPie() { std::cout << "Apples added.\n"; }
};

class Oven_o {
public:
    void bakePie(const Pie&, int temp, int minutes) {
        std::cout << "Baking pie at " << temp << "F for " << minutes << " minutes.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // simulate time
    }
};

template<typename T>
class threadsafe_queue {
private:
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void enqueue(T item) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(std::move(item));
        }
        cv.notify_one();
    }

    T dequeue() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !q.empty(); });

        T item = std::move(q.front());
        q.pop();
        return item;
    }
};

void T() {
    Oven_o vikingOven;
    threadsafe_queue<Pie> conveyorBelt;

    std::thread chef1([&]() {
        for (int i = 0; i < 50; ++i) {
            Pie pie;
            pie.makeCrust();
            pie.putApplesInPie();
            conveyorBelt.enqueue(std::move(pie));
        }
    });

    std::thread chef2([&]() {
        for (int i = 0; i < 50; ++i) {
            Pie pie = conveyorBelt.dequeue();
            vikingOven.bakePie(pie, 375, 35);
        }
    });

    chef1.join();
    chef2.join();
}

// Dummy food classes
class Food {
public:
    virtual std::string name() const = 0;
    virtual ~Food() = default;
};

class Pizza : public Food {
public:
    void addSauce() {}
    void addCheese() {}
    std::string name() const override { return "Pizza"; }
};

class GarlicKnots : public Food {
public:
    std::string name() const override { return "Garlic Knots"; }
};

class ApplePie : public Food {
public:
    std::string name() const override { return "Apple Pie"; }
};

class IceCream : public Food {
public:
    std::string name() const override { return "Ice Cream"; }
};

// Kitchen appliances
class Oven {
public:
    template<typename T>
    std::unique_ptr<T> bake(std::unique_ptr<T> food) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return food;
    }
};

class IceCreamMaker {
public:
    std::unique_ptr<IceCream> make() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return std::make_unique<IceCream>();
    }
};



// Type aliases
using PatronTicket = std::future<std::unique_ptr<Food>>;
using ChefTicket = std::promise<std::unique_ptr<Food>>;

struct Kitchen {
    Oven vikingOven;
    std::mutex vikingOven_mutex;

    Oven brickOven;
    std::mutex brickOven_mutex;

    IceCreamMaker iceCreamMaker;
    std::mutex iceCreamMaker_mutex;

    threadsafe_queue<std::function<void()>> orderQueue;
    std::atomic<bool> restaurantOpen{true};
};

/* Globals
Oven vikingOven;
std::mutex vikingOven_mutex;

Oven brickOven;
std::mutex brickOven_mutex;

IceCreamMaker iceCreamMaker;
std::mutex iceCreamMaker_mutex;

threadsafe_queue<std::function<void()>> orderQueue;
std::atomic<bool> restaurantOpen{true};*/

PatronTicket orderPizza(Kitchen& kitchen) {
    auto ticket = std::make_shared<ChefTicket>();
    PatronTicket patron = ticket->get_future();

    kitchen.orderQueue.enqueue([ticket, &kitchen]() {
        auto pizza = std::make_unique<Pizza>();
        pizza->addSauce();
        pizza->addCheese();
        {
            std::lock_guard<std::mutex> lock(kitchen.brickOven_mutex);
            pizza = kitchen.brickOven.bake(std::move(pizza));
        }
        ticket->set_value(std::move(pizza));
    });

    return patron;
}

PatronTicket orderGarlicKnots(Kitchen& kitchen) {
    auto ticket = std::make_shared<ChefTicket>();
    PatronTicket patron = ticket->get_future();
    kitchen.orderQueue.enqueue([ticket, &kitchen]() {
        auto knots = std::make_unique<GarlicKnots>();
        {
            std::lock_guard<std::mutex> lock(kitchen.vikingOven_mutex);
            knots = kitchen.vikingOven.bake(std::move(knots));
        }
        ticket->set_value(std::move(knots));
    });
    return patron;
}

PatronTicket orderApplePie(Kitchen& kitchen) {
    auto ticket = std::make_shared<ChefTicket>();
    PatronTicket patron = ticket->get_future();
    kitchen.orderQueue.enqueue([ticket, &kitchen]() {
        auto pie = std::make_unique<ApplePie>();
        {
            std::lock_guard<std::mutex> lock(kitchen.vikingOven_mutex);
            pie = kitchen.vikingOven.bake(std::move(pie));
        }
        ticket->set_value(std::move(pie));
    });
    return patron;
}

PatronTicket orderIceCream(Kitchen& kitchen) {
    auto ticket = std::make_shared<ChefTicket>();
    PatronTicket patron = ticket->get_future();
    kitchen.orderQueue.enqueue([ticket, &kitchen]() {
        std::unique_ptr<IceCream> iceCream;
        {
            std::lock_guard<std::mutex> lock(kitchen.iceCreamMaker_mutex);
            iceCream = kitchen.iceCreamMaker.make();  // ← fix: access via kitchen
        }
        ticket->set_value(std::move(iceCream));
    });
    return patron;
}


// Eat function
void eat(std::unique_ptr<Food> food) {
    std::cout << "Patron was served: " << food->name() << "\n";
}

// Entry point
void V2() {
    Kitchen kitchen;
    std::thread patron1([&kitchen] {
        PatronTicket knots = orderGarlicKnots(kitchen);
        PatronTicket pizza = orderPizza(kitchen);
        PatronTicket iceCream = orderIceCream(kitchen);
        eat(knots.get());
        eat(pizza.get());
        eat(iceCream.get());
    });

    std::thread patron2([&kitchen] {
        PatronTicket iceCream = orderIceCream(kitchen);
        PatronTicket pie = orderApplePie(kitchen);
        eat(iceCream.get());
        eat(pie.get());
    });

    std::thread chef1([&kitchen] {
        while (kitchen.restaurantOpen) {
            auto order = kitchen.orderQueue.dequeue();
            order();
        }
    });

    std::thread chef2([&kitchen] {
        while (kitchen.restaurantOpen) {
            auto order = kitchen.orderQueue.dequeue();
            order();
        }
    });

    patron1.join();
    patron2.join();

    // Close restaurant after patrons are done
    kitchen.restaurantOpen = false;

    // Optional: enqueue dummy orders to unblock chefs
    kitchen.orderQueue.enqueue([] {});
    kitchen.orderQueue.enqueue([] {});

    chef1.join();
    chef2.join();
}




std::unique_ptr<ComplicatedObject> createObject(int param1, double param2) {
    auto obj = std::make_unique<ComplicatedObject>();
    obj->doSomething(param1);      // if this throws, obj is destroyed
    obj->somethingElse(param2);    // also safe
    return obj;                    // moved out safely
}
/*  Benefits:
If an exception is thrown, obj is automatically deleted.

No manual memory management required.

Safe, clean, modern C++ style.

Cleaner and safer.
Avoids direct use of new.
Single allocation for both control block and object (performance benefit).
Works with constructor arguments:

auto obj = std::make_unique<ComplicatedObject>(arg1, arg2);*/

// 💡 Alternative: Return by Value (if cheap or movable)
// If ComplicatedObject is movable (has move constructor), return by value:

ComplicatedObject createObject(int param1, double param2) {
    ComplicatedObject obj;
    obj.doSomething(param1);
    obj.somethingElse(param2);
    return obj;  // RVO (Return Value Optimization)
}




// template <typename T, typename M = std::mutex>
// class guarded {
// public:
//     using handle = std::unique_ptr<T, deleter>;  // RAII smart-pointer with custom deleter

//     template <typename... Us>
//     guarded(Us&&... data);       // Forward constructor args to T

//     handle lock();               // Lock and return handle
//     handle try_lock();          // Try-lock
//     template <class Duration>
//     handle try_lock_for(const Duration&);
//     template <class TimePoint>
//     handle try_lock_until(const TimePoint&);

// private:
//     T m_obj;
//     M m_mutex;

//     struct deleter {
//         std::unique_lock<M> lk_;
//         deleter(std::unique_lock<M>&& lk) : lk_(std::move(lk)) {}
//         void operator()(T*) noexcept {} // Does nothing to T, only unlocks the mutex
//     };
// };

// template <typename T, typename M>
// template <typename... Us>
// guarded<T, M>::guarded(Us &&... data) : m_obj(std::forward<Us>(data)...)
// {
// }

// template <typename T, typename M>
// auto guarded<T, M>::lock() -> handle
// {
//     std::unique_lock<M> lock(m_mutex);
//     return handle(&m_obj, deleter(std::move(lock)));
// }

// template <typename T, typename M>
// auto guarded<T, M>::try_lock() -> handle
// {
//     std::unique_lock<M> lock(m_mutex, std::try_to_lock);
//     if (lock.owns_lock()) {
//         return handle(&m_obj, deleter(std::move(lock)));
//     } else {
//         return handle(nullptr, deleter(std::move(lock)));
//     }
// }

// class deleter {
// public:
//     using pointer = T *;
//     deleter(std::unique_lock<M> lock) : m_lock(std::move(lock)) { }

//     void operator()(T * ptr) {
//         if (m_lock.owns_lock()) {
//             m_lock.unlock();
//         }
//     }
// private:
//     std::unique_lock<M> m_lock;
// };




template <typename T, typename M = std::mutex>
class guarded {
public:
    class deleter {
    public:
        using pointer = T*;

        deleter(std::unique_lock<M> lock) : m_lock(std::move(lock)) {}

        void operator()(T* ptr) {
            if (m_lock.owns_lock()) {
                m_lock.unlock();  // unlock is safe even if ptr is null
            }
        }

    private:
        std::unique_lock<M> m_lock;
    };

    using handle = std::unique_ptr<T, deleter>;

    template <typename... Args>
    guarded(Args&&... args)
        : m_obj(std::forward<Args>(args)...) {}

    handle lock() {
        std::unique_lock<M> lock(m_mutex);
        return handle(&m_obj, deleter(std::move(lock)));
    }

    handle try_lock() {
        std::unique_lock<M> lock(m_mutex, std::try_to_lock);
        if (lock.owns_lock()) {
            return handle(&m_obj, deleter(std::move(lock)));
        } else {
            return handle(nullptr, deleter(std::move(lock)));
        }
    }

private:
    T m_obj;
    M m_mutex;
};



class {
    void store(std::shared_ptr<T> desire);
    shared_ptr<T> load();
    bool compare_exchange_weak(auto& expected, shared_ptr<T>&& desired);
    snapshot_ptr<T> get_snapshot();

    // atomic<shared+ptr> + snapshot_ptr\
    // get_snapshot() allows us to read without incrementing the reference count
}

struct Node : public hazard_pointer_obj_base<Node> {
    T value;
    atomic<Node*> next;
};
atomic<Node*> head;
// Single (or synchronized) writer
void remove(Node* prev, Node* target) {
    prev->next.store(target->next.load());
    target->next.store(nullptr);
    target->retrie();
};

struct Node {
    T value;
    deferred_shared_ptr<Node*> next;
};
deferred_shared_ptr<Node*> head;
// Single (or synchronized) writer
void remove(Node* prev, Node* target) {
    prev->next.store(target->next.load());
};
// https://www.youtube.com/watch?v=OS7Asaa6zmY

int main() {
    // T();
    V2();
    return 0;
}
