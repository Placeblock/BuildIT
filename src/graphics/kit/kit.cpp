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

    std::unique_ptr<NodeList> lNodeList = std::make_unique<NodeList>(view, this->calculateNLSize(), this->simulation);
    this->nodeList = lNodeList.get();
    std::unique_ptr<CircuitBoard> lCircuitBoard = std::make_unique<CircuitBoard>(view, this->calculateCBSize());
    this->circuitBoard = lCircuitBoard.get();
    std::unique_ptr<GUI::Element> element1 = std::move(lNodeList);
    this->addChild(element1);
    std::unique_ptr<GUI::Element> element2 = std::move(lCircuitBoard);
    this->addChild(element2);
}