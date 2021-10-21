#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <list>


class MemoryPiece{
public:
    MemoryPiece(size_t l){
        std::cout<<"Constructed MemoryPiece of "<<l<<" bytes"<<std::endl;
        bytes = new char[l];
        len = l;
    }
    ~MemoryPiece(){ 
        delete[] bytes; 
        std::cout<<"Deconstructor of MemoryPiece, sized "<<this->len<<std::endl;
    }
    std::list<void*> SplitBySize(size_t size){
        std::list<void *> ans;
        for (auto pos = bytes; pos != bytes + len; pos+=size)   ans.push_back((void *)pos);
        return ans;
    }
    MemoryPiece *next = nullptr, *prev = nullptr;
private:
    char *bytes = nullptr;
    size_t len = 0;
};

class MemoryPool{
public:
    MemoryPool(){
        std::cout << "Constructor of MemoryPool" << std::endl;
    }
    ~MemoryPool(){
        std::cout << "Deconstructor of MemoryPool" << std::endl;
        auto killer = pieces;
        while(killer){
            auto tmp = killer;
            killer = killer->next;
            tmp->~MemoryPiece();
        }
    }
    void* Allocate(size_t l){
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
    void Deallocate(void* ptr){
        if(std::find(used.begin(), used.end(), ptr) != used.end()){
            used.remove(ptr);
            unused.push_front(ptr);
        }
    }
private:
    MemoryPiece *pieces = nullptr, *last;
    std::list<void*> unused, used;
    size_t k = 2;
};

template<typename T>
struct MyAllocator {
    using value_type = T;
    MyAllocator(){}
    template<typename U>
    MyAllocator(const MyAllocator<U>&) {}
    T *allocate(std::size_t n) {
        auto p = storage.Allocate(n);
        if (!p)
            throw std::bad_alloc();
        return reinterpret_cast<T *>(p);
    }
    void deallocate(T *p, size_t n) {
        storage.Deallocate(p);
    }
    private:
        MemoryPool storage;
};

template<typename T>
void Print(T v){
    for(auto el : v) std::cout << el << " ";
    std::cout<<std::endl;
}

int main(int, char *[]) {
    auto v = std::vector<int, MyAllocator<int>>{};
    for (int i = 0; i < 6; ++i) {
        std::cout << "vector size = " << v.size() + 1 << std::endl;
        v.emplace_back(i);
        Print(v);
    }
    auto m = std::map<
        int,
        unsigned,
        std::less<int>,
        MyAllocator<std::pair<const int, float>> 
        >{};
    for (int i = 0; i < 10; ++i) {
        std::cout << "map size = " << m.size() << std::endl;
        m[i] = static_cast<float>(i);
    }
    return 0;
}