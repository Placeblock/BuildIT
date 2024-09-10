//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_CURSORFEATURE_H
#define BUILDIT_CURSORFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/cursor.h"
#include "graphics/circuitBoard/renderer/cursorRenderer.h"
#include "graphics/circuitBoard/observer.h"

struct CursorEvent {
    glm::vec2 delta;
    glm::vec2 newPos;
};

/**
 * Adds a cursor to the CircuitBoard that snaps to the grid
 */
class CursorFeature : public Feature, public Updatable, public Renderable, public Subject<CursorEvent> {
public:
    CursorFeature(Programs *programs, Camera *camera, MousePosAccessor *mpa);
    void update(float timeStep) override;
    void render() override;

    [[nodiscard]] intVec2 getHoveringCell() const;
    [[nodiscard]] glm::vec2 getHoveringCellDelta() const;
    [[nodiscard]] glm::vec2 getCursorPos() const;
private:
    Camera *camera;
    MousePosAccessor *mpa;
    Cursor cursor;
    CursorRenderer cursorRenderer;
};


#endif //BUILDIT_CURSORFEATURE_H
