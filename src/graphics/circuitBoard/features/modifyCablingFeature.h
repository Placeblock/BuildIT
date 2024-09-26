//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_MODIFYCABLINGFEATURE_H
#define BUILDIT_MODIFYCABLINGFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "cursorFeature.h"
#include "graphics/circuitBoard/components/componentContainer.h"
#include "graphics/circuitBoard/components/cabling/cabling.h"

template<typename T>
class CollisionDetection;
class History;
class HistoryChangeEvent;
class SelectionAccessor;
class NetworkContainer;

class ModifyCablingFeature final : public Feature, public Observer<CursorEvent>,
                                   public Observer<HistoryChangeEvent>, public Renderable {
    History *history;
    CablingRenderer visWiresRenderer;
    CollisionDetection<Interactable> *collisionDetection;
    CursorFeature *cursorFeature;
    SelectionAccessor *selectionAccessor;
    ComponentContainer *componentContainer;
    Cabling *cabling;

    std::shared_ptr<Network> visNetwork = std::make_shared<Network>(glm::vec3{1.0f, 0.8f, 1.0f});
    std::unique_ptr<Joint> startJoint = std::make_unique<Joint>(glm::vec2{}, visNetwork);
    std::unique_ptr<Joint> endJoint = std::make_unique<Joint>(glm::vec2{}, visNetwork);
    std::unique_ptr<Wire> wire = std::make_unique<Wire>(startJoint.get(), endJoint.get(), visNetwork);
    intVec2 startCell{};
    bool creating = false;
    bool aligned = true;
    intVec2 endDelta{};

    void startCable(intVec2 cell);
    void endCable();
    void createCable(intVec2 start, intVec2 end);
    Joint* createOrInsertJoint(glm::vec2 pos);

    intVec2 calculateEndCell() const;
public:
    ModifyCablingFeature(Programs *programs, History *history, CollisionDetection<Interactable> *cd, SelectionAccessor *selectionAccessor,
                         CursorFeature *cursorFeature, ComponentContainer *componentContainer,
                         Cabling *cabling);

    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
    void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) override;

    void notify(const CursorEvent& data) override;
    void notify(const HistoryChangeEvent& data) override;

    void render() override;
};


#endif //BUILDIT_MODIFYCABLINGFEATURE_H
