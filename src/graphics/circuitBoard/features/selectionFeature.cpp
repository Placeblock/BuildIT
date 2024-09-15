//
// Created by felix on 9/7/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "selectionFeature.h"
#include "cursorFeature.h"
#include "graphics/circuitBoard/components/collisionDetection.h"

std::list<Component *> *SelectionFeature::getComponents() {
    return this->selection.getComponents();
}

void SelectionFeature::clearSelection() {
    this->selection.clearSelection();
}

void SelectionFeature::addComponent(Component *component) {
    this->selection.addComponent(component);
}

void SelectionFeature::removeComponent(Component *component) {
    this->selection.removeComponent(component);
}

void SelectionFeature::notify(const HistoryChangeEvent &data) {
    this->clearSelection();
}

SelectionFeature::SelectionFeature(Programs *programs, CursorFeature *cursorFeature, CollisionDetection<Component> *collisionDetection)
    : Renderable(programs), cursorFeature(cursorFeature), collisionDetection(collisionDetection) {

}

void SelectionFeature::render() {

}

void SelectionFeature::onMouseAction(glm::vec2 relPos, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        glm::vec2 cursorPos = this->cursorFeature->getHoveringCell() * 32;
        Component *colliding = this->collisionDetection->getColliding(cursorPos);
        this->clickedComponent = colliding;
    } else if (this->clickedComponent != nullptr) {
        this->selection.addComponent(this->clickedComponent);
        this->clickedComponent = nullptr;
    }
}

void SelectionFeature::onMouseMove(glm::vec2 relPos, glm::vec2 delta) {
    this->clickedComponent = nullptr;
}
