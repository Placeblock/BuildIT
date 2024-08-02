//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_UTIL_H
#define BUILDIT_UTIL_H

#include <vector>
#include <algorithm>

template<typename T>
void removeByValue(std::vector<T>* vec, T value) {
    vec->erase(std::remove(vec->begin(), vec->end(), value), vec->end());
}

#endif //BUILDIT_UTIL_H
