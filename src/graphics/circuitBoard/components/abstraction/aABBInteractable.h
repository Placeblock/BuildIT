//
// Created by felix on 9/17/24.
//

#ifndef BUILDIT_AABBINTERACTABLE_H
#define BUILDIT_AABBINTERACTABLE_H


#include "interactable.h"

class AABBInteractable : public Interactable {
private:
    BoundingBox boundingBox;
protected:
    void updateBoundingBox(BoundingBox newBB);
    void updateBoundingBoxPos(glm::vec2 pos);
public:
    explicit AABBInteractable(BoundingBox boundingBox);

    [[nodiscard]] bool intersects(glm::vec2 pos) const override;
    [[nodiscard]] bool intersects(BoundingBox otherBB) const override;
};


#endif //BUILDIT_AABBINTERACTABLE_H
