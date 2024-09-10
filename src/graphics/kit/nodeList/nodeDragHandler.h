//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODEDRAGHANDLER_H
#define BUILDIT_NODEDRAGHANDLER_H

#include "glm/vec2.hpp"

class Component;

class ComponentDragHandler {
public:
    virtual void setCreatingComponent(std::unique_ptr<Component> component) = 0;
};

#endif //BUILDIT_NODEDRAGHANDLER_H
