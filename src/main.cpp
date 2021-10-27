#include <iostream>
#include <map>
#include <memory>

#include "Allocator.h"
#include "Donut.h"

using namespace std;

int main(int, char *[]) {
    auto m_standard = map<int,int>{};
    m_standard[0] = 1;
    for (int i = 1; i < 10; ++i) {
        m_standard[i] = m_standard[i-1]*i;
    }
    cout << "Map without specified allocator:" << endl;
    for(const auto& el : m_standard){
        cout <<  el.first << " " << el.second << endl; 
    }

    auto m = std::map<
        int, int,
        std::less<int>,
        MyAllocator<std::pair<const int, unsigned>> 
    >{};
    m[0] = 1;
    for (int i = 1; i < 10; ++i) {
        m[i] = m[i-1]*i;
    }
    cout << "Map with my allocator:" << endl;
    for(const auto& el : m){
        cout <<  el.first << " " << el.second << endl; 
    }

    auto d_standard = Donut<int, std::allocator<DonutPiece<int>>>();
    for (int i = 0; i < 10; ++i) {
        d_standard.add_bite(i);
    }
    cout << "Donut with standard allocator:" << endl;
    for(auto i = 0; i < 10; ++i){
        cout<< d_standard[i] << " ";
    }
    cout<<endl;
    auto d = Donut<int, MyAllocator<DonutPiece<int>>>();
    for (int i = 0; i < 10; ++i) {
        d.add_bite(i);
    }
    cout << "Donut with my allocator:" << endl;
    for(auto i = 0; i < 10; ++i){
        cout<< d[i] << " ";
    }
    cout<<endl;

    return 0;
}