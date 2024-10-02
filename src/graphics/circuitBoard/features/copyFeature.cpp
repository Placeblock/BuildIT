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

CopyFeature::CopyFeature(History *history, SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature,
                         ComponentContainer *componentContainer)
    : history(history), selectionAccessor(selectionAccessor), componentContainer(componentContainer),
        cursorFeature(cursorFeature) {

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
}

void CopyFeature::paste() {
}