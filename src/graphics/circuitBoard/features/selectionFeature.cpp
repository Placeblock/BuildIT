//
// Created by felix on 9/7/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "selectionFeature.h"
#include "cursorFeature.h"
#include "graphics/circuitBoard/components/collisionDetection.h"
#include "graphics/util.h"
#include "graphics/data/program.h"
#include "glm/gtc/matrix_transform.hpp"
#include "graphics/data/camera.h"

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
    : Renderable(programs), cursorFeature(cursorFeature), collisionDetection(collisionDetection),
      selectionQuadVB(GL_ARRAY_BUFFER, Util::getDefaultLayout()){
    this->selectionQuadVB.bufferData(this->getSelectionVisData());
    this->selectionQuadVA.addBuffer(&this->selectionQuadVB);
}

std::vector<VertexData> SelectionFeature::getSelectionVisData() {
    const Color color{255, 255, 0, 50};
    std::vector<VertexData> data{};
    data.emplace_back(this->selectionBB.start, color);
    data.emplace_back(glm::vec2(this->selectionBB.start.x + this->selectionBB.size.x, this->selectionBB.start.y), color);
    data.emplace_back(glm::vec2(this->selectionBB.start.x, this->selectionBB.start.y + this->selectionBB.size.y), color);
    data.emplace_back(this->selectionBB.start + this->selectionBB.size, color);
    return data;
}

void SelectionFeature::render() {
    if (this->selecting) {
        this->programs->defaultProgram->use();
        this->selectionQuadVB.bind();
        this->selectionQuadVB.bufferSubData(0, this->getSelectionVisData());
        this->selectionQuadVA.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
            this->selectionStart = cursorPos;
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
    glm::vec2 cursorPos = this->cursorFeature->getCursorPos();
    glm::vec2 bbStart = glm::min(cursorPos, this->selectionStart);
    glm::vec2 bbEnd = glm::max(cursorPos, this->selectionStart);
    this->selectionBB.start = bbStart;
    this->selectionBB.size = bbEnd - bbStart;
    this->clickedComponent = nullptr;
    if (this->selecting) {
        this->selection.clearSelection();
        for (const auto &item: this->collisionDetection->getColliding(this->selectionBB)) {
            this->selection.addComponent(item);
        }
    }
}
