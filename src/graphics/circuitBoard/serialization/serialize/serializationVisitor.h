//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_SERIALIZATIONVISITOR_H
#define BUILDIT_SERIALIZATIONVISITOR_H

#include <sstream>
#include <unordered_set>
#include <string>
#include <memory>
#include <queue>
#include "graphics/circuitBoard/components/visitor.h"
#include "simulation/node.h"

class Component;

class SerializationVisitor : public Visitor {
protected:
    std::queue<Sim::Node*> updateQueue;

    std::unordered_set<std::shared_ptr<Component>>* components;
public:
    explicit SerializationVisitor(std::unordered_set<std::shared_ptr<Component>>* components, std::queue<Sim::Node*> updateQueue);

    std::stringstream serialized{};

    virtual void serialize() = 0;
};


#endif //BUILDIT_SERIALIZATIONVISITOR_H
