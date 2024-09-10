//
// Created by felix on 8/31/24.
//

#ifndef BUILDIT_MOVEFEATURE_H
#define BUILDIT_MOVEFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/components/abstraction/component.h"
#include "graphics/circuitBoard/components/collisionDetection.h"
#include "selectionFeature.h"
#include "graphics/circuitBoard/components/renderer/componentRenderers.h"
#include "cursorFeature.h"

/**
 * Adds functionality for moving things around
 */
class MoveFeature : public Feature, public Updatable, Observer<CursorEvent> {
private:
    CollisionDetection<Component> *collisionDetection;
    SelectionFeature *selectionFeature;
    CursorFeature *cursorFeature;
    std::unordered_set<Component*> movingComponents;

    ComponentRenderers visRenderers;
    glm::vec2 moveDelta{};
public:
    MoveFeature(CollisionDetection<Component> *collisionDetection, SelectionFeature *selectionFeature, CursorFeature *cursorFeature, FontRenderer *fontRenderer);

    void updateMovingComponents();

    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void notify(const CursorEvent& data) override;
};


#endif //BUILDIT_MOVEFEATURE_H
