//
// Created by felix on 9/23/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "copyFeature.h"
#include "graphics/circuitBoard/components/abstraction/component.h"
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
    Visitor::doFor(joint);
}

void CopyVisitor::doFor(Wire *wire) {
    Visitor::doFor(wire);
}

void CopyVisitor::copy() {
    for (const auto &component: this->toCopy) {
        component->visit(this);
    }
}
