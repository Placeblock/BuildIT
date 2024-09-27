//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_IDENTIFICATION_H
#define BUILDIT_IDENTIFICATION_H

#include <unordered_map>
#include "counter.h"

template<typename T>
class Identificator {

    std::unordered_map<T*, uint32_t> ids;
    Counter counter{};
public:
    uint32_t getID(T* object);
};

template<typename T>
uint32_t Identificator<T>::getID(T* object) {
    if (object == nullptr) return 0;
    if (this->ids.contains(object)) return this->ids[object];
    uint32_t id = this->counter.increaseAndGet();
    this->ids[object] = id;
    return id;
}

#endif //BUILDIT_IDENTIFICATION_H
