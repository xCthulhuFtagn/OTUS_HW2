#include <iostream>
#include <vector>
#include <map>
#include <memory>

class MemoryPool{
public:
    size_t GetLength(){ return length; }
    MemoryPool(){}
    ~MemoryPool(){
        delete[] bytes_pool;
    }
    void* Allocate(size_t l){
        if(!bytes_pool){
            bytes_pool = new char[l];
        }
        if(*adresses.rbegin() + l <= length){
            size_t end = (adresses.empty())? 0 : *adresses.rbegin() + 1;
            adresses.push_back(l + end);
        }else{
            char* new_bytes_pool = new char[length*2];
            for(size_t i = 0; i < length; ++i){
                new_bytes_pool[i] = bytes_pool[i];
            }
            delete[] bytes_pool;
            bytes_pool = new_bytes_pool;
            length*=2;
        }
        return (void*) bytes_pool + *adresses.rbegin();
    }
    template<typename T>
    void Deallocate(T* ptr){
        size_t l = sizeof(T);
    }
private:
    char* bytes_pool = nullptr;
    std::vector<size_t> adresses;
    size_t length = 0;
};

template<typename T>
struct MyAllocator {
    using value_type = T;
    MyAllocator(){}
    template<typename U>
    MyAllocator(const MyAllocator<U>&) {}
    T *allocate(std::size_t n) {
        std::cout << "allocating memory "<<n<<std::endl;
        auto p = storage.Allocate(n);
        if (!p)
            throw std::bad_alloc();
        return reinterpret_cast<T *>(p);
    }
    void deallocate(T *p, std::size_t n) {
        std::cout << "deallocating memory "<<n<<std::endl;
        //std::free(p);
    }
    private:
        MemoryPool storage;
};

int main(int, char *[]) {
    auto v = std::vector<int, MyAllocator<int>>{};
    // v.reserve(5);
    for (int i = 0; i < 6; ++i) {
        std::cout << "vector size = " << v.size() << std::endl;
        v.emplace_back(i);
        //std::cout << std::endl;
    }

    auto m = std::map<
        int,
        unsigned,
        std::less<int>,
        MyAllocator<std::pair<const int, float>> 
        >{};
    for (int i = 0; i < 10; ++i) {
        m[i] = static_cast<float>(i);
    }
    return 0;
}