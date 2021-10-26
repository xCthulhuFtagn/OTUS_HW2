#pragma once
#include <cstddef>

template<typename DataType>
struct DonutPiece{
    DonutPiece() = default;
    DonutPiece(const DataType& el) : filling(el){}
    DataType filling;
    DonutPiece *next = nullptr, *prev = nullptr;
};

template<typename DataType, typename AllocatorType>
class Donut{
public:
    Donut(){}
    ~Donut(){}
    void add_bite(const DataType& data);
    void remove_bite(const DataType& data);
    DataType operator[](std::size_t i);
    AllocatorType allocator;
private:
    DonutPiece<DataType> *first_bite;
};

