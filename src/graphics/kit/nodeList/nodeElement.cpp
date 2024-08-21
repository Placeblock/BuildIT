//
// Created by felix on 8/19/24.
//

#include "nodeElement.h"
#include "graphics/gui/widgets/text.h"
#include "graphics/util.h"

NodeElement::NodeElement(GUI::View *view, const std::string& name, NodeReceiver* nodeReceiver)
    : GUI::VerticalList(view, uintVec2(160, 176)), nodeReceiver(nodeReceiver) {
    std::unique_ptr<GUI::Element> title = std::make_unique<GUI::Text>(view, uintVec2(160, 0),
                                                                      name, Alignment::CENTER, Color{255, 255, 0},
                                                                      16);
    this->addChild(title);
}

void NodeElement::onMouseAction(glm::vec2 relPos, int button, int mouseAction) {
    Container::onMouseAction(relPos, button, mouseAction);
    if (button != GLFW_MOUSE_BUTTON_LEFT || mouseAction != GLFW_PRESS) return;
    this->nodeReceiver->receiveNode(glm::vec2(this->getRelPos()) + relPos, std::move(this->createNode()));
}
