#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

class MemoryPiece{
public:
    MemoryPiece(size_t l){
        std::cout<<"Constructed MemoryPiece of "<<l<<" bytes"<<std::endl;
        bytes = new char[l];
        pos = bytes; 
        len = l;
    }
    ~MemoryPiece(){ 
        delete[] bytes; 
        std::cout<<"Deconstructor of MemoryPiece, sized "<<this->len<<std::endl;
    }
    bool CanFit(size_t l){  return pos - bytes + l <= len;    };
    void* Allocate(size_t l){
        objects_in_memory.push_back(pos);
        pos+=l;
        std::cout<<"Allocated "<<l<<" bytes of memory from MemoryPiece"<<std::endl;
        return (void*) *objects_in_memory.rbegin();
    }
    MemoryPiece *next = nullptr, *prev = nullptr;
    std::vector<char*> objects_in_memory;
private:
    char *bytes = nullptr, *pos;
    size_t len = 0;
};

class MemoryPool{
public:
    MemoryPool(){
        std::cout<<"Constructed MemoryPool"<<std::endl;
    }
    ~MemoryPool(){
        for(auto piece = mp; piece!=nullptr; piece = piece->next){
            piece->~MemoryPiece();
        }
        std::cout<<"Deconstructed MemoryPool"<<std::endl;
    }
    void* Allocate(size_t l){
        if(!mp){
            mp = new MemoryPiece(l);
            last = mp;
            std::cout<<"Allocated "<<l<<" bytes from MemoryPool"<<std::endl;
            return mp->Allocate(l);
        }else{
            if(!last->CanFit(l)){
                k*=2;
                last->next = new MemoryPiece(l*k);
                std::cout<<"Created memory piece of "<<l*k<<" bytes"<<std::endl;
                last->next->prev = last;
                last = last->next;
            }
            std::cout<<"Allocated "<<l<<" bytes from MemoryPool"<<std::endl;
            return last->Allocate(l);
        }
    }
    template<typename T>
    void Deallocate(T* ptr){
        std::vector<char *>::iterator it;
        for(auto piece = mp; piece; piece = piece->next){
            it = std::remove(piece->objects_in_memory.begin(), piece->objects_in_memory.end(), (char*) ptr);
            if(it != piece->objects_in_memory.end()) {
                piece->objects_in_memory.erase(it, piece->objects_in_memory.end());
                delete[] ptr;
                if(piece->objects_in_memory.size()==0) {
                    if(piece->prev) piece->prev->next = piece->next;
                    if(piece->next) piece->next->prev = piece->prev;
                    piece->~MemoryPiece(); 
                }
                break;
            }
        }
        std::cout << "Deallocated "<<sizeof(T)<<" bytes from MemoryPool"<<std::endl;
    }
private:
    MemoryPiece *mp = nullptr, *last = mp;
    size_t k = 1;
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