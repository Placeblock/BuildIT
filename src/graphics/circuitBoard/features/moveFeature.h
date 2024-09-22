//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_MOVEFEATURE_H
#define BUILDIT_MOVEFEATURE_H

#include <unordered_set>
#include "feature.h"
#include "cursorFeature.h"
#include "graphics/circuitBoard/components/renderer/componentRenderers.h"
#include "graphics/data/camera.h"

template<typename T>
class CollisionDetection;
class SelectionAccessor;
class CursorFeature;
class FontRenderer;
class History;
class HistoryChangeEvent;

/**
 * Adds functionality for moving things around
 */
class MoveFeature final : public Feature, public Observer<CursorEvent>,
                          public Observer<HistoryChangeEvent>, public Renderable {
    History *history;
    Camera *boardCamera;
    intVec2 *boardSize;
    CollisionDetection<Interactable> *collisionDetection;
    SelectionAccessor *selectionAccessor;
    CursorFeature *cursorFeature;

    std::unordered_set<Movable*> movables;
    ComponentRenderers visRenderers;
    glm::vec2 moveDelta{};
    intVec2 startCell{};
public:
    MoveFeature(Programs *programs, History *history, Camera *camera, intVec2 *boardSize, CollisionDetection<Interactable> *collisionDetection,
                SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature, FontRenderer *fontRenderer);

    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void notify(const CursorEvent& data) override;
    void notify(const HistoryChangeEvent& data) override;

    void updateMovables(glm::vec2 delta);

    void addMovable(Movable *movable);

    void endMove();

    void render() override;
};


#endif //BUILDIT_MOVEFEATURE_H
