//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_SERIALIZATIONVISITOR_H
#define BUILDIT_SERIALIZATIONVISITOR_H

#include <sstream>
#include <unordered_set>
#include <string>
#include "graphics/circuitBoard/components/visitor.h"

class Component;

class SerializationVisitor : public Visitor {
protected:
    std::unordered_set<Component*> components;
public:
    explicit SerializationVisitor(std::unordered_set<Component*> components);

    std::stringstream serialized{};

    virtual void serialize() = 0;
};


#endif //BUILDIT_SERIALIZATIONVISITOR_H
