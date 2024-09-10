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

/**
 * Adds functionality for moving things around
 */
class MoveFeature : public Feature, public Updatable {
private:
    CollisionDetection<Component> *collisionDetection;
    SelectionFeature *selectionFeature;
    std::unordered_set<Component*> movingComponents;

    ComponentRenderers visRenderers;
    glm::vec2 moveDelta{};
public:
    MoveFeature(CollisionDetection<Component> *collisionDetection, SelectionFeature *selectionFeature, FontRenderer *fontRenderer);

    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;
};


#endif //BUILDIT_MOVEFEATURE_H
