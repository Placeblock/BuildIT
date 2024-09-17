//
// Created by felix on 9/17/24.
//

#ifndef BUILDIT_CIRCLEINTERACTABLE_H
#define BUILDIT_CIRCLEINTERACTABLE_H


#include "interactable.h"

class CircleInteractable : public Interactable {
private:
    glm::vec2 center;
    float radius;
public:
    CircleInteractable(glm::vec2 center, float radius);

    [[nodiscard]] bool intersects(glm::vec2 pos) const override;
    [[nodiscard]] bool intersects(BoundingBox otherBB) const override;
protected:
    void setCenter(glm::vec2 newCenter);
};


#endif //BUILDIT_CIRCLEINTERACTABLE_H
