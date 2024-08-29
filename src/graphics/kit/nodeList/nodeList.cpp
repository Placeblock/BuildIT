//
// Created by felix on 8/11/24.
//

#include "nodeList.h"
#include "image/stb_image.h"
#include "notNodeElement.h"
#include "graphics/shapes/shapes.h"
#include "graphics/circuitBoard/components/nodes/gate.h"

NodeList::NodeList(GUI::View *view, uintVec2 size, Sim::Simulation* simulation, NodeDragHandler* nodeReceiver)
    : GUI::VerticalList(view, size) {

    std::unique_ptr<GUI::Element> notNodeElement = std::make_unique<NotNodeElement>(view, nodeReceiver, simulation);
    this->addChild(notNodeElement);
}

