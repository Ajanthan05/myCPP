// Implementation.h

#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H
#include <iostream>
#include <stdint.h>



#define IC(...) do { \
    std::cout << "[ " << #__VA_ARGS__ << " ] = "; \
    debug_out(__VA_ARGS__); \
} while (0)

template<typename T>
void debug_out(T t) {
    std::cout << t << std::endl;
}

template<typename T, typename... Args>
void debug_out(T t, Args... args) {
    std::cout << t << ", ";
    debug_out(args...);
}



template <typename Derive>
class CTRPInterface {
private:
    CTRPInterface() = default;
    friend Derive;
    // This will detect the miss match like class CRTPDrived : public CTRPInterface<CRTPDrived_MissMatch> {

public:
    void count(uint64_t n) {
        static_cast<Derive*>(this)->count(n);
    }

    uint64_t getValue() {
        return static_cast<Derive*>(this)->getValue();
    }
};

class CRTPDrived : public CTRPInterface<CRTPDrived> {
public:
    CRTPDrived() : counter(0) {}

    void count(uint64_t n) {
        counter += n;
    }

    uint64_t getValue() {
        return counter;
    }

private:
    uint64_t counter;
};


template <typename T>
void RunCRTP(CTRPInterface<T> *obj) {
    const unsigned N = 40000;
    for (unsigned i = 0; i<N; ++i) {
        for(unsigned j=0; j<i; ++j) {
            obj->count(j);
        }
    }
    IC(obj->getValue());
}

#endif // IMPLEMENTATION_H
