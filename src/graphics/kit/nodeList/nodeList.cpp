//
// Created by felix on 8/11/24.
//

#include "nodeList.h"
#include "image/stb_image.h"
#include "nodeElement.h"

NodeList::NodeList(GUI::View *view, uintVec2 size, const std::vector<NodeElementData>& elements, GUI::Element *parent)
    : GUI::VerticalList(view, size, parent) {
    for (const auto &element: elements) {
        std::unique_ptr<GUI::Element> nodeElement = std::make_unique<NodeElement>(view, element.icon, element.name, this);
        this->addChild(nodeElement);
    }
}

