//
// Created by felix on 8/10/24.
//

#include "kit.h"

#include "graphics/shapes/shapes.h"
#include "simulation/gate.h"
#include "image/stb_image.h"
#include "graphics/gui/widgets/text.h"
#include "graphics/circuitBoard/history/actions/createNodeAction.h"

void Kit::updateSize(uintVec2 newSize) {
    GUI::HorizontalList::updateSize(newSize);
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

Kit::Kit(GUI::View* view, Sim::Simulation* simulation, uintVec2 size)
    : simulation(simulation), FrameBufferRenderable(size),
    GUI::HorizontalList(view, size) {

    std::unique_ptr<NodeList> lNodeList = std::make_unique<NodeList>(view, this->calculateNLSize(), this->simulation, this);
    this->nodeList = lNodeList.get();
    std::unique_ptr<CircuitBoard> lCircuitBoard = std::make_unique<CircuitBoard>(view, this->calculateCBSize(), this->simulation);
    this->circuitBoard = lCircuitBoard.get();
    std::unique_ptr<GUI::Element> element1 = std::move(lNodeList);
    this->addChild(element1);
    std::unique_ptr<GUI::Element> element2 = std::move(lCircuitBoard);
    this->addChild(element2);
}

void Kit::setActiveNodeAdder(NodeAdder *adder) {
    this->activeNodeAdder = adder;
}

void Kit::onMouseAction(glm::vec2 relPos, int button, int mouseAction) {
    Container::onMouseAction(relPos, button, mouseAction);
    if (button == GLFW_MOUSE_BUTTON_LEFT && mouseAction == GLFW_RELEASE
            && this->activeNodeAdder != nullptr) {
        if (this->circuitBoard->mouseOver) {
            std::shared_ptr<Node> node = this->activeNodeAdder->addNode(this->circuitBoard);
            std::unique_ptr<Action> createAction = std::make_unique<CreateNodeAction>(&this->circuitBoard->simBridge, node, false);
            this->circuitBoard->history.dispatch(createAction);
        }
        this->activeNodeAdder->removeNode();
        this->activeNodeAdder = nullptr;
    }
}

void Kit::prerender(Programs *programs) {
    GUI::HorizontalList::prerender(programs);
    if (this->activeNodeAdder != nullptr) {
        if (this->circuitBoard->mouseOver) {
            const glm::vec2 cursorPos = this->circuitBoard->camera.worldToScreen(this->circuitBoard->cursor.pos);
            const glm::vec2 nodePos = glm::vec2(this->circuitBoard->getAbsPos()) + cursorPos;
            this->activeNodeAdder->moveNode(nodePos / 32.0f);
        } else {
            this->activeNodeAdder->moveNode(this->view->mousePos / 32.0f);
        }
    }
}

float Kit::getBoardZoom() {
    return this->circuitBoard->camera.zoom;
}