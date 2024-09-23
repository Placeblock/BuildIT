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

class CopyVisitor : public Visitor {
private:
    std::unordered_set<Component*> toCopy;
public:
    std::unordered_map<Component*, std::shared_ptr<Component>> copies;
    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
    void doFor(Wire *wire) override;

    void copy();
};

class CopyFeature : public Feature {
    History *history;
    SelectionAccessor *selectionAccessor;
    CursorFeature *cursorFeature;
public:
    CopyFeature(History *history, SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature);

    void onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) override;
};


#endif //BUILDIT_COPYFEATURE_H
