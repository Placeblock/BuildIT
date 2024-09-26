//
// Created by felix on 9/23/24.
//

#ifndef BUILDIT_COPYFEATURE_H
#define BUILDIT_COPYFEATURE_H


#include "feature.h"
#include "graphics/circuitBoard/components/visitor.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>

class History;
class SelectionAccessor;
class CursorFeature;
class Component;
class ComponentContainer;
class NetworkContainer;
class Network;

class CopyVisitor : public Visitor {
private:
    std::unordered_set<Component*> toCopy;
    std::unordered_map<Component*, std::shared_ptr<Component>> copies;
public:
    explicit CopyVisitor(std::unordered_set<Component*> toCopy);
    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
    void doFor(Wire *wire) override;

    void copy();
    [[nodiscard]] std::unordered_set<std::shared_ptr<Component>> getCopies() const;
};

class CopyFeature : public Feature {
    History *history;
    SelectionAccessor *selectionAccessor;
    ComponentContainer *componentContainer;
    CursorFeature *cursorFeature;

    std::unordered_set<std::shared_ptr<Component>> copies;

    uint pasteCount = 0;
public:
    CopyFeature(History *history, SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature,
                ComponentContainer *componentContainer);

    void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) override;

    void copy();
    void paste();
};


#endif //BUILDIT_COPYFEATURE_H
