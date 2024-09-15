//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_SELECTIONFEATURE_H
#define BUILDIT_SELECTIONFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/data/selection.h"

class HistoryChangeEvent;
class CursorFeature;
template<typename T>
class CollisionDetection;

class SelectionFeature : public Feature, public SelectionAccessor, public Observer<HistoryChangeEvent>, public Renderable {

private:
    CursorFeature *cursorFeature;
    CollisionDetection<Component> *collisionDetection;

    Selection selection{};
    bool selecting = false;
    Component *clickedComponent = nullptr;
public:
    SelectionFeature(Programs *programs, CursorFeature *cursorFeature, CollisionDetection<Component> *collisionDetection);

    std::list<Component*>* getComponents() override;
    void clearSelection() override;
    void addComponent(Component *component);
    void removeComponent(Component *component);

    void notify(const HistoryChangeEvent& data) override;

    void render() override;

    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;
    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
};


#endif //BUILDIT_SELECTIONFEATURE_H
