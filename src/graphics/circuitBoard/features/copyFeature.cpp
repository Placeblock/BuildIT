//
// Created by felix on 9/23/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "copyFeature.h"
#include "graphics/circuitBoard/components/cabling/wire.h"
#include "graphics/circuitBoard/components/nodes/notGate.h"

CopyFeature::CopyFeature(History *history, SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature)
    : history(history), selectionAccessor(selectionAccessor), cursorFeature(cursorFeature) {

}

void CopyFeature::onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {
    if (!(mods & GLFW_MOD_CONTROL)) return;
    if (key == GLFW_KEY_C) {

    } else if (key == GLFW_KEY_V) {

    }
}

void CopyVisitor::doFor(NotGate *notGate) {
    this->copies[notGate] = std::make_shared<NotGate>(*notGate);
}

void CopyVisitor::doFor(Joint *joint) {
    this->copies[joint] = std::make_shared<Joint>(*joint);
}

void CopyVisitor::doFor(Wire *wire) {
    if (!this->toCopy.contains(wire->start)) {
        this->doFor(wire->start);
    }
    if (!this->toCopy.contains(wire->end)) {
        this->doFor(wire->end);
    }
    this->copies[wire] = std::make_shared<Wire>(*wire);
}

void CopyVisitor::copy() {
    for (const auto &component: this->toCopy) {
        component->visit(this);
    }
}

void FinalizeCopyVisitor::doFor(NotGate *notGate) {
}

void FinalizeCopyVisitor::doFor(Joint *joint) {
    std::shared_ptr<Joint> jointCopy = std::dynamic_pointer_cast<Joint>(this->copies[joint]);
    for (const auto &wire: joint->wires) {
        std::shared_ptr<Wire> wireCopy = std::dynamic_pointer_cast<Wire>(this->copies[wire]);
        if (this->copies.contains(wire)) {
            jointCopy->wires.insert(wireCopy.get());
        }
    }
}

void FinalizeCopyVisitor::doFor(Wire *wire) {
    std::shared_ptr<Wire> wireCopy = std::dynamic_pointer_cast<Wire>(this->copies[wire]);
    wireCopy->start = std::dynamic_pointer_cast<Joint>(this->copies[wire->start]).get();
    wireCopy->end = std::dynamic_pointer_cast<Joint>(this->copies[wire->end]).get();
}

void FinalizeCopyVisitor::finalize() {
    for (const auto &[toCopy, _]: this->copies) {
        toCopy->visit(this);
    }
}
