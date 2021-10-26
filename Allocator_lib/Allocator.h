#pragma once

#include <list>
#include <iostream>
#include <algorithm>

class MemoryPiece{
public:
    MemoryPiece(std::size_t l);
    ~MemoryPiece();
    std::list<void*> SplitBySize(size_t size);
    MemoryPiece *next = nullptr, *prev = nullptr;
private:
    char* bytes = nullptr;
    size_t len = 0;
};

class MemoryPool{
public:
    MemoryPool();
    ~MemoryPool();
    void* Allocate(size_t l);
    void Deallocate(void* ptr);
private:
    MemoryPiece *pieces = nullptr, *last;
    std::list<void*> unused, used;
    size_t k = 2;
};

template<typename T>
struct MyAllocator {
    using value_type = T;
    MyAllocator();
    ~MyAllocator();
    template<typename U>
    MyAllocator(const MyAllocator<U>&);
    T *allocate(std::size_t n);
    void deallocate(T *p, size_t n);
    private:
        MemoryPool* storage;
};