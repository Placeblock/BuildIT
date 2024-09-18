//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_SELECTIONFEATURE_H
#define BUILDIT_SELECTIONFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/selection/selection.h"
#include "graphics/buffer/vertexArray.h"
#include "graphics/circuitBoard/observer.h"
#include "graphics/circuitBoard/components/abstraction/boundingBox.h"

class HistoryChangeEvent;
class CursorFeature;
template<typename T>
class CollisionDetection;
class Interactable;

class SelectionFeature : public Feature, public SelectionAccessor, public Observer<HistoryChangeEvent>, public Renderable {

private:
    CursorFeature *cursorFeature;
    CollisionDetection<Interactable> *collisionDetection;
    Camera *camera;

    Selection selection{};
    bool selecting = false;
    BoundingBox selectionBB{};
    glm::vec2 selectionStart{};
    Selectable *clickedSelectable = nullptr;

    VertexArray selectionQuadVA;
    VertexBuffer<VertexData> selectionQuadVB;

    std::vector<VertexData> getSelectionVisData();
public:
    SelectionFeature(Programs *programs, CursorFeature *cursorFeature, CollisionDetection<Interactable> *collisionDetection);

    std::list<Selectable*>* getSelected() override;
    void clearSelection() override;
    void addSelectable(Selectable *selectable);
    void removeSelectable(Selectable *selectable);

    void notify(const HistoryChangeEvent& data) override;

    void render() override;

    void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;
    void onMouseAction(glm::vec2 relPos, int button, int action, int mods) override;
};


#endif //BUILDIT_SELECTIONFEATURE_H
