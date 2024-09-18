//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_COMPONENT_H
#define BUILDIT_COMPONENT_H


#include "graphics/circuitBoard/components/visitor.h"

class Component : public Visitable {
public:
    void visit(Visitor *visitor) override;
    virtual ~Component() = default;
};

struct ComponentAddEvent {
    Component *component;
};

struct ComponentRemoveEvent {
    Component *component;
};


#endif //BUILDIT_COMPONENT_H
