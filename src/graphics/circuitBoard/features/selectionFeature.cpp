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
#include "graphics/data/camera.h"
#include "graphics/circuitBoard/components/abstraction/interactable.h"
#include "graphics/circuitBoard/selection/selectable.h"

std::list<Selectable *> *SelectionFeature::getSelected() {
    return this->selection.getSelected();
}

void SelectionFeature::clearSelection() {
    this->selection.clearSelection();
}

void SelectionFeature::addSelectable(Selectable *selectable) {
    this->selection.select(selectable);
}

void SelectionFeature::removeSelectable(Selectable *selectable) {
    this->selection.deselect(selectable);
}

void SelectionFeature::notify(const HistoryChangeEvent &data) {
    this->clearSelection();
}

SelectionFeature::SelectionFeature(Programs *programs, CursorFeature *cursorFeature, CollisionDetection<Interactable> *collisionDetection)
    : Renderable(programs), cursorFeature(cursorFeature), collisionDetection(collisionDetection),
      selectionQuadVB(GL_ARRAY_BUFFER, Util::getDefaultLayout()){
    this->selectionQuadVB.bufferData(this->getSelectionVisData());
    this->selectionQuadVA.addBuffer(&this->selectionQuadVB);
}

std::vector<VertexData> SelectionFeature::getSelectionVisData() {
    constexpr Color color{255, 255, 0, 50};
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

void SelectionFeature::onMouseAction(glm::vec2 relPos, int button, const int action, const int mods) {
    if (action == GLFW_PRESS) {
        const glm::vec2 cursorPos = this->cursorFeature->getHoveringCell() * 32;
        Interactable *colliding = this->collisionDetection->getColliding(cursorPos);
        if (const auto selectable = dynamic_cast<Selectable*>(colliding)) {
            this->clickedSelectable = selectable;
        }
        if (mods & GLFW_MOD_CONTROL && colliding == nullptr) {
            this->selection.clearSelection();
            this->selecting = true;
            this->selectionBB.size = {};
            this->selectionStart = cursorPos;
        }
        return;
    }
    this->selecting = false;
    if (this->clickedSelectable != nullptr) {
        this->selection.select(this->clickedSelectable);
        this->clickedSelectable = nullptr;
    }
}

void SelectionFeature::onMouseMove(glm::vec2 relPos, glm::vec2 delta) {
    const glm::vec2 cursorPos = this->cursorFeature->getCursorPos();
    const glm::vec2 bbStart = min(cursorPos, this->selectionStart);
    const glm::vec2 bbEnd = max(cursorPos, this->selectionStart);
    this->selectionBB.start = bbStart;
    this->selectionBB.size = bbEnd - bbStart;
    this->clickedSelectable = nullptr;
    if (this->selecting) {
        const std::unordered_set<Interactable*> interacted = this->collisionDetection->getColliding(this->selectionBB);
        this->selection.deselectIf([interacted](Selectable* selected) -> bool {
            return !interacted.contains(dynamic_cast<Interactable*>(selected));
        });
        for (const auto &item: interacted) {
            if (const auto selectable = dynamic_cast<Selectable*>(item)) {
                this->selection.select(selectable);
            }
        }
    }
}

bool SelectionFeature::isSelected(Selectable *selectable) const {
    return this->selection.isSelected(selectable);
}
