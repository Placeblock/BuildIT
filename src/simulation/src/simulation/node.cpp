//
// Created by felix on 29.03.25.
//

#include "simulation/node.hpp"

using namespace sim;

template <typename T>
pin<T>::pin() = default;

template <typename T>
T pin<T>::get_value() const {
    return this->value;
}

template <typename T>
bool pin<T>::set_value(const T value) {
    if (this->value != value) {
        this->value = value;
        return true;
    }
    return false;
}

node::node() = default;