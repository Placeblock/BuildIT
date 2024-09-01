//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_COMPONENT_H
#define BUILDIT_COMPONENT_H


#include "glm/vec2.hpp"

class Component {
public:
    explicit Component(glm::vec2 pos);
    virtual ~Component() = default;
};


#endif //BUILDIT_COMPONENT_H
