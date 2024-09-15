//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CAMERA_H
#define BUILDIT_CAMERA_H

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"

class CoordinateConverter {
public:
    [[nodiscard]] virtual glm::vec2 screenToWorld(glm::vec2 screenPos) const = 0;
    [[nodiscard]] virtual glm::vec2 worldToScreen(glm::vec2 worldPos) const = 0;
};

class Camera : public CoordinateConverter {
public:
    explicit Camera(glm::vec2 target = glm::vec2{}, glm::vec2 offset = glm::vec2{}, float zoom = 1.0f) : target(target), offset(offset), zoom(zoom) {};
    glm::vec2 target;
    glm::vec2 offset;
    float zoom;
    [[nodiscard]] float getZoomScalar() const;
    [[nodiscard]] glm::vec2 getPos() const;
    [[nodiscard]] glm::mat4 getProjectionMat(glm::vec2 screenSize) const;
    [[nodiscard]] glm::vec2 screenToWorld(glm::vec2 screenPos) const override;
    [[nodiscard]] glm::vec2 worldToScreen(glm::vec2 worldPos) const override;
};


#endif //BUILDIT_CAMERA_H
