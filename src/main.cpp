#include <iostream>
#include <vector>
#include <map>

#include "Allocator.h"
#include "Donut.h"

int main(int, char *[]) {
    auto v = std::vector<int, MyAllocator<int>>{};
    for (int i = 0; i < 6; ++i) {
        std::cout << "vector size = " << v.size() + 1 << std::endl;
        v.emplace_back(i);
    }
    auto m = std::map<
        int,
        unsigned,
        std::less<int>,
        MyAllocator<std::pair<const int, unsigned>> 
        >{};
    for (int i = 0; i < 10; ++i) {
        std::cout << "map size = " << m.size() << std::endl;
        m[i] = static_cast<unsigned>(i);
    }
    auto d = Donut<int, MyAllocator<DonutPiece<int>>>();
    for (int i = 0; i < 10; ++i) {
        std::cout << "donut size = " << i << std::endl;
        d.add_bite(i);
        for(auto s = 0; s <= i; ++s) std::cout << d[s] << " ";
        std::cout<<std::endl;
    }
    return 0;
}