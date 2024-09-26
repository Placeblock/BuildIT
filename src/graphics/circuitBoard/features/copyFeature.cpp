//
// Created by felix on 9/23/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utility>
#include "copyFeature.h"
#include "graphics/circuitBoard/components/cabling/wire.h"
#include "graphics/circuitBoard/components/nodes/notGate.h"
#include "graphics/circuitBoard/selection/selection.h"
#include "graphics/circuitBoard/history/actions/createComponentAction.h"
#include "graphics/circuitBoard/history/history.h"
#include "graphics/circuitBoard/components/cabling/networkContainer.h"

CopyFeature::CopyFeature(History *history, SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature,
                         ComponentContainer *componentContainer, NetworkContainer *networkContainer)
    : history(history), selectionAccessor(selectionAccessor), componentContainer(componentContainer),
        networkContainer(networkContainer), cursorFeature(cursorFeature) {

}

void CopyFeature::onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {
    if (!(mods & GLFW_MOD_CONTROL) || action != GLFW_PRESS) return;
    if (key == GLFW_KEY_C) {
        this->copy();
    } else if (key == GLFW_KEY_V) {
        this->paste();
    }
}

void CopyFeature::copy() {
    this->pasteCount = 0;
    std::list<Selectable*>* selected = this->selectionAccessor->getSelected();
    std::unordered_set<Component*> toCopy(selected->begin(), selected->end());
    CopyVisitor copyVisitor{toCopy};
    copyVisitor.copy();
    this->copies = copyVisitor.getCopies();
}

void CopyFeature::paste() {
    this->pasteCount++;
    std::unordered_set<Component*> toCopy;
    std::transform(this->copies.begin(), this->copies.end(), std::inserter(toCopy, toCopy.end()),
                   [](const auto &ptr){return ptr.get();});
    CopyVisitor copyVisitor{toCopy};
    copyVisitor.copy();
    for (const auto &network: copyVisitor.getCreatedNetworks()) {
        this->networkContainer->addNetwork(network);
    }
    History::startBatch(this->history);
    for (auto &copy: copyVisitor.getCopies()) {
        if (std::shared_ptr<Movable> movable = std::dynamic_pointer_cast<Movable>(copy)) {
            movable->move(glm::vec2(32*5*this->pasteCount, 32*5*this->pasteCount));
        }
        std::unique_ptr<Action> action = std::make_unique<CreateComponentAction>(this->componentContainer, copy, false);
        History::dispatch(this->history, action);
    }
    History::endBatch(this->history);
}

CopyVisitor::CopyVisitor(std::unordered_set<Component *> toCopy) : toCopy(std::move(toCopy)) {

}

void CopyVisitor::doFor(NotGate *notGate) {
    this->copies[notGate] = std::make_shared<NotGate>(*notGate);
}

void CopyVisitor::doFor(Joint *joint) {
    if (this->copies.contains(joint)) return;
    this->copies[joint] = std::make_shared<Joint>(*joint);
    std::shared_ptr<Joint> jointCopy = std::dynamic_pointer_cast<Joint>(this->copies[joint]);
    std::shared_ptr<Network> jointNetwork = std::make_shared<Network>();
    jointCopy->setNetwork(jointNetwork);
    jointNetwork->joints.push_back(jointCopy.get());
    this->createdNetworks.insert(jointNetwork);
    for (const auto &wire: joint->wires) {
        if (!this->toCopy.contains(wire)) continue;
        if (!this->copies.contains(wire)) {
            wire->visit(this);
        }
        std::shared_ptr<Wire> wireCopy = std::dynamic_pointer_cast<Wire>(this->copies[wire]);
        jointCopy->wires.insert(wireCopy.get());
    }
}

void CopyVisitor::doFor(Wire *wire) {
    if (this->copies.contains(wire)) return;
    this->copies[wire] = std::make_shared<Wire>(*wire);
    std::shared_ptr<Wire> wireCopy = std::dynamic_pointer_cast<Wire>(this->copies[wire]);
    if (!this->toCopy.contains(wire->start) || !this->copies.contains(wire->start)) {
        this->doFor(wire->start);
    }
    if (!this->toCopy.contains(wire->end) || !this->copies.contains(wire->end)) {
        this->doFor(wire->end);
    }
    wireCopy->start = std::dynamic_pointer_cast<Joint>(this->copies[wire->start]).get();
    wireCopy->setNetwork(wireCopy->start->getNetwork());
    wireCopy->start->getNetwork()->wires.push_back(wireCopy.get());
    wireCopy->end = std::dynamic_pointer_cast<Joint>(this->copies[wire->end]).get();
}

void CopyVisitor::copy() {
    for (const auto &component: this->toCopy) {
        component->visit(this);
    }
}

std::unordered_set<std::shared_ptr<Component>> CopyVisitor::getCopies() const {
    std::unordered_set<std::shared_ptr<Component>> copiesSet;
    std::transform(this->copies.begin(), this->copies.end(), std::inserter(copiesSet, copiesSet.end()),
                   [](const auto &pair){return pair.second;});
    return copiesSet;
}

std::unordered_set<std::shared_ptr<Network>> CopyVisitor::getCreatedNetworks() const {
    return this->createdNetworks;
}
