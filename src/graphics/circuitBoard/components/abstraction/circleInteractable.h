//
// Created by felix on 9/17/24.
//

#ifndef BUILDIT_CIRCLEINTERACTABLE_H
#define BUILDIT_CIRCLEINTERACTABLE_H


#include "interactable.h"

class CircleInteractable : public Interactable {
    float radius;
public:
    CircleInteractable(std::string cnamespace, std::string ckey, float radius);
    CircleInteractable(const CircleInteractable &other);
    [[nodiscard]] bool intersects(glm::vec2 pos) const override;
    [[nodiscard]] bool intersects(BoundingBox otherBB) const override;
protected:
    [[nodiscard]] virtual glm::vec2 getCenter() const = 0;
};


#endif //BUILDIT_CIRCLEINTERACTABLE_H
