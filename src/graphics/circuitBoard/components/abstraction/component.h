//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_COMPONENT_H
#define BUILDIT_COMPONENT_H


#include "glm/vec2.hpp"
#include "graphics/circuitBoard/observer.h"
#include "graphics/circuitBoard/components/visitor.h"
#include "movable.h"

class Component : public Visitable, virtual public Movable {
public:
    Component(glm::vec2 pos, glm::vec2 size);
};

struct ComponentAddEvent {
    Component *component;
};

struct ComponentRemoveEvent {
    Component *component;
};


#endif //BUILDIT_COMPONENT_H
