//
// Created by felix on 9/17/24.
//

#ifndef BUILDIT_LINEINTERACTABLE_H
#define BUILDIT_LINEINTERACTABLE_H


#include "interactable.h"

class LineInteractable : public Interactable {
protected:
    [[nodiscard]] virtual glm::vec2 getStartPos() const = 0;
    [[nodiscard]] virtual glm::vec2 getEndPos() const = 0;
public:
    [[nodiscard]] bool intersects(glm::vec2 pos) const override;
    [[nodiscard]] bool intersects(BoundingBox otherBB) const override;
};


#endif //BUILDIT_LINEINTERACTABLE_H
