//
// Created by felix on 8/11/24.
//

#include "nodeList.h"
#include "image/stb_image.h"
#include "nodeElement.h"

NodeList::NodeList(GUI::View *view, uintVec2 size, const std::vector<NodeElementData>& elements)
    : GUI::VerticalList(view, size) {
    for (const auto &element: elements) {
        std::unique_ptr<GUI::Element> nodeElement = std::make_unique<NodeElement>(view, element.icon, element.name);
        this->addChild(nodeElement);
    }
}

void NodeList::receiveNode(glm::vec2 pos, std::unique_ptr<Node> node) {

}

