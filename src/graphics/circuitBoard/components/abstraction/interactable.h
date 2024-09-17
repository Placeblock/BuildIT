//
// Created by felix on 9/17/24.
//

#ifndef BUILDIT_INTERACTABLE_H
#define BUILDIT_INTERACTABLE_H


#include "glm/vec2.hpp"
#include "boundingBox.h"

class Interactable {
public:
    [[nodiscard]] virtual bool intersects(glm::vec2 pos) const = 0;
    [[nodiscard]] virtual bool intersects(BoundingBox bb) const = 0;

    virtual ~Interactable() = default;
};


#endif //BUILDIT_INTERACTABLE_H
