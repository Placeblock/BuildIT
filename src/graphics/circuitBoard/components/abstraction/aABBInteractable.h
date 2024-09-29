//
// Created by felix on 9/17/24.
//

#ifndef BUILDIT_AABBINTERACTABLE_H
#define BUILDIT_AABBINTERACTABLE_H


#include "interactable.h"

class AABBInteractable : public Interactable {
    BoundingBox boundingBox;
protected:
    void updateBoundingBox(BoundingBox newBB);
    void updateBoundingBoxPos(glm::vec2 pos);
public:
    AABBInteractable(std::string cnamespace, std::string ckey, BoundingBox boundingBox);
    AABBInteractable(const AABBInteractable &other);
    [[nodiscard]] bool intersects(glm::vec2 pos) const override;
    [[nodiscard]] bool intersects(BoundingBox otherBB) const override;
};


#endif //BUILDIT_AABBINTERACTABLE_H
