//
// Created by felix on 9/7/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "selectionFeature.h"
#include "cursorFeature.h"
#include "graphics/circuitBoard/components/collisionDetection.h"
#include "graphics/util.h"

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
    VertexBuffer<VertexData> vertexBuffer{GL_ARRAY_BUFFER, Util::getDefaultLayout()};
    const Color color{184, 64, 40, 50};
    std::vector<VertexData> data{};
    for (int x = 0; x < 1; ++x) {
        for (int y = 0; y < 1; ++y) {
            data.emplace_back(glm::vec2(x, y), color);
        }
    }
    vertexBuffer.bufferData(data);
    this->selectionQuadVA.addBuffer(&vertexBuffer);
}

void SelectionFeature::render() {
    if (this->selecting) {
        this->selectionQuadVA.bind();
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}

void SelectionFeature::onMouseAction(glm::vec2 relPos, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        glm::vec2 cursorPos = this->cursorFeature->getHoveringCell() * 32;
        Component *colliding = this->collisionDetection->getColliding(cursorPos);
        this->clickedComponent = colliding;
        if (mods & GLFW_MOD_CONTROL && colliding == nullptr) {
            this->selection.clearSelection();
            this->selecting = true;
            this->selectionBB = {cursorPos};
        }
        return;
    }
    this->selecting = false;
    if (this->clickedComponent != nullptr) {
        this->selection.addComponent(this->clickedComponent);
        this->clickedComponent = nullptr;
    }
}

void SelectionFeature::onMouseMove(glm::vec2 relPos, glm::vec2 delta) {
    glm::vec2 cursorPos = this->cursorFeature->getHoveringCell() * 32;
    this->selectionBB.size = cursorPos - this->selectionBB.start;
    this->clickedComponent = nullptr;
    if (this->selecting) {
        this->selection.clearSelection();
        for (const auto &item: this->collisionDetection->getColliding(this->selectionBB)) {
            this->selection.addComponent(item);
        }
    }
}
