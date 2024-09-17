//
// Created by felix on 9/17/24.
//

#ifndef BUILDIT_LINEINTERACTABLE_H
#define BUILDIT_LINEINTERACTABLE_H


#include "interactable.h"

class LineInteractable : public Interactable {
private:
    glm::vec2 start;
    glm::vec2 end;
protected:
    void setStart(glm::vec2 newStart);
    void setEnd(glm::vec2 newEnd);
public:
    [[nodiscard]] bool intersects(glm::vec2 pos) const override;
    [[nodiscard]] bool intersects(BoundingBox otherBB) const override;
};


#endif //BUILDIT_LINEINTERACTABLE_H
