//
// Created by felix on 8/21/24.
//

#include "notNodeElement.h"
#include "graphics/util.h"
#include "graphics/gui/widgets/image.h"
#include "graphics/circuitBoard/elements/nodes/gate.h"
#include "simulation/gate.h"

std::unique_ptr<Node> NotNodeElement::createNode(glm::vec2 absPos) {
    std::shared_ptr<Sim::AndGate> simGate = std::make_shared<Sim::AndGate>();
    std::cout << absPos.x << " | " << absPos.y << "\n";
    return std::make_unique<Gate>(intVec2(absPos), this->notMeshRenderer, "&", this->simulation, simGate);
}

NotNodeElement::NotNodeElement(GUI::View *view, NodeReceiver *nodeReceiver, InstancedMeshRenderer* notMeshRenderer, Sim::Simulation* simulation)
    : NodeElement(view, "Not", nodeReceiver, simulation), notMeshRenderer(notMeshRenderer) {
    uint texture = Util::loadTexture("gate_not.png");
    std::unique_ptr<GUI::Element> image = std::make_unique<GUI::Image>(view, uintVec2(160, 160), texture);
    this->addChild(image);
}

void NotNodeElement::postrender(Programs *programs) {
    Element::postrender(programs);
    this->notMeshRenderer->render(programs->instancedProgram);
}
