//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CAMERA_H
#define BUILDIT_CAMERA_H

#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec2 target{0.0f, 0.0f};
    glm::vec2 offset{0.0f, 0.0f};
    float zoom = 1.0;
    [[nodiscard]] float getZoomScalar() const;
    [[nodiscard]] glm::vec2 getPos() const;
    [[nodiscard]] glm::mat4 getProjectionMat(glm::vec2 screenSize) const;
    [[nodiscard]] glm::vec2 screenToWorld(glm::vec2 screenPos) const;
};


#endif //BUILDIT_CAMERA_H
