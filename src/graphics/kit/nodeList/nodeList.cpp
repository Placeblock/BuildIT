//
// Created by felix on 8/11/24.
//

#include "nodeList.h"
#include "notNodeElement.h"

NodeList::NodeList(GUI::View *view, uintVec2 size, Sim::Simulation* simulation, NodeDragHandler* nodeReceiver)
    : GUI::VerticalList(view, size) {

    std::unique_ptr<GUI::Element> notNodeElement = std::make_unique<NotNodeElement>(view, nodeReceiver, simulation);
    this->addChild(notNodeElement);
}

