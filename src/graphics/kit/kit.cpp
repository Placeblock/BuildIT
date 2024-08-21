//
// Created by felix on 8/10/24.
//

#include "kit.h"

#include "graphics/shapes/shapes.h"
#include "graphics/circuitBoard/elements/nodes/gate.h"
#include "simulation/gate.h"
#include "image/stb_image.h"
#include "graphics/gui/widgets/text.h"

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

    //std::unique_ptr<GUI::Element> textElem = std::make_unique<GUI::Text>(view, uintVec2(), "Test HAHA",
    //                                                                     Alignment::LEFT, Color{255, 255, 0}, 15, this);
    //this->addChild(textElem);

    std::unique_ptr<NodeList> lNodeList = std::make_unique<NodeList>(view, this->calculateNLSize(), this->simulation, this);
    this->nodeList = lNodeList.get();
    std::unique_ptr<CircuitBoard> lCircuitBoard = std::make_unique<CircuitBoard>(view, this->calculateCBSize());
    this->circuitBoard = lCircuitBoard.get();
    std::unique_ptr<GUI::Element> element1 = std::move(lNodeList);
    this->addChild(element1);
    std::unique_ptr<GUI::Element> element2 = std::move(lCircuitBoard);
    this->addChild(element2);
}

void Kit::receiveNode(glm::vec2 pos, std::unique_ptr<Node> node) {
    this->createdNode = std::move(node);
}

void Kit::onMouseAction(glm::vec2 relPos, int button, int mouseAction) {
    Container::onMouseAction(relPos, button, mouseAction);
    if (button == GLFW_MOUSE_BUTTON_LEFT && mouseAction == GLFW_RELEASE) {
        this->createdNode = nullptr;
    }
}

void Kit::onMouseMove(glm::vec2 relPos, glm::vec2 delta) {
    Container::onMouseMove(relPos, delta);
    this->mousePos = relPos;
}

void Kit::prerender(Programs *programs) {
    GUI::HorizontalList::prerender(programs);
    if (this->createdNode != nullptr) {
        if (this->circuitBoard->mouseOver) {
            const glm::vec2 cursorPos = this->circuitBoard->camera.worldToScreen(this->circuitBoard->cursor.pos);
            const glm::vec2 nodePos = glm::vec2(this->circuitBoard->getAbsPos()) + cursorPos;
            this->createdNode->onMove(nodePos, true);
        } else {
            this->createdNode->onMove(this->mousePos, true);
        }
    }
}

void Kit::postrender(Programs *programs) {
    if (this->createdNode != nullptr) {
        Camera tcamera{this->createdNode->cell, -this->createdNode->cell, this->circuitBoard->camera.zoom};
        programs->updateProjectionUniforms(this->view->root->getSize(), tcamera);
    }
    Element::postrender(programs);
}
