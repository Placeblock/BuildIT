//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_MOVEFEATURE_H
#define BUILDIT_MOVEFEATURE_H

#include <unordered_set>
#include "feature.h"
#include "graphics/circuitBoard/components/abstraction/component.h"
#include "cursorFeature.h"
#include "graphics/circuitBoard/components/renderer/componentRenderers.h"
#include "graphics/circuitBoard/history/history.h"

template<typename T>
class CollisionDetection;
class SelectionAccessor;
class CursorFeature;
class FontRenderer;

/**
 * Adds functionality for moving things around
 */
class MoveFeature : public Feature, public Observer<CursorEvent> {
private:
    History *history;
    CollisionDetection<Component> *collisionDetection;
    SelectionAccessor *selectionAccessor;
    CursorFeature *cursorFeature;
    std::unordered_set<Component*> movingComponents;

    ComponentRenderers visRenderers;
    glm::vec2 moveDelta{};
public:
    MoveFeature(History *history, CollisionDetection<Component> *collisionDetection, SelectionAccessor *selectionAccessor,
                CursorFeature *cursorFeature, FontRenderer *fontRenderer);

    void updateMovingComponents();

    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void notify(const CursorEvent& data) override;
};


#endif //BUILDIT_MOVEFEATURE_H
