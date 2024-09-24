//
// Created by felix on 8/21/24.
//

#include "notNodeElement.h"
#include "graphics/gui/widgets/image.h"
#include "simulation/updaters.h"
#include "graphics/util.h"

NotNodeElement::NotNodeElement(GUI::View *view, NodeDragHandler *nodeDragHandler, Sim::Simulation* simulation)
        : NodeElement(view, "Not", nodeDragHandler, simulation) {
    uint texture = Util::loadTexture("gate_not.png");
    std::unique_ptr<Element> image = std::make_unique<GUI::Image>(view, uintVec2(160, 160), texture);
    this->Container::addChild(image);
}

std::unique_ptr<Gate> NotNodeElement::createNode(glm::vec2 absPos) {
    auto simGate = std::make_shared<Sim::NotGate>();
    return std::make_unique<NotGate>(absPos, "!", simGate);
}