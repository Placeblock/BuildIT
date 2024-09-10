//
// Created by felix on 9/10/24.
//

#include <GLFW/glfw3.h>
#include "historyFeature.h"

HistoryFeature::HistoryFeature(History *history) : history(history) {

}

void HistoryFeature::onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {
    if (mods & GLFW_MOD_CONTROL && action != GLFW_PRESS) return;

    if (key == GLFW_KEY_Y) {
        this->history->undo();
    } else if (key == GLFW_KEY_Z) {
        this->history->redo();
    }
}
