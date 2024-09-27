//
// Created by felix on 8/10/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "kit.h"
#include "graphics/gui/widgets/text.h"
#include "graphics/circuitBoard/history/actions/createComponentAction.h"
#include "graphics/circuitBoard/features/cursorFeature.h"

void Kit::updateSize(const uintVec2 newSize) {
    HorizontalList::updateSize(newSize);
    this->updateFrameBufferSize(newSize);
    this->nodeList->updateSize(this->calculateNLSize());
    this->circuitBoard->updateSize(this->calculateCBSize());
}

uintVec2 Kit::calculateCBSize() {
    return this->getSize() - uintVec2(160, 0);
}

uintVec2 Kit::calculateNLSize() {
    return {160, this->getSize().y};
}

Kit::Kit(Programs *programs, GUI::View* view, const uintVec2 size)
    : FrameBufferRenderable(size), HorizontalList(view, size),
    creatingRenderers(&view->fontRenderer) {
    auto lNodeList = std::make_unique<NodeList>(view, this->calculateNLSize(), this);
    this->nodeList = lNodeList.get();
    auto lCircuitBoard = std::make_unique<CircuitBoard>(programs, view, this->calculateCBSize());
    this->circuitBoard = lCircuitBoard.get();
    std::unique_ptr<GUI::Element> element1 = std::move(lNodeList);
    this->Container::addChild(element1);
    std::unique_ptr<GUI::Element> element2 = std::move(lCircuitBoard);
    this->Container::addChild(element2);
}

void Kit::setCreatingNode(std::unique_ptr<Node> node) {
    this->creatingNode = std::move(node);
    RendererAddVisitor addVisitor{&this->creatingRenderers};
    this->creatingNode->visit(&addVisitor);
}

void Kit::onMouseAction(const glm::vec2 relPos, const int button, const int mouseAction, const int mods) {
    Container::onMouseAction(relPos, button, mouseAction, mods);
    if (button == GLFW_MOUSE_BUTTON_LEFT && mouseAction == GLFW_RELEASE
            && this->creatingNode != nullptr) {
        // We remove the node from the node adder first because it gets invalidated next
        RendererRemoveVisitor removeVisitor{&this->creatingRenderers};
        this->creatingNode->visit(&removeVisitor);
        const glm::vec2 delta = glm::vec2(this->circuitBoard->cursorFeature->getHoveringCell() * 32) - this->creatingNode->getPos();
        this->creatingNode->move(delta);
        if (this->circuitBoard->mouseOver) {
            std::unique_ptr<Action> createAction = std::make_unique<CreateComponentAction>(&this->circuitBoard->components,
                                                                                           std::move(this->creatingNode), false);
            History::dispatch(&this->circuitBoard->history, createAction);
        }
    }
}

void Kit::prerender(Programs *programs) {
    HorizontalList::prerender(programs);
    if (this->creatingNode != nullptr) {
        glm::vec2 delta;
        if (this->circuitBoard->mouseOver) {
            const glm::vec2 cursorPos = this->circuitBoard->camera.worldToScreen(this->circuitBoard->cursorFeature->getCursorPos());
            const glm::vec2 nodePos = glm::vec2(this->circuitBoard->getAbsPos()) + cursorPos;
            delta = glm::vec2(nodePos - this->creatingNode->getPos());
        } else {
            delta = this->view->mousePos - this->creatingNode->getPos();
        }
        this->creatingNode->move(delta);
    }
}

void Kit::postrender(Programs *programs) {
    if (this->creatingNode != nullptr) {
        const Camera tcamera{this->creatingNode->getPos(), -this->creatingNode->getPos(), this->getBoardZoom()};
        programs->updateProjectionUniforms(this->view->root->getSize(), tcamera);
        this->creatingRenderers.render(programs);
    }
}

float Kit::getBoardZoom() const {
    return this->circuitBoard->camera.zoom;
}
