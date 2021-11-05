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

MemoryPiece::MemoryPiece(std::size_t l){
    //std::cout<<"Constructed MemoryPiece of "<<l<<" bytes"<<std::endl;
    bytes = new char[l];
    len = l;
}
MemoryPiece::~MemoryPiece() { 
    delete[] bytes;
    //std::cout<<"Deconstructor of MemoryPiece, sized "<<this->len<<std::endl;
}
std::list<void *> MemoryPiece::SplitBySize(size_t size){
    std::list<void *> ans;
    for (auto pos = bytes; pos != bytes + len; pos += size)   ans.push_back((void *)pos);
    return ans;
}

MemoryPool::MemoryPool(){
    //std::cout << "Constructor of MemoryPool" << std::endl;
}
MemoryPool::~MemoryPool() {
    //std::cout << "Deconstructor of MemoryPool" << std::endl;
    auto killer = pieces;
    while(killer){
        auto tmp = killer;
        killer = killer->next;
        tmp->~MemoryPiece();
    }
    used.clear();
    unused.clear();
}
void *MemoryPool::Allocate(size_t l){
    if(!pieces){
        pieces = new MemoryPiece(l);
        last = pieces;
        unused = last->SplitBySize(l);
    }
    if(unused.empty()){
        last->next = new MemoryPiece(k*l);
        k *= 2;
        last->next->prev = last;
        last = last->next;
        unused = last->SplitBySize(l);
    }
    void *ans = *unused.begin();
    unused.pop_front();
    used.push_back(ans);
    return ans;
}
void MemoryPool::Deallocate(void *ptr){
    if(std::find(used.begin(), used.end(), ptr) != used.end()){
        used.remove(ptr);
        unused.push_front(ptr);
    }
}

template<class T> MyAllocator<T>::MyAllocator(){
    //std::cout << "Constructor of Allocator" << std::endl;
    storage = new MemoryPool();
}
template<class T> MyAllocator<T>::~MyAllocator(){
    //std::cout << "Deconstructor of Allocator" << std::endl;
    storage->~MemoryPool();
}
template<class T> template<class U> 
MyAllocator<T>::MyAllocator(const MyAllocator<U> &){
    //std::cout << "Constructor of Allocator" << std::endl;
}
template<class T> T *MyAllocator<T>::allocate(std::size_t n){
    auto p = storage->Allocate(n * sizeof(T));
    if (!p)
        throw std::bad_alloc();
    return reinterpret_cast<T *>(p);
}
template<class T> void MyAllocator<T>::deallocate(T *p, size_t n){
    storage->Deallocate(p);
}