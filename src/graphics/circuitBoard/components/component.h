//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_COMPONENT_H
#define BUILDIT_COMPONENT_H


#include "glm/vec2.hpp"
#include "graphics/circuitBoard/observer.h"
#include "visitor.h"

struct ComponentAddEvent {
    Component *component;
};

struct ComponentRemoveEvent {
    Component *component;
};

class Component : public Visitable {
public:
};


#endif //BUILDIT_COMPONENT_H
