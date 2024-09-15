//
// Created by felix on 9/9/24.
//

#ifndef BUILDIT_BOUNDINGBOX_H
#define BUILDIT_BOUNDINGBOX_H


#include "glm/vec2.hpp"

class Movable;

struct BoundingBox {
    glm::vec2 start;
    glm::vec2 size;
};

class Boundable {
    friend class Movable;
private:
    BoundingBox boundingBox;
public:
    explicit Boundable(BoundingBox bb);
    [[nodiscard]] bool intersects(glm::vec2 pos) const;
    [[nodiscard]] bool intersects(BoundingBox bb) const;
};


#endif //BUILDIT_BOUNDINGBOX_H
