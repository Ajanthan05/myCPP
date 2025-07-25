#include <semaphore>
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

/*  
Mutex                                       vs      Eemaphore
One thread goes to critical section                 Mutiple
locked or Unlocked                                  Count
Locking Mechanism  (Key Mechanism)                  Signaling Mechanism (producer Consumer sigmaling)
Mutual execlution                                   (Here not Binary Semaphore) It has phole of resources (say m
                                                    Threads N => here Semaphore mechanism only allow m thread (Maintain count) to access resources


semaphore is used for signaling purpose
Mutex for ownership*/

/*  1. What is a Semaphore?
A semaphore is a synchronization primitive that controls access to a shared resource through the use of a counter.

Counting Semaphore: Allows a fixed number of concurrent accesses.

Binary Semaphore (like a mutex): Allows only one access at a time (counter is either 0 or 1).

std::counting_semaphore<N>
N is the maximum number of tokens (compile-time constant).
You initialize it with the current number of available tokens.

    std::counting_semaphore<5> sem{3}; // max = 5, initially 3 tokens

| Function                  | Description                                 |
| ------------------------- | ------------------------------------------- |
| `acquire()`               | Blocks until a token is available           |
| `try_acquire()`           | Returns immediately; true if token acquired |
| `try_acquire_for(dur)`    | Waits up to duration                        |
| `try_acquire_until(time)` | Waits until timeout                         |
| `release()`               | Adds a token (not exceeding max)            |


*/


std::counting_semaphore<3> sem(3); // 3 concurrent threads allowed

void worker(int id) {
    sem.acquire(); // wait for a token
    std::cout << "Thread " << id << " is working\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sem.release(); // return the token
}
// Only 3 threads run at once; others wait for a token.
void Counting_Semaphore() {
    std::thread threads[5];
    for (int i = 0; i < 5; ++i)
        threads[i] = std::thread(worker, i);

    for (auto& t : threads)
        t.join();
}
/*
| Scenario                     | Why Semaphores Are Useful             |
| ---------------------------- | ------------------------------------- |
| **Rate limiting**            | Limit number of tasks in flight       |
| **Thread pools**             | Restrict number of concurrent workers |
| **Producer-consumer queues** | Control buffer capacity               |
| **GPU/DB connections**       | Limit access to scarce resources      |
| **Task throttling**          | Prevent overload                      |
*/

// 🔹 7. RAII Style Token Pool     To prevent forgetting to release:
using Sem = std::counting_semaphore<10>;

struct SemReleaser {
    void operator()(Sem* s) const { s->release(); }
};

class TokenPool {
    Sem sem_{5};
    using Token = std::unique_ptr<Sem, SemReleaser>;

public:
    Token acquire() {
        sem_.acquire();
        return Token(&sem_);
    }
};
// RAII ensures token is released automatically when the token goes out of scope.


/*  Global binary Semaphore Instances
Object counts are set to zero
Objects are in non-signaled state*/
std::binary_semaphore SignalMainToThread{0}, SignalThreadToMain{0};

void ThreadProcess() {
    // Wait for a signal from the main proc
    // by attempting to decrement the semaphore
    SignalMainToThread.acquire();
    // This call blocks until the semaphore's count
    // This increase from the main proc

    cout << "Thread got the signal\n";
    
    using namespace std::literals;
    std::this_thread::sleep_for(3s);

    cout << "Thread Send the signal\n";
    
    SignalThreadToMain.release();
}
void T_binary_semaphore() {
    std::thread Worker(ThreadProcess);

    cout << "Main Sending the signal\n";

    // Signal the worker thread to start working
    SignalMainToThread.release();

    // Wait until worker thread is done doing the work
    // By attempting to decrement the semaphore's count
    SignalThreadToMain.acquire();

    cout << "Main received signal from thread\n";

    Worker.join();
}

/*  std::counting_semaphore<5>
➤ The template argument 5 is the maximum number of tokens the semaphore can hold.
➤ You can never release() more than 5 tokens total at any given time.

{3} is the initial count
➤ This is how many tokens the semaphore starts with — in this case, 3 tokens are initially available.

    Imagine a room with 5 chairs (max = 5), but only 3 people can initially sit (3 tokens available).
    If 3 threads call .acquire(), they will all pass through.
    A 4th thread calling .acquire() will block.
    You can .release() up to 2 more times (to reach max 5).
    A 6th .release() will do nothing or is undefined behavior depending on the platform.


Semaphore  Vs Mutex
Both are synchronization premitive
Both help to syngronize resourse

    Mutex                               Vs              Semaphore
    Allow one thread to go inside           Allow multiple theread to go inside
    Lock / Unlock                           Count
    Locking Mechanism                       Signaling Mechanism
                                                Producer    Buffer Consumer
                                                Produce buffer
                                                Inform consumer Buffer been produce
                                                                    Consumer start consuming
                                                                    Tell the producer buffer been consumed

                                            In n no of thread only number od count is allowed to access resource 


*/

#define BUFFER_SIZE 5
int buff[BUFFER_SIZE];
std::binary_semaphore signal_to_producer{1}, signal_to_consumer{0};

void Producer() {
    while(1) {
        signal_to_producer.acquire();
        cout << "Produced = ";
        for(int i=0; i<BUFFER_SIZE; ++i) {
            buff[i] = i*i; // Task of producer
            cout << buff[i] <<  " " << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        cout << endl;
        signal_to_consumer.release();
    }
}

void Consumer() {
    while(1) {
        signal_to_consumer.acquire();
        cout << "Consumed = ";
        for(int i=BUFFER_SIZE-1; i>=0; --i) {
            cout << buff[i] <<  " " << std::flush;
            buff[i] = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        cout << endl;
        signal_to_producer.release();
    }
}
void ProducerConsumer_binary_semaphore() {
    std::thread producer_thread(Producer);
    std::thread consumer_thread(Consumer);

    producer_thread.join();
    consumer_thread.join();
}

int main() {
    // Counting_Semaphore();
    T_binary_semaphore();

    ProducerConsumer_binary_semaphore();
}