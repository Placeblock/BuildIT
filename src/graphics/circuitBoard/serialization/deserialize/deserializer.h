//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_DESERIALIZER_H
#define BUILDIT_DESERIALIZER_H

#include <unordered_set>
#include <istream>
#include <memory>
#include "graphics/circuitBoard/components/visitor.h"

class Component;

class Deserializer {
public:
    std::unordered_set<std::shared_ptr<Component>> components;

    virtual void deserialize(std::istream &in) = 0;
};


#endif //BUILDIT_DESERIALIZER_H
