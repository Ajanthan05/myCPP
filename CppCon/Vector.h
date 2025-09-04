#pragma once
#include <cstddef>
#include <utility>
#include <memory>
#include <new>   // for placement new
#include <cassert>
#include <stdexcept>

#include <initializer_list>
#include <cstdint>
#include <cstring>

template <typename Vector>
class VectorIterator {
public:
    using ValueType = typename Vector::ValueType;
    using PointerType = ValueType*;
    using ReferenceType = ValueType&;

public:
    VectorIterator(PointerType ptr) : _mPtr(ptr) {}

    VectorIterator& operator++() {
        _mPtr++;
        return *this;
    }
// Postfix Operator :- returning the copy of the VectorIterator
    VectorIterator operator++(int ) {
        VectorIterator iterator = *this;
        ++(*this);
        return iterator;
    }


    VectorIterator& operator--() {
        _mPtr--;
        return *this;
    }
    VectorIterator operator--(int ) {
        VectorIterator iterator = *this;
        --(*this);
        return iterator;
    }

    ReferenceType operator[](int index) {
        return *(_mPtr + index);
    }

    PointerType operator->() {
        return _mPtr;
    }

    ReferenceType operator*() {
        return *_mPtr;
    }

    bool operator==(const VectorIterator& other) const {
        return _mPtr == other._mPtr;
    }
    bool operator!=(const VectorIterator& other) const {
        return _mPtr != other._mPtr;
    }

 private:
    PointerType _mPtr;
};


template <typename T>
class Vector {
public:
    using ValueType = T;
    using iterator = VectorIterator<Vector<T>>;
public:
    Vector() {
        realloc(2);
    }

    Vector(std::initializer_list<T> init) {
        realloc(init.size());
        for (const auto& elem : init) {
            push_back(elem);
        }
    }

    ~Vector() {
        clear();
        ::operator delete(mData_, mCapacity_ * sizeof(T)); 
    }

    void clear() {
        for (size_t i=0; i<mSize_; ++i) {
            mData_[i].~T();
        }
        mSize_ = 0;
    }

    void push_back(const T& value) {
        if (mSize_ >= mCapacity_) realloc(mCapacity_ * 2);

        // mData_[mSize_++] = std::move(value);
        new(&mData_[mSize_]) T(value); // copy-construct
        mSize_++;
    }
    void push_back(T&& value) {
        if (mSize_ >= mCapacity_) realloc(mCapacity_ * 2);

        // mData_[mSize_++] = std::move(value);
        new(&mData_[mSize_]) T(std::move(value)); // move-constructor
        mSize_++;
    }

    template<typename... Args>
    T& emplace_back(Args&&... args) {
        if (mSize_ >= mCapacity_) realloc(mCapacity_ * 2);

        new(&mData_[mSize_]) T(std::forward<Args>(args)...);
        return mData_[mSize_++];
    }

    void pop_back() {
        if (mSize_ > 0) {
            mSize_--;
            mData_[mSize_].~T();
        }
    }

    T& operator[](size_t index) { 
        if (index >= mSize_) { 
            throw std::out_of_range("Vector overflow: index out of range");
        }
        return mData_[index]; 
    }
    const T& operator[](size_t index) const { 
        if (index >= mSize_) { 
            throw std::out_of_range("Vector overflow: index out of range");
        }
        return mData_[index]; 
    }

    size_t size() {
        return mSize_;
    }
    size_t capacity() const {
        return mCapacity_;
    }

    // void realloc(size_t newCapacity) {
    //     if (mSize_ > newCapacity) {
    //         mSize_ = newCapacity;
    //         // How to clear rest
    //         return;
    //     }
    //     // T* newBlock = new T[newCapacity];
    //     T* newBlock = (T*)::operator new(newCapacity * sizeof(T)); 

    //     for(size_t i=0; i<mSize_; ++i) {
    //         newBlock[i] = std::move(mData_[i]);
    //     }
        
    //     for (size_t i=0; i<mSize_; ++i) {
    //         mData_[i].~T();
    //     }

    //     // delete[] mData_;
    //     ::operator delete(mData_, mCapacity_ * sizeof(T)); 
    //     mData_ = newBlock;
    //     mCapacity_ = newCapacity;
    // }

    void realloc(size_t newCapacity) {
        // allocate raw memory
        T* newBlock = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

        // determine how many elements to move (truncate if shrinking)
        size_t elementsToMove = (mSize_ < newCapacity) ? mSize_ : newCapacity;

        if constexpr (std::is_trivially_copyable_v<T>) {
            // Fast path: raw memory copy
            std::memcpy(newBlock, mData_, mSize_ * sizeof(T));
        } else {
            for (size_t i = 0; i < elementsToMove; i++) {
                // Slow path: move construct each element
                new(&newBlock[i]) T(std::move(mData_[i])); // move-construct
                mData_[i].~T(); // destroy old
            }
        }

        // destroy extra elements if we shrank
        for (size_t i = elementsToMove; i < mSize_; i++) {
            mData_[i].~T();
        }

        // free old memory
        ::operator delete(mData_, mCapacity_ * sizeof(T));

        mData_ = newBlock;
        mCapacity_ = newCapacity;
        mSize_ = elementsToMove; // adjust size if we truncated
    }


    iterator begin() {
        return iterator(mData_);
    }
    iterator end() {
        return iterator(mData_ + mSize_);
    }

private:
    T* mData_ = nullptr;
    std::size_t mSize_ = 0;
    std::size_t mCapacity_ = 0;
};

/*
🔹 Case 1: new T[newCapacity]

This allocates memory and default-constructs newCapacity objects of type T.

If T = std::string, this will run std::string() for each slot.

But in your design, you don’t want all slots constructed — only the ones you push_back or emplace_back.

Wasteful and wrong for uninitialized slots.

🔹 Case 2: ::operator new(newCapacity * sizeof(T))

This only allocates raw memory, without constructing any T objects.

Later, you manually construct them with placement new:

new(&mData_[i]) T(...);


This ensures only mSize_ elements exist at any time.

Works perfectly for std::string, Vector3, or any non-trivial type.

🔹 Why you can’t skip new for std::string

If you tried to just malloc or raw new[] char for std::string, you’d only get memory, not a valid object.

std::string needs its constructor (to allocate its internal buffer, initialize size, etc.).

Without placement new, your memory would be garbage.

✅ Correct logic (for any type, including std::string):

// allocate raw memory
T* newBlock = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

// construct only needed elements
new(&newBlock[i]) T(std::move(mData_[i]));

// destroy when shrinking or clearing
mData_[i].~T();


👉 So:

Fundamental types (int, float, char) don’t really need construction — but placement new is harmless.

Complex types (std::string, your Vector3) absolutely need placement new so constructors run properly.*/