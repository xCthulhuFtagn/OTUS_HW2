#include "Donut.h"

template<class DataType, class AllocatorType> void Donut<DataType, AllocatorType>::add_bite(const DataType &data){
    DonutPiece<DataType>* new_bite = allocator.allocate(1);
    if(!first_bite){
        first_bite = new_bite;
        first_bite->next = first_bite;
        first_bite->prev = first_bite;
    } else{
        first_bite->prev->next = new_bite;
        new_bite->prev = first_bite->prev;
        first_bite->prev = new_bite;
        new_bite->next = first_bite;
    }
    new_bite->filling = data;
}
template<class DataType, class AllocatorType> void Donut<DataType, AllocatorType>::remove_bite(const DataType &data){
    auto runner = first_bite;
    bool found = false;
    do{
        if(runner->filling == data) found = true;
        runner = runner->next;
    }while(runner != first_bite && !found);
    if(found) allocator.deallocate(runner);
}
template<class DataType, class AllocatorType> DataType Donut<DataType, AllocatorType>::operator[](std::size_t i){
    auto ans = first_bite;
    for(auto runner = 0; runner < i; ++runner) 
        ans = ans->next;
    return ans->filling;
}