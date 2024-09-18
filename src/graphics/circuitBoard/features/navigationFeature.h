//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_NAVIGATIONFEATURE_H
#define BUILDIT_NAVIGATIONFEATURE_H

#include "graphics/data/camera.h"
#include "feature.h"
#include "graphics/circuitBoard/observer.h"

struct BoardZoomEvent {
    float newZoom;
};

struct BoardMoveEvent {
    glm::vec2 delta;
};

/**
 * Handles the dragging and zooming of the circuit board
 */
class NavigationFeature final : public Feature, public Subject<BoardZoomEvent>, public Subject<BoardMoveEvent> {
public:
    explicit NavigationFeature(Camera *camera);
    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;
    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void onScroll(glm::vec2 relPos, glm::vec2 offset) override;
private:
    bool navigating = false;
    Camera *camera;
};


#endif //BUILDIT_NAVIGATIONFEATURE_H
