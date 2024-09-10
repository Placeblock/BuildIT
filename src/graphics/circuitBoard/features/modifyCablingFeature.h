//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_MODIFYCABLINGFEATURE_H
#define BUILDIT_MODIFYCABLINGFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "cursorFeature.h"
#include "graphics/circuitBoard/components/componentContainer.h"

template<typename T>
class CollisionDetection;
class History;
class SelectionAccessor;

class ModifyCablingFeature : public Feature, public Observer<CursorEvent> {
private:
    History *history;
    CablingRenderer visWiresRenderer;
    CollisionDetection<Component> *collisionDetection;
    CursorFeature *cursorFeature;
    SelectionAccessor *selectionAccessor;
    ComponentContainer *componentContainer;
    WireContainer *wireContainer;

    std::unique_ptr<Network> visNetwork = std::make_unique<Network>(glm::vec3{1.0f, 0.8f, 1.0f});
    std::unique_ptr<Wire> wire;
    std::unique_ptr<Joint> startJoint;
    std::unique_ptr<Joint> endJoint;
    intVec2 startCell;
    bool creating = false;

    void startCable(intVec2 cell);
    void endCable();
    void createCable(intVec2 start, intVec2 end);
    void createOrInsertJoint(std::unique_ptr<Joint> &joint);

    static intVec2 calculateEndCell(intVec2 startCell, intVec2 hoveringCell, glm::vec2 cursorPos);
    intVec2 calculateEndCell();
public:
    ModifyCablingFeature(History *history, CollisionDetection<Component> *cd, SelectionAccessor *selectionAccessor,
                         CursorFeature *cursorFeature, WireContainer *wireContainer, ComponentContainer *componentContainer);

    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void notify(const CursorEvent& data) override;
};


#endif //BUILDIT_MODIFYCABLINGFEATURE_H
