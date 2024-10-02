//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_DESERIALIZER_H
#define BUILDIT_DESERIALIZER_H

#include <unordered_set>
#include <istream>
#include <memory>
#include <queue>
#include "simulation/node.h"

class Component;

class Deserializer {
public:
    std::unordered_set<std::shared_ptr<Component>> components;
    std::queue<Sim::Node*> updateQueue;

    virtual void deserialize(std::istream &in) = 0;

    virtual ~Deserializer() = default;
};


#endif //BUILDIT_DESERIALIZER_H
