//
// Created by felix on 29.03.25.
//

#include "simulation/node.hpp"

using namespace Sim;

template <typename T>
Pin<T>::Pin() = default;

template <typename T>
T Pin<T>::getValue() const {
    return this->value;
}

template <typename T>
bool Pin<T>::setValue(const T value) {
    if (this->value != value) {
        this->value = value;
        return true;
    }
    return false;
}

Node::Node() = default;